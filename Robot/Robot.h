//Example ATmega2560 Project
//File: ATmega2560Project.h
//Author: Robert Howie

#ifndef ROBOT_H_ //double inclusion guard
#define ROBOT_H_

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

//constants
#define BIG_NUMBER 120000
#define SERIAL_PRINT_MESSAGE "Hello student!\n"
#define BUILD_DATE __TIME__ " " __DATE__"\n"
#endif /* ATMEGA2560_H_ */

// My constants
#ifndef BYTE_CONSTANTS
#define START_BYTE 255
#define STOP_BYTE 254
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

#ifndef COMPAREMSG
#define COMPAREMSG
typedef struct CompareMsg
{
	uint16_t byte1;
	uint16_t byte2;
	uint16_t byte3;
	uint16_t byte4;
	uint16_t byte5;
} CompareMsg;
#endif

// Function Prototypes
void construct_msg(FiveByteMsg*, uint16_t, uint16_t, uint16_t);
void send_to_controller(); // Send data to controller over serial
void read_serial(FiveByteMsg*, FiveByteMsg*); // Read data from serial
int convert_to_compare_val(uint16_t);
void proc_msg(FiveByteMsg*, CompareMsg*);
void motors_init();
uint8_t convert_to_8_bit(uint16_t);