/**
 * @file SCD41.c
 * @date 4/23/2025 4:16:31 PM
 * @author Farhaan Khan
 * @brief Source file for SCD41 Sensor Functions.
 *
 * Description: This source file contains the definitions of the
 * functions declared in the SCD41.h header file. These functions 
 * read data from the Sensiron SCD41 sensor via I2C using the TWI0
 * module of the AVR128DB48. 
 */

#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "SCD41.h"

// Initialize TWI0 Module
void TWI0_SCD41_init(void)
{
	
	TWI0.MBAUD = 15;	// Set BAUD (standard speed mode) - SCL is 100 kHz
	
	TWI0.MCTRLA |= 0x01; // Enable TWI Master
	
	TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;	// Force bus state to idle
	
	_delay_ms(1050);	// Wait for SCD41 to reach idle state
	
}

// Reads a byte (assumes address has been sent)
uint8_t SCD41_read_byte(void)
{
	uint8_t data;
	
	data = TWI0.MDATA;	// Read byte
	
	TWI0.MCTRLB = 0x02;		// Send acknowledge followed by byte read
	
	while((TWI0.MSTATUS & TWI_RIF_bm) == 0); // Wait for read to complete
	
	return data;
}

// Reads a word (assumes address has been sent)
uint16_t SCD41_read_word(void)
{
	uint16_t data = (uint16_t)SCD41_read_byte();	// Read MSB
	
	data = ( (data << 8) | (uint16_t)SCD41_read_byte() )  ;	//Read LSB
	
	return data;
}

uint16_t SCD41_read_word2(void)
{
	uint16_t data = (uint16_t)SCD41_read_byte();	// Read MSB
	uint16_t data2 = TWI0.MDATA;				    // Read LSB
	TWI0.MCTRLB = 0x07;		//issue nack and send stop bit
	data = ( (data << 8) | data2);
	return data;
	
}

// Writes a word to SCD41 (assumes address has been sent)
void SCD41_write_word(uint16_t data)
{
	TWI0.MDATA = (uint8_t)(data >> 8); // write MSB

	while((TWI0.MSTATUS & TWI_WIF_bm) == 0); // Wait for write operation to complete

	// Check if slave NACKed the data
	if (TWI0.MSTATUS & TWI_RXACK_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;  // Send STOP condition
		return;  // Error: Slave did not acknowledge
	}
	
	TWI0.MCTRLB |= TWI_MCMD_RECVTRANS_gc; // Initiate another byte transfer
	
	TWI0.MDATA = (uint8_t)(data & 0x00FF); // Write LSB
	
	while((TWI0.MSTATUS & TWI_WIF_bm) == 0); // Wait for write operation to complete

}

// Sends a command-word to SCD41
void SCD41_send_command(uint16_t command, uint8_t command_type)
{
	uint8_t addr = 0x62;
	TWI0.MADDR = (addr << 1);	// set slave address and direction bit (W)
	
	while((TWI0.MSTATUS & TWI_WIF_bm) == 0); // wait until address is transmitted
	
	// Check if slave NACKed the address
	if (TWI0.MSTATUS & TWI_RXACK_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;  // Send STOP condition
		exit(-1);  // Error: Slave did not acknowledge
	}
	
	SCD41_write_word(command);	// Send command
	
	// Check if we need to send a stop condition
	if (command_type == SEND_COMMAND_SEQUENCE)
		TWI0.MCTRLB = TWI_MCMD_STOP_gc; // Send STOP condition
}


// Issues read_measurement command and places data in global variables
void SCD41_read_measurement(void)
{
	SCD41_send_command(READ_MEASUREMENT, READ_SEQUENCE);	// send read_measurement command
	_delay_ms(1.5);								// Wait for command to execute
	
	// Write address
	uint8_t addr = 0x62;
	TWI0.MADDR = ((addr << 1) | 0x01);	// set slave address and direction bit (R)
	
	while((TWI0.MSTATUS & TWI_RIF_bm) == 0); // wait until address is transmitted
	
	// Check if slave NACKed the address
	if (TWI0.MSTATUS & TWI_RXACK_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;  // Send STOP condition
		exit(-1);  // Error: Slave did not acknowledge
	}
	
	co2_level_ppm = SCD41_read_word(); // Read C02 level data
	uint8_t CRC = SCD41_read_byte();	// Read CRC

	
	uint16_t temperature_data = SCD41_read_word(); // Read temperature data
	CRC = SCD41_read_byte();	// Read CRC
	
	uint16_t humidity_data = SCD41_read_word(); // Read humidity data
	CRC = TWI0.MDATA;		// Read CRC	

	TWI0.MCTRLB = 0x07; // Issue nack and send STOP condition
	
	// Calculate temperature
	temperature_celcius = -45 + (175 * ((float)temperature_data/0xFFFF));
	
	// Calculate humidity level
	relative_humidity = 100 * ((float)humidity_data/0xFFFF);
	
}

// Issues get_data_ready_status command and returns response
uint16_t SCD41_data_ready(void)
{
	SCD41_send_command(GET_DATA_READY_STATUS, READ_SEQUENCE);	// send get_data_ready_status command
	_delay_ms(1.5);								// Wait for command to execute
	
	// Write address
	uint8_t addr = 0x62;
	TWI0.MADDR = ((addr << 1) | 0x01);	// set slave address and direction bit (R)
	
	while((TWI0.MSTATUS & TWI_RIF_bm) == 0); // wait until address is transmitted
	
	// Check if slave NACKed the address
	if (TWI0.MSTATUS & TWI_RXACK_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;  // Send STOP condition
		exit(-1);  // Error: Slave did not acknowledge
	}
	
	uint16_t data_rdy = SCD41_read_word(); // Read word;
	uint8_t CRC = TWI0.MDATA;		// Read CRC	
	
	TWI0.MCTRLB = 0x07; // Issue nack and send STOP condition
	
	return (data_rdy << 5);	// We only care about least significant 11 bits
}

