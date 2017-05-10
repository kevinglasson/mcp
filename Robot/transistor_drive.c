//include this .c file's header file
#include "Robot.h"
#include "../adc.h"

int main(void)
{
	//variable declarations
		
	//put PORTA into output mode
	DDRA = 0xFF;
	PORTA = 0x00;
		
	//main loop
	while(1)
	{
		PORTA |= (1<<0);
		_delay_ms(500);
		PORTA &= ~(1<<0);
		_delay_ms(500);
	}
	return(1);
}//end main