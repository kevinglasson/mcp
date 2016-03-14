//Example ATmega2560 Project
//File: ATmega2560Project.c
//Author: Robert Howie
//Created: March 2015
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"

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
	
	//main loop
	while(1)
	{
		//loop code goes here		
	}
	return(1);
} //end main
