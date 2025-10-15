#include "controller.h"

// Constructor -----------------------------------------------------------------
Controller::Controller(RotationRates maxRotationRates, Angles maxAngles) {
	switch(rxPin) {
		case 0:
		case 1:
		case 12:
		case 13:
		case 17:
		case 16:
			_uart = uart0;
			break;
		case 4:
		case 5:
		case 8:
		case 9:
			_uart = uart1;
			break;
		default:
			throw "Non-valid controller rx pin.\n";
	}

	_maxRotationRates = maxRotationRates;
	_maxAngles = maxAngles;
	// Init UART
	uart_init(_uart, _BAUD_RATE);
	gpio_set_function(rxPin, GPIO_FUNC_UART);
};

// Public ----------------------------------------------------------------------
StickValues Controller::getStickValues() {
	if (!uart_is_readable(_uart)) return _prevStickValues;

	// Check for start
	uint8_t startByte = uart_getc(_uart);
	if (startByte != 0x20) return _prevStickValues;

	// Load buffer
	uint8_t buffer[31];
	for (int i = 0; i < 31; i++) {
		while (!uart_is_readable(_uart));
		buffer[i] = uart_getc(_uart);
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

	// Convert channels to StickValues
		// Mode 2
			// Throttle - Left stick up/down
			// Yaw - Left stick left/right
			// Pitch - Right stick up/down
			// Roll - Right stick left/right
	StickValues newStickValues;
	double rawRoll = (int(channels[0]) - 1500) / 500.0;
	double rawPitch = (int(channels[1]) - 1500) / 500.0;
	newStickValues.throttle = (channels[2] > 1800) ? 1800 : channels[2]; // Limit to 1800
	double rawYaw = (int(channels[3]) - 1500) / 500.0;
	newStickValues.leftPot = channels[4] - 1000;
	newStickValues.rightPot = channels[5] - 1000;
	switch (_mode) {
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