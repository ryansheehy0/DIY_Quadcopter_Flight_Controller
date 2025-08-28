#include "controller.h"

// Constructor -----------------------------------------------------------------
Controller::Controller(RotationRates maxRotationRates, Angles maxAngles) {
	_maxRotationRates = maxRotationRates;
	_maxAngles = maxAngles;
	// Init UART
	uart_init(_UART, _BAUD_RATE);
	gpio_set_function(_RX_PIN, GPIO_FUNC_UART);
};


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
	double rawRoll = (int(channels[0]) - 1500) / 500.0; // Right stick left/right
	double rawPitch = (int(channels[1]) - 1500) / 500.0; // Right stick up/down
	// Limit throttle to prevent saturation of motors
	newStickValues.throttle = (channels[2] > 1800) ? 1800 : channels[2]; // Left stick up/down
	double rawYaw = (int(channels[3]) - 1500) / 500.0; // Left stick left/down
	newStickValues.leftPot = channels[4] - 1000;
	newStickValues.rightPot = channels[5] - 1000;
	switch (_MODE) {
		case ControllerMode::ACRO:
			newStickValues.pitch = rawPitch * _maxRotationRates.pitch;
			newStickValues.roll = rawRoll * _maxRotationRates.roll;
			newStickValues.yaw = rawYaw * _maxRotationRates.yaw;
			break;
		case ControllerMode::STABILIZED:
			newStickValues.pitch = rawPitch * _maxAngles.pitch;
			newStickValues.roll = rawRoll * _maxAngles.roll;
			newStickValues.yaw = rawYaw * _maxRotationRates.yaw;
			break;
	}

	// Return
	_prevStickValues = newStickValues;
	return newStickValues;
}