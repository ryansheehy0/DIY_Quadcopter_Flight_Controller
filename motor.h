#pragma once

#include "pico/stdlib.h"
#include "hardware/pwm.h"

/* How Pico PWM works:

ESCs typically expect 50Hz(20ms) PWM period with pulse widths of 1000 to 2000µs(5 to 10% duty cycle).

Pico PWM is controlled with three main variables:
	Clock divider: Divides the clock frequency in order to extend the period.
	Wrap: The counter counts from 0 up to wrap then resets. The period is defined as one loop.
		The counter increments every clock divider / clock freq.
	Level: sets the number of counts the output stays high (controls duty cycle).

Key formulas:
	Duty cycle(%) = level / (wrap + 1)
	PWM period = ((wrap + 1) * divider) / clock freq
*/

class Motor {
	private:
		// Each slice controls two pins. A pin is tied to a specific channel.
		uint _slice;
		uint _channel;

		// Constants
		static constexpr uint32_t _CLOCK_FEQ = 125'000'000; // Hz
		static constexpr double _PWM_PERIOD = 0.02; // Sec
		static constexpr uint16_t _WRAP = 0xFFFF; // 2^16(Max wrap) - Gives the most resolution.
		static constexpr double _CLOCK_DIV = (_PWM_PERIOD * _CLOCK_FEQ) / double(_WRAP + 1);

	public:
		Motor(uint pwmPin);

		void setOutput(uint16_t value);
		void enable() const { pwm_set_enabled(_slice, true); }
		void disable() const { pwm_set_enabled(_slice, false); }
};