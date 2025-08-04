/**
 * @file LCD.h
 * @date 4/3/2025 1:37:33 PM
 * @author Farhaan Khan
 * @brief Header file for SerLCD display.
 *
 * Description: This header file contains function declarations to 
 * write data to the SparkFun 20 x 4 SerLCD display via SPI using the 
 * SPI0 module of the AVR128DB48. It also contains char arrays that
 * are used as display buffers.
 */ 

#ifndef LCD_H_
#define LCD_H_

/** Display buffers */
char dsp_buff1[21];
char dsp_buff2[21];
char dsp_buff3[21];
char dsp_buff4[21];

// Function Declarations
void SPI_serLCD_init(void);
void write_spi0_SerLCD(unsigned char data); //!< write char to LCD
void clear_display_buffers(void); //!< clear buffers
void update_SerLCD(void); //!< Update LCD


#endif /* LCD_H_ */