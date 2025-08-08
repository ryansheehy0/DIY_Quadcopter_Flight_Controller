#include "imu.h"
#include <cmath>

// Constructor -----------------------------------------------------------------

IMU::IMU() {
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

// Private ---------------------------------------------------------------------
RotationRates IMU::_gyro() {
	// Get gyro data
	// Convert to proper units
}

Gravity IMU::_acc() {
	// Get acc data
	// Convert to proper units
}

// Public ----------------------------------------------------------------------
Angles IMU::getAngles() {
	Gravity curGravity = _acc();

	// Subtract offset
	curGravity.x -= _accOffset.x;
	curGravity.y -= _accOffset.y;
	curGravity.z -= _accOffset.z;

	// Convert to pitch and roll
	Angles angles;
	angles.pitch = _toDegrees(atan(-curGravity.x / hypot(curGravity.y, curGravity.z)));
	angles.roll = _toDegrees(atan(curGravity.y / hypot(curGravity.x, curGravity.z)));

	// Combine gyro data to prevent vibrations

	return angles;
}

RotationRates IMU::getRotationRate() {
	RotationRates curRotationRate = _gyro();

	// Subtract offset
	curRotationRate.pitch -= _gyroOffset.pitch;
	curRotationRate.roll -= _gyroOffset.roll;
	curRotationRate.yaw -= _gyroOffset.yaw;

	return curRotationRate;
}