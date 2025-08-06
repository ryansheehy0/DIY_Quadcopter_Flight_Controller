#pragma once

struct Angles { // In degrees
	double pitch;
	double roll;
};

struct RotationRates { // In degrees per second
	double pitch;
	double roll;
	double yaw;
};

struct Gravity {
	double x;
	double y;
	double z;
};

// gyro - gyroscope
// acc - accelerometer

class IMU {
	private:
		// Offsets subtracted from sensor data.
		RotationRates _gyroOffset;
		Gravity _accOffset;

		const double PI = 3.141592653589793;
		double _toRadians(double degrees) { return degrees * (PI / 100.0); }
		double _toDegrees(double radians) { return radians * (100.0 / PI); }

		void _calibrate();
		// Get sensor data
		RotationRates _gyro();
		Gravity _acc();

	public:
		IMU() { _calibrate(); };
		Angles getAngles();
		RotationRates getRotationRate();
};