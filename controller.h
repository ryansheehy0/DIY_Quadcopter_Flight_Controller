#pragma once

#include "hardware/uart.h"
#include "IMU/imu.h"

struct StickValues {
	uint16_t throttle = 1500; // 1000 to 2000(PWM microseconds)
	// Acro(Degrees per second) or Stabilized(Degrees)
	double pitch = 0;
	double roll = 0;
	double yaw = 0;
	uint16_t leftPot = 0; // 0 to 1000
	uint16_t rightPot = 0; // 0 to 1000
};

enum class ControllerMode { ACRO, STABILIZED };

class Controller {
	private:
		ControllerMode _MODE = ControllerMode::ACRO;

		// UART consts
		static constexpr uint _RX_PIN = 1;
		static constexpr uint _BAUD_RATE = 115'200;
		static constexpr uart_inst_t* _UART = uart0;

		StickValues _prevStickValues;
		RotationRates _maxRotationRates;
		Angles _maxAngles;

		uint16_t _combineBytes(uint8_t lsb, uint8_t msb) const { return (uint16_t(msb) << 8) | lsb; }

	public:
		Controller(
			RotationRates maxRotationRates = {.pitch = 75, .roll = 75, .yaw = 75},
			Angles maxAngles = {.pitch = 35, .roll = 35}
		);

		StickValues getStickValues();
};