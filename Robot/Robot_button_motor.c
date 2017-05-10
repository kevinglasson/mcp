//Example ATmega2560 Project
//File: ATmega2560Project.c
//Author: Robert Howie
//Created: 26 February 2016
//An example file for second year mechatronics project

//include this .c file's header file
#include "Robot.h"
#include "../adc.h"
#include "avr/interrupt.h"

bool state = true;

ISR(INT0_vect)
{
	state = !state;
}
/*
int main(void)
{
	serial0_init();
	
	// put PORTC in output mode
	DDRC = 0xFF;
	
	// put PORTD in input mode
	DDRD = 0x00;
		
	// set PORTC pins to low
	PORTC = 0x00;
	
	// set the interrupt to trigger on falling edge, 10
	EICRA |= (1<<ISC01);
	EICRA &= ~(1<<ISC00);
	
	// enable the INT0 interrupt pin
	EIMSK |= (1<<INT0);
	
	// set interrupt on
	sei();
	

	
	while(1)
	{
		if (state)
		{
			PORTC |= (1<<0);
		}
		else
		{
			PORTC &= ~(1<<0);	
		}
	}
	return(1);
}//end main*/