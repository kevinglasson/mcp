//Example ATmega2560 Project
//File: ATmega2560Project.h
//Author: Robert Howie

#ifndef CONTROLLER_H_ //double inclusion guard
#define CONTROLLER_H_

//include standard libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>

//include header files
#include "../serial.h" //minimal serial lib
#include "../adc.h" //minimal adc lib
#include "../milliseconds.h" //milliseconds timekeeping lib
#include "../hd44780.h" //LCD lib

// Constants
#define BUILD_DATE __TIME__ " " __DATE__"\n"
#endif /* ATMEGA2560_H_ */

// My constants
#ifndef BYTE_CONSTANTS
#define START_BYTE 255
#define STOP_BYTE 254
#endif

#ifndef LINE_2
#define LINE_2 0x40
#endif

// Structs
#ifndef FIVEBYTEMSG
#define FIVEBYTEMSG
typedef struct FiveByteMsg
{
    uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
} FiveByteMsg;
#endif

// Function Prototypes
void construct_msg(FiveByteMsg*, uint16_t, uint16_t);
void send_to_robot(FiveByteMsg*); // Send data to robot over serial
void read_serial(FiveByteMsg*, FiveByteMsg*); // Read data from serial
uint8_t convert_to_8_bit(uint16_t); // ADC value conversion for sending
void proc_msg(FiveByteMsg* msg_in); // Process msg for writing to LCD
void write_lcd(FiveByteMsg* msg_in); // Write msg to LCD
