#pragma once

#include <cstddef> // size_t
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
}

// gyro - gyroscope
// acc - accelerometer

class IMU {
	private:
		// I2C consts
		constexpr uint _SCL_PIN = 17;
		constexpr uint _SDA_PIN = 16;
		constexpr uint8_t _BMI160_ADDR = 0x68; // Connect BMI160's SAO to GND
		constexpr i2c_inst_t* _I2C = i2c0;
		constexpr uint _BAUD_RATE = 400'000;

		// Addresses
		constexpr uint8_t _CMD_ADDR = 0x7E;
		constexpr uint8_t _GYRO_NORMAL_MODE = 0x15;
		constexpr uint8_t _ACC_NORMAL_MODE = 0x11;
		constexpr uint8_t _GYRO_ADDR = 0x0C;
		constexpr uint8_t _ACC_ADDR = 0x12;

		// Offsets subtracted from sensor data.
		RotationRates _gyroOffset;
		Gravity _accOffset;

		Angles _gyroAngleEstimation;

		MovingAverage<11> _accAvgX;
		MovingAverage<11> _accAvgY;
		MovingAverage<11> _accAvgZ;

		constexpr double _PI = 3.141592653589793;
		double _toRadians(double degrees) { return degrees * (_PI / 180.0); }
		double _toDegrees(double radians) { return radians * (180.0 / _PI); }

		int16_t _combineBytes(uint8_t lsb, uint8_t msb) const { return (int16_t(msb) << 8) | lsb; }

		// Get sensor data
		RawData _getRawData(uint8_t reg) const;
		RotationRates _gyro();
		Gravity _acc();

	public:
		IMU();

		Angles getAngles(double deltaTime);
		RotationRates getRotationRates();
};