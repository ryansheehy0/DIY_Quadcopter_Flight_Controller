#pragma once
#include "hardware/uart.h"

struct StickValues {
	uint16_t throttle = 1500;
	uint16_t pitch = 1500;
	uint16_t roll = 1500;
	uint16_t yaw = 1500;
	uint16_t leftPot = 1000;
	uint16_t rightPot = 1000;
};

class Controller {
	private:
		constexpr uint _RX_PIN = 1;
		constexpr uint _BAUD_RATE = 115'200;
		constexpr uart_inst_t* _UART = uart0;

		StickValues _prevStickValues;

		uint16_t _combineBytes(uint8_t lsb, uint8_t msb) const { return ((uint16_t)msb << 8) | lsb; };

	public:
		Controller();
		StickValues getStickValues();
};