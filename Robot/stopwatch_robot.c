//Example ATmega2560 Project
//File: ATmega2560Project.c
//Author: Robert Howie
//Created: 26 February 2016
//An example file for second year mechatronics project

//include this .c file's header file
#include "Robot.h"
#include "../adc.h"
#include "avr/interrupt.h"

// definitions

// timer specific
#define T1_PRESCALE 8
#define T1_COMPARE 1999
#define T1_TOP 65535

// clock frequency in hertz
#define CLOCK_FREQ 16000000

// global variables
int m_sec = 0;
int sec = 0;
int min = 0;

static char lcd_string[33] = {0};

int main(void){
	
	// initialise the LCD
	lcd_init();
	_delay_ms(20);
	
	// using timer1 as it is a 16bit timer, it is controlled using TCCR1A and
	// TCCR1B. The associated interrupt is controlled using TIMSK1

	// disable output and set both (Timing Counter Control Registers) to 0
	TCCR1A = 0;
	TCCR1B = 0;

	// setting the prescalar to 8
	TCCR1B |= (1<<CS11);

	// clear timer on compare (CTC mode)
	TCCR1B |= (1<<WGM12);

	// enable the timers compare A interrupt (Output Compare Interrupt Enable 1 A)
	TIMSK1 |= (OCIE1A);

	// set the comparison value in  (Output Compare Register 1 A)
	OCR1A = T1_COMPARE;

	// set interrupts to on
	sei();

	while(1){
		if (m_sec == 1000){
			sec++;
			m_sec - 1000;
		}
		if (sec == 60){
			min++;
			sec = sec - 60;
		}
		
		// create the formatted string with the reading
		sprintf(lcd_string, "%3d:%2d:%2d ", min, sec, m_sec);
		
		// print the string on the LCD
		lcd_puts(lcd_string);
	}
	return(1);
}

// execute this ISR when the value in timer 0 is equal to the value in the
// compare register A
ISR(TIMER0_COMPA_vect){
	m_sec++;
}