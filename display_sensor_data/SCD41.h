/**
 * @file SCD41.h
 * @date 4/23/2025 4:03:03 PM
 * @author Farhaan Khan
 * @brief Header file for SCD41 Sensor Functions.
 *
 * Description: This header file contains function declarations to 
 * read data from the Sensiron SCD41 sensor via I2C using the 
 * TWI0 module of the AVR128DB48. It also contains global variables
 * that store the data from the sensor.
 */

#ifndef SCD41_H_
#define SCD41_H_

// Command Sequence-Type macros
#define SEND_COMMAND_SEQUENCE 0x01
#define WRITE_SEQUENCE 0x02
#define READ_SEQUENCE 0x03
#define SEND_COMMAND_AND_FETCH_SEQUENCE 0x04

// Command macros (for clearer code)
#define START_PERIODIC_MEASUREMENT 0x21b1
#define STOP_PERIODIC_MEASUREMENT 0x3f86
#define READ_MEASUREMENT 0xec05
#define GET_DATA_READY_STATUS 0xe4b8 

// Initialize TWI0
void TWI0_SCD41_init(void); //!< Initialize TWI0 Module

// Helper Functions to read/write words and/or bytes
void SCD41_write_word(uint16_t data); //!< Writes a word to SCD41 (assumes address has been sent)
uint16_t SCD41_read_word(void); //!<Reads a word (assumes address has been sent)
uint16_t SCD41_read_word2(void);
uint8_t SCD41_read_byte(void); //!< Reads a byte (assumes address has been sent)

// SCD41 Command Sequences
void SCD41_send_command(uint16_t command, uint8_t command_type); //!< Sends a command-word to SCD41
void SCD41_read_measurement(void); //!< Issues read_measurement command and places data in global variables
uint16_t SCD41_data_ready(void); //!< Issues get_data_ready_status command and returns response


// Global variables to store sensor data
uint16_t co2_level_ppm;
float temperature_celcius;
float relative_humidity;


#endif /* SCD41_H_ */