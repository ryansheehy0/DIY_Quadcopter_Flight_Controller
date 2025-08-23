#include "controller.h"

// Constructor -----------------------------------------------------------------
Controller::Controller() {
	// Init UART
	uart_init(_UART, _BAUD_RATE);
	gpio_set_function(_RX_PIN, GPIO_FUNC_UART);
}

// Public ----------------------------------------------------------------------
StickValues Controller::getStickValues() {
	if (!uart_is_readable(_UART)) return _prevStickValues;

	// Check for start
	uint8_t startByte = uart_getc(_UART);
	if (startByte != 0x20) return _prevStickValues;

	// Load buffer
	uint8_t buffer[31];
	for (int i = 0; i < 31; i++) {
		while (!uart_is_readable(_UART));
		buffer[i] = uart_getc(_UART);
	}

	// Validate checksum
	uint16_t checksum = 0xFFFF - 0x20;
	for (int i = 0; i < 29; i++) {
		checksum -= buffer[i];
	}
	uint16_t lastTwoBytes = _combineBytes(buffer[29], buffer[30]);
	if (checksum != lastTwoBytes) return _prevStickValues;

	// Get data from buffer
	uint16_t channels[6];
	for (uint i = 0; i < 6; i++) {
		channels[i] =  _combineBytes(buffer[(i*2) + 1], buffer[(i*2) + 2]);
	}

	// Convert channels to StickValues. Mode 2
	StickValues newStickValues;
	newStickValues.roll = channels[0];
	newStickValues.pitch = channels[1];
	newStickValues.throttle = channels[2];
	newStickValues.yaw = channels[3];
	newStickValues.leftPot = channels[4];
	newStickValues.rightPot = channels[5];

	// Return
	_prevStickValues = newStickValues;
	return newStickValues;
}