//Example ATmega2560 Project
//File: ATmega2560Project.c
//Author: Robert Howie
//Created: March 2015
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"
#include "../adc.h"

//static function prototypes, functions only called in this file


//file scope variables
static char serial_string[200] = {0}; //declare and initialise string for serial printing
static char lcd_string[33] = {0}; //declare and initialise string for LCD

int main(void)
{
	//variable declarations
	
	//initialisation section, runs once
	serial0_init(); //initialise serial subsystem
	adc_init(); //initialse ADC
	lcd_init(); //initialise 
	milliseconds_init(); //initialise timer3 to track milliseconds
	_delay_ms(20);
	
	// create a variable so that we have a place to store the ADC reading
	uint16_t sensor_input = 0;
		
	//main loop
	while(1)
	{
		// read sensor input from ADC0
		sensor_input = adc_read(0);
		
		// home the cursor on the LCD
		lcd_home();
		
		// create the formatted string with the reading
		sprintf(lcd_string, "ADC Reading: %3u", sensor_input);
		
		// print the string on the LCD
		lcd_puts(lcd_string);
		
		// delay so that we have time to read the LCD output
		_delay_ms(2000);	
	}
	return(1);
} //end main
