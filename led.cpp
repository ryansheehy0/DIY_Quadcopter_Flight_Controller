#include "led.h"

LED::LED() {
	gpio_init(_LED_PIN);
	gpio_set_dir(_LED_PIN, true);
}

void LED::on() {
	gpio_put(_LED_PIN, 1);
}

void LED::off() {
	gpio_put(_LED_PIN, 0);
}

void LED::blink(unsigned int count, unsigned int msTime) {
	for (; count > 0; count--) {
		on();
		sleep_ms(msTime);
		off();
		sleep_ms(msTime);
	}
}

void LED::display64Bits(bitset<64> bits) {
	for(size_t i = 0; i < 64; i++) {
		if (bits.test(i)) { // 1
			blink(2);
		} else { // 0
			blink(1);
		}
		sleep_ms(1000);
	}
}