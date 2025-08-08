#pragma once

struct Angles { // In degrees
	double pitch = 0;
	double roll = 0;
};

struct RotationRates { // In degrees per second
	double pitch = 0;
	double roll = 0;
	double yaw = 0;
};

struct Gravity { // In m/s^2
	double x = 0;
	double y = 0;
	double z = 0;
};

// gyro - gyroscope
// acc - accelerometer

class IMU {
	private:
		// Offsets subtracted from sensor data.
		RotationRates _gyroOffset;
		Gravity _accOffset;

		const double _PI = 3.141592653589793;
		double _toRadians(double degrees) { return degrees * (_PI / 180.0); }
		double _toDegrees(double radians) { return radians * (180.0 / _PI); }

		// Get sensor data
		RotationRates _gyro();
		Gravity _acc();

	public:
		IMU();

		Angles getAngles();
		RotationRates getRotationRate();
};