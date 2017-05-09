#include "Controller.h"
#include "../adc.h"
#include "../serial.h"

// Declare and initialise string for serial printing
static char serial_string[200] = {0};
static char lcd_string[33] = {0}; // Declare and initialise string for LCD
static uint32_t current_ms = 0; // Current time
static uint32_t last_send_ms = 0; // Last time a msg was sent
static uint8_t serial_fsm_state = 0; // State of fsm
static uint8_t serial_byte_in = 0; // Serial byte in

int main(void)
{
	// Variable declarations

	// ADC readings
	uint16_t r_joy_vert_in = 0;
	uint16_t l_joy_vert_in = 0;

	// Initialisation section
	serial0_init(); // Initialise serial0 subsystem for debugging (USB)
	serial2_init(); // Initialise serial2 subsystem for comms (XBee)
	adc_init(); // Initialise ADC
	lcd_init(); // Initialise
	milliseconds_init(); // Initialise timer one to track milliseconds
	_delay_ms(10);

    FiveByteMsg msg_out = {0}, msg_in = {0}, temp_msg_in = {0};
    LcdMsg lcd_out = {0}

	while(1)
	{
        // Get the current time in ms
		current_ms = milliseconds;

		// Get ADC readings
		r_joy_vert_in = adc_read(0); // Right joystick vertical axis
		l_joy_vert_in = adc_read(15); // Left joystick vertical axis

        // Construct msg
        construct_msg(&msg_out, r_joy_vert_in, l_joy_vert_in);

        // Only send after at least 100ms
		if(current_ms - last_send_ms >= 100)
		{
            // Send message
            send_to_robot(&msg_out);
		}

        // Read serial
        if(UCSR2A & (1<<RXC2)) // Checks for new serial byte
        {
            read_serial(&temp_msg_in, &msg_in);
        }

        // Process message
        proc_msg(&msg_in);

		// Write to the LCD
		write_lcd(&msg_in);
	}
	return(1);
}

void construct_msg(FiveByteMsg* msg_out,
    uint16_t r_joy_vert_in, uint16_t l_joy_vert_in)
{
    msg_out->byte1 = convert_to_8_bit(r_joy_vert_in);
    msg_out->byte2 = convert_to_8_bit(l_joy_vert_in);
}

void send_to_robot(FiveByteMsg* msg_out)
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

	// sprintf(serial_string, "%u \n",serial_byte_in);
	// serial0_print_string(serial_string);

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
		// sprintf(serial_string, "DFUCK \n");
		// serial0_print_string(serial_string);
		serial_fsm_state = 1; // Wait for first parameter
	}
}

// Convert to an 8 bit number (adc_read is 10 bit)
uint8_t convert_to_8_bit(uint16_t adc_num)
{
    uint8_t converted_out;
    converted_out = ((adc_num * 253.0) / 1023);
    if converted_out > 253
    {
        converted_out = 253;
    }
    return converted_out;
}

int revert_to_sensor_val(uint8_t converted_num)
{
    int value;
    value = ((converted_num * 1023) / 253.0);
    return value;
}

void proc_msg(FiveByteMsg* msg_in, LcdMsg* lcd_msg)
{
    lcd_msg->byte1 = revert_to_sensor_val(msg_in->byte1);
    lcd_msg->byte2 = revert_to_sensor_val(msg_in->byte2);
    lcd_msg->byte3 = revert_to_sensor_val(msg_in->byte3);
}

void write_lcd(FiveByteMsg* msg_in)
{
    // Home the cursor
    lcd_home();

    // Create string
    sprintf(lcd_string, "L_S:%3u R_S:%3u", msg_in->byte1, msg_in->byte2);

	// Send string
	lcd_puts(lcd_string);

	// Create String
	sprintf(lcd_string, "Long_S:%3u", msg_in->byte3);

	// Set cursor to line 2 and send string
	lcd_goto(LINE_2);

    // print the string on the LCD
    lcd_puts(lcd_string);
}
