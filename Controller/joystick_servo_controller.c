//file scope variables




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

//declare and initialise string for serial printing
static char serial_string[200] = {0};
	
// main
int main(void){
	
	DDRB = 0xFF;
	
	// initialise the serial monitor
	serial0_init();
	
	//initialse ADC
	adc_init();
	
	// create variable to store in ADC reading
	uint16_t fixed_rotation_input = 0;
	uint16_t continuous_rotation_input = 0;
	
	//initialise timer3 to track milliseconds
	milliseconds_init();
	_delay_ms(20);

	// disable output and set both (Timing Counter Control Registers) to 0
	TCCR1A = 0;
	TCCR1B = 0;

	// setting the prescalar to 8
	TCCR1B |= (1<<CS11);

	// phase correct 8 bit PWM
	TCCR1B |= (1<<WGM13);
	
	// set to clear on up mode, which will make the PWM mode low on comparison
	TCCR1A |= (1<<COM1A1);
	
	TCCR1A |= (1<<COM1B1);
	
	ICR1 = 20000;
	
	int fixed_pwm_duty = 0;
	int continuous_pwm_duty = 0;

	// set interrupts to on
	sei();
	
	while(1){
		
		fixed_rotation_input = adc_read(0);
		
		continuous_rotation_input = adc_read(1);
		
		fixed_pwm_duty = fixed_rotation_input + 1000;
		
		continuous_pwm_duty = continuous_rotation_input + 1000;
		
		OCR1A = fixed_pwm_duty;
		
		OCR1B = continuous_pwm_duty;
		
		sprintf(serial_string, "%d \n", continuous_pwm_duty);
		
		serial0_print_string(serial_string);
	}
	return(1);
}