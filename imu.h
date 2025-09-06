#pragma once

#include <cstddef>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "moving_average.h"

struct Angles { // In degrees
	double pitch = 0;
	double roll = 0;
};

struct RotationRates { // In degrees/second
	double pitch = 0;
	double roll = 0;
	double yaw = 0;
};

struct Gravity {
	double x = 0;
	double y = 0;
	double z = 1;
};

struct RawData {
	int16_t data1;
	int16_t data2;
	int16_t data3;
};

// gyro - gyroscope
// acc - accelerometer

class IMU {
	private:
		// I2C consts
		static constexpr uint8_t _BMI160_ADDR = 0x68; // Connect BMI160's SAO to GND
		static constexpr i2c_inst_t* _I2C = i2c0;

		// Offsets subtracted from sensor data.
		RotationRates _gyroOffset;
		Gravity _accOffset;

		Angles _gyroAngleEstimation;

		static constexpr uint _AVERAGE_SAMPLE = 11;
		MovingAverage<_AVERAGE_SAMPLE> _accAvgX;
		MovingAverage<_AVERAGE_SAMPLE> _accAvgY;
		MovingAverage<_AVERAGE_SAMPLE> _accAvgZ;

		static constexpr double _PI = 3.141592653589793;
		double _toDegrees(double radians) { return radians * (180.0 / _PI); }

		int16_t _combineBytes(uint8_t lsb, uint8_t msb) const { return (int16_t(msb) << 8) | lsb; }

		// Get sensor data
		RawData _getRawData(uint8_t reg) const;
		RotationRates _gyro();
		Gravity _acc();

	public:
		IMU();

		RotationRates getRotationRates();
		Angles getAngles(double deltaTime);
};