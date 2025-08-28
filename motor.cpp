#include "motor.h"

// Constructor -----------------------------------------------------------------
Motor::Motor(uint pwmPin) {
	// Init PWM
	gpio_set_function(pwmPin, GPIO_FUNC_PWM);
	_slice = pwm_gpio_to_slice_num(pwmPin);
	_channel = pwm_gpio_to_channel(pwmPin);
	pwm_set_wrap(_slice, _WRAP);
	pwm_set_clkdiv(_slice, _CLOCK_DIV);
	setOutput(1000);
}

// Public ----------------------------------------------------------------------
void Motor::setOutput(uint16_t value) {
	if (value > 2000) value = 2000;
	if (value < 1000) value = 1000;

	// Map value to duty cycle percentage
		// 1000 = 0.05(5%)
		// 2000 = 0.1(10%)
	double dutyCycle = double(value - 1000) / 1000 * 0.05 + 0.05;

	// Set level
	uint16_t level = dutyCycle * (_WRAP + 1) + 0.5; // +0.5 rounds when there's truncation
	pwm_set_chan_level(_slice, _channel, level);
}