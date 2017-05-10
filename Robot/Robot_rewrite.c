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
        // Get the current time in ms
		current_ms = milliseconds;

		// Get ADC readings
        r_short_sens = adc_read(0);
        l_short_sens = adc_read(1);
        long_sens = adc_read(2);

        // Construct msg
        construct_msg(&msg_out, r_short_sens, l_short_sens, long_sens);

        // Only send after at least 100ms
        if(current_ms - last_send_ms >= 100)
        {
            // Send message
            send_to_controller(&msg_out);
        }

    }

void construct_msg(FiveByteMsg* msg_out,
    uint16_t r_short_sens, uint16_t l_short_sens, uint16_t long_sens)
{
    msg_out->byte1 = convert_to_8_bit(r_short_sens);
    msg_out->byte2 = convert_to_8_bit(l_short_sens);
    msg_out->byte3 = convert_to_8_bit(long_sens);
    msg_out->byte4 = 1;
    msg_out->byte5 = 1;
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
