/**
 * @file LCD.C
 * @date 4/23/2025 3:18:36 PM
 * @author Farhaan Khan
 * @brief Source file containing function definitions for SerLCD display.
 *
 * Description: This source file contains the definitions of the 
 * functions declared in the LCD.h header file. These functions can 
 * be used to write data to the SparkFun 20 x 4 SerLCD display using 
 * the SPI0 module of the AVR128DB48.
 */
  

#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

// LCD Functions

void SPI_serLCD_init(void)
{
	//Configure location of SPI Pins (PA4-PA7)
	PORTMUX.SPIROUTEA &= ~(PORTMUX_SPI00_bm | PORTMUX_SPI01_bm);
	
	// Configure /SS, MOSI, and SCK as output, MISO is input
	PORTA.DIR |= 0x80 | 0x40 | 0x10;
		
	// Deselect LCD
	PORTA.OUT |= 0x80;

	// Enable Master Mode
	SPI0.CTRLA |= SPI_MASTER_bm;
	
	// Configure Clock Phase and Polarity
	SPI0.CTRLB |= SPI_MODE_1_gc;
	
	// Enable SPI
	SPI0.CTRLA |= SPI_ENABLE_bm;
	
}

void clear_display_buffers(void) // helper function
{
	// Write a space char to every buffer location
	for (int i = 0; i < 20; ++i)
	{
		dsp_buff1[i] = ' ';
		dsp_buff2[i] = ' ';
		dsp_buff3[i] = ' ';
		dsp_buff4[i] = ' ';
	}
}

//LCD Functions
void update_SerLCD(void)
{
	// Write each char in buffers to data register one by one
	PORTA.OUT &= ~PIN7_bm;	// Select Slave
	for (int i = 0; i < 80; ++i)
	{
		// Write data from appropriate buffer
		if (i < 20)
		write_spi0_SerLCD(dsp_buff1[i]);
		else if (i < 40)
		write_spi0_SerLCD(dsp_buff2[i % 20]);
		else if (i < 60)
		write_spi0_SerLCD(dsp_buff3[i % 20]);
		else
		write_spi0_SerLCD(dsp_buff4[i % 20]);
	}
	PORTA.OUT |= PIN7_bm;	// Disable Slave
	
}

void write_spi0_SerLCD(unsigned char data)
{
	// Write data
	SPI0.DATA = data;
	
	// wait for transfer to complete
	while (!(SPI0.INTFLAGS & SPI_IF_bm));
}