#pragma once

#include <bitset>
#include "pico/stdlib.h"

using std::bitset;

class LED {
	private:
		static constexpr uint _LED_PIN = 25;

	public:
		LED();

		void on();
		void off();
		void blink(unsigned int count, unsigned int msTime = 500);
		void display64Bits(bitset<64> bits); // 0 - 1 blink, 1 - 2 blinks
};