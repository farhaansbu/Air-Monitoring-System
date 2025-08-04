/**
 * @file display_sensor_data.c
 * @date 4/23/2025 2:50:32 PM
 * @author Farhaan Khan
 * @brief Reads sensor data and displays it on LCD display.
 *
 * Description: This is the main file for a program that 
 * reads the temperature, CO2 levels, and relative humidity
 * from the SCD41 sensor, and then displays the data on
 * the SparkFun 20 x 4 SerLCD. Communication is done
 * using I2C & SPi via the TWI0 and SPI0 modules of the AVR128DB48.
 */ 

#define F_CPU 4000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCD.h"
#include "SCD41.h"	

int main(void)
{
	// Initialize TWI0 & SPI
	
    TWI0_SCD41_init();
	SPI_serLCD_init();
	
	
	// Restart periodic measurement
	SCD41_send_command(STOP_PERIODIC_MEASUREMENT, SEND_COMMAND_SEQUENCE);
	SCD41_send_command(START_PERIODIC_MEASUREMENT, SEND_COMMAND_SEQUENCE);
	
	// Clear display
	PORTA.OUT &= ~PIN7_bm;	// Select Slave
	write_spi0_SerLCD('|');
	write_spi0_SerLCD('-');
	PORTA.OUT |= PIN7_bm;	// Disable Slave
	
	
	// Clear display buffers
	clear_display_buffers();
	
	sprintf(dsp_buff1, "Air Quality Data:  ");
	
    while (1) 
    {
		// Wait until data is ready
		while (SCD41_data_ready() == 0x0000);
		
		// read sensor data
		SCD41_read_measurement();
		
		// Print data to buffers
		sprintf(dsp_buff2, "CO2 Levels: %uppm", co2_level_ppm);
		sprintf(dsp_buff3, "Temperature: %.1fC", temperature_celcius);
		sprintf(dsp_buff4, "Rel. Humidity: %.0f%%", relative_humidity);
		
		 // Display everything on LCD
		 update_SerLCD();
    }
	
	// Stop periodic measurement
	SCD41_send_command(STOP_PERIODIC_MEASUREMENT, SEND_COMMAND_SEQUENCE);
	_delay_ms(500);	// Wait for command to execute
	
}
