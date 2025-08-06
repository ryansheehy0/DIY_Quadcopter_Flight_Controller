#include "imu.h"
#include <cmath>

// Private ---------------------------------------------------------------------
void IMU::_calibrate() {
	const int CALIBRATION_SAMPLES = 2000;

	// Calibrate gyroscope
	RotationRates avgRates;
	for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
		RotationRates gyroRates = _gyro();
		avgRates.pitch += gyroRates.pitch;
		avgRates.roll += gyroRates.roll;
		avgRates.yaw += gyroRates.yaw;
		// Timer daley?
	}
	_gyroOffset.pitch = avgRates.pitch / CALIBRATION_SAMPLES;
	_gyroOffset.roll = avgRates.roll / CALIBRATION_SAMPLES;
	_gyroOffset.yaw = avgRates.yaw / CALIBRATION_SAMPLES;

	// Calibrate accelerometer
	Gravity avgGravity;
	for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
		Gravity accData = _acc();
		avgGravity.x += accData.x;
		avgGravity.y += accData.y;
		avgGravity.z += accData.z;
		// Timer daley?
	}
	_accOffset.x = avgGravity.x / CALIBRATION_SAMPLES;
	_accOffset.y = avgGravity.y / CALIBRATION_SAMPLES;
	_accOffset.z = avgGravity.z / CALIBRATION_SAMPLES;
}

double IMU::_degreesToRadians(double degrees) {
	const double PI = 3.141592653589793;
	return 
	- `degrees = radians * (100.0 / PI)`
	- ``
}

// Public ----------------------------------------------------------------------
Angles IMU::getAngles() {
	Gravity curGravity = _acc();
	Angles angles;
	angles.pitch = 
}

/*
pitch = atan(\frac{-Acc_x}{\sqrt(Acc^{2}_{y} + Acc^{2}_{z})})$
	- $\theta_{roll} = atan(\frac{Acc_y}{\sqrt(Acc^{2}_{x} + Acc^{2}_{z})})$
*/

RotationRates IMU::getRotationRate() {

}