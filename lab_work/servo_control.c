//Example ATmega2560 Project
//File: ATmega2560Project.c
//Author: Robert Howie
//Created: 26 February 2016
//An example file for second year mechatronics project

//include this .c file's header file
#include "Controller.h"
#include "../serial.h"

// definitions

// duty cycle

// global variables


bool start_state = false;
bool reset_state = false;

static char serial_string[200] = {0};
static char lcd_string[33] = {0};

int main(void){
	
	// initialise the LCD
	serial0_init();
	lcd_init();
	_delay_ms(20);
	
	// using timer1 as it is a 16bit timer, it is controlled using TCCR1A and
	// TCCR1B. The associated interrupt is controlled using TIMSK1

	// disable output and set both (Timing Counter Control Registers) to 0
	TCCR1A = 0;
	TCCR1B = 0;

	// setting the prescalar to 8
	TCCR1B |= (1<<CS11);

	// phase correct 8 bit PWM
	TCCR1B |= (1<<WGM10);
	
	// set to clear on up mode, which will make the PWM mode low on comparison
	TCCR1A |= (1<<COM1A1);

	// set the comparison value in  (Output Compare Register 1 A)
	OCR1A = T1_COMPARE;

	// set interrupts to on
	sei();
	
	while(1){
		
	}
	return(1);
}

// execute this ISR when the value in timer 0 is equal to the value in the
// compare register A
ISR(TIMER1_COMPA_vect){
	if (start_state)
	{
		m_sec++;
	}
}

// execute this when the start/stop button is pressed
ISR(INT0_vect){
	start_state = !start_state;
}

// execute when the reset button is pressed
ISR(INT1_vect){
	reset_state = !reset_state;
}