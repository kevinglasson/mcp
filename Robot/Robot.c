#include "Robot.h"
#include "../adc.h"
#include "avr/interrupt.h"

// Declare and initialise string for debugging
static char serial_string[200] = {0};
static uint32_t current_ms = 0; // Current time
static uint32_t last_send_ms = 0; // Last time a msg was sent
static uint8_t serial_fsm_state = 0; // State of fsm
static uint8_t serial_byte_in = 0; // Serial byte in

int main(void)
{
    // Variable declarations

	// ADC readings
	uint16_t r_short_sens = 0;
	uint16_t l_short_sens = 0;
	uint16_t long_sens = 0;

    // Initialisation section
	serial0_init(); // Initialise serial0 subsystem for debugging (USB)
	serial2_init(); // Initialise serial2 subsystem for comms (XBee)
	adc_init(); // Initialise ADC
    motors_init();
	milliseconds_init(); // Initialise timer one to track milliseconds
	_delay_ms(10);

    FiveByteMsg msg_out = {0}, msg_in = {0}, temp_msg_in = {0};
	CompareMsg comp_msg = {0};

	while(1)
	{
		current_ms = milliseconds;

        // Get ADC readings
        r_short_sens = adc_read(0); // Right joystick vertical axis
        l_short_sens = adc_read(1); // Left joystick vertical axis
        long_sens = adc_read(2); // Left joystick vertical axis

        // Construct msg
        construct_msg(&msg_out, r_short_sens, l_short_sens, long_sens);

        // Only send after at least 100ms
        if(current_ms - last_send_ms >= 1000)
        {
            // Send message
            send_to_controller(&msg_out);
        }

        // Read serial
        if(UCSR2A & (1<<RXC2))// Checks for new serial byte
        {
            read_serial(&temp_msg_in, &msg_in);
        }

        // Process message
        proc_msg(&msg_in, &comp_msg);

		// Set the compare registers with the processed message
		OCR1A = comp_msg.byte1; // Digital pin 11
		OCR1B = comp_msg.byte2; // Digital pin 12
	}
	return(1);
}

void construct_msg(FiveByteMsg* msg_out,
    uint16_t r_short_sens, uint16_t l_short_sens, uint16_t long_sens)
{
    msg_out->byte1 = convert_to_8_bit(r_short_sens);
    msg_out->byte2 = convert_to_8_bit(l_short_sens);
    msg_out->byte3 = convert_to_8_bit(long_sens);
}

void send_to_controller(FiveByteMsg* msg_out)
{
    last_send_ms = current_ms;
    serial2_write_byte(START_BYTE); //send start byte
    serial2_write_byte(msg_out->byte1); // Send first parameter
    serial2_write_byte(msg_out->byte2); // Send second parameter
    serial2_write_byte(msg_out->byte3); // Send third parameter
    serial2_write_byte(msg_out->byte4); // Send fourth parameter
    serial2_write_byte(msg_out->byte5); // Send fifth parameter
    serial2_write_byte(STOP_BYTE); // Send stop byte
}

void read_serial(FiveByteMsg* temp_msg_in, FiveByteMsg* msg_in)
{
    serial_byte_in = UDR2; // Store serial byte
    switch(serial_fsm_state) // Switch on the current state
    {
        // Continue waiting for START_BYTE
        case 0:
        break;

        case 1: // Waiting for first parameter
        temp_msg_in->byte1 = serial_byte_in;
        serial_fsm_state++;
        break;

        case 2: // Waiting for second parameter
        temp_msg_in->byte2 = serial_byte_in;
        serial_fsm_state++;
        break;

        case 3: // Waiting for third parameter
        temp_msg_in->byte3 = serial_byte_in;
        serial_fsm_state++;
        break;

        case 4: // Waiting for fourth parameter
        temp_msg_in->byte4 = serial_byte_in;
        serial_fsm_state++;
        break;

        case 5: // Waiting for fifth parameter
        temp_msg_in->byte5 = serial_byte_in;
        serial_fsm_state++;
        break;

        case 6: // Waiting for stop byte
        if(serial_byte_in == STOP_BYTE)
        {
            msg_in->byte1 = temp_msg_in->byte1;
            msg_in->byte2 = temp_msg_in->byte2;
            msg_in->byte3 = temp_msg_in->byte3;
            msg_in->byte4 = temp_msg_in->byte4;
            msg_in->byte5 = temp_msg_in->byte5;

            // For debugging
            //sprintf(serial_string, "Read 1:%d, Read 2:%d \n", msg_in->byte1, msg_in->byte2);
            //serial0_print_string(serial_string);
        }
        serial_fsm_state = 0; // Wait for START_BYTE
        break;
    }
    if(serial_byte_in == START_BYTE)
    {
        serial_fsm_state = 1; // Wait for first parameter
    }
}

// Convert 8 bit number from serial port to comapare registed value
int convert_to_compare_val(uint16_t converted_num)
{
    int compare_val;
    compare_val = ((converted_num * 1023.0) / 253) + 1000;
    return compare_val;
}

uint8_t convert_to_8_bit(uint16_t adc_num)
{
    uint8_t converted_out;
	if (adc_num != 0)
	{
		converted_out = ((adc_num * 253.0) / 1023);
	}
	else
	{
		converted_out = 0;
	}
    return converted_out;
}

void proc_msg(FiveByteMsg* msg_in, CompareMsg* comp_msg)
{
    //TODO: Convert the rest of the bytes when we know what they are
    comp_msg->byte1 = convert_to_compare_val(msg_in->byte1);
    comp_msg->byte2 = convert_to_compare_val(msg_in->byte2);
}

// Initialise registers to control the motors
void motors_init()
{
    // Initialise DDRB as output, this turns on the PWM output
    DDRB = 0xFF;

    // Disable output and set both (Timing Counter Control Registers) to 0
    TCCR1A = 0;
    TCCR1B = 0;

    // Setting the PRESCALAR to 8
    TCCR1B |= (1<<CS11);

    // Phase correct 8 bit PWM
    TCCR1B |= (1<<WGM13);

    // Set to clear on up mode, which will make the PWM mode low on comparison
    TCCR1A |= (1<<COM1A1);
    TCCR1A |= (1<<COM1B1);

    // Set TOP
    ICR1 = 20000;

    // Set interrupts to on
    sei();
}
