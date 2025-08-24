#include "imu.h"
#include <cmath> // atan, hypot

// Constructor -----------------------------------------------------------------
IMU::IMU() {
	// Init I2C
	i2c_init(_I2C, _BAUD_RATE);
	gpio_set_function(_SCL_PIN, GPIO_FUNC_I2C);
	gpio_set_function(_SDA_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(_SCL_PIN);
	gpio_pull_up(_SDA_PIN);

	// Set Gyro to normal mode
	uint8_t gyroCmd[2] = {_CMD_ADDR, _GYRO_NORMAL_MODE};
	i2c_write_blocking(_I2C, _BMI160_ADDR, gyroCmd, 2, false);
	sleep_ms(500);

	// Set Acc to normal mode
	uint8_t accCmd[2] = {_CMD_ADDR, _ACC_NORMAL_MODE};
	i2c_write_blocking(_I2C, _BMI160_ADDR, accCmd, 2, false);
	sleep_ms(500);

	// Calibrate sensors
	constexpr int _CALIBRATION_SAMPLES = 2000;

	// Calibrate gyroscope
	RotationRates avgRates;
	for (int i = 0; i < _CALIBRATION_SAMPLES; i++) {
		RotationRates gyroRates = _gyro();
		avgRates.pitch += gyroRates.pitch;
		avgRates.roll += gyroRates.roll;
		avgRates.yaw += gyroRates.yaw;
	}
	_gyroOffset.pitch = avgRates.pitch / _CALIBRATION_SAMPLES;
	_gyroOffset.roll = avgRates.roll / _CALIBRATION_SAMPLES;
	_gyroOffset.yaw = avgRates.yaw / _CALIBRATION_SAMPLES;

	// Calibrate accelerometer
	Gravity avgGravity;
	for (int i = 0; i < _CALIBRATION_SAMPLES; i++) {
		Gravity accData = _acc();
		avgGravity.x += accData.x;
		avgGravity.y += accData.y;
		avgGravity.z += accData.z;
	}
	_accOffset.x = avgGravity.x / _CALIBRATION_SAMPLES;
	_accOffset.y = avgGravity.y / _CALIBRATION_SAMPLES;
	_accOffset.z = avgGravity.z / _CALIBRATION_SAMPLES;
}

// Private ---------------------------------------------------------------------
void IMU::_getData(uint8_t reg, int16_t* data1, int16_t* data2, int16_t* data3) const {
	uint8_t buffer[6];
	i2c_write_blocking(_I2C, _BMI160_ADDR, &reg, 1, true);
	i2c_read_blocking(_I2C, _BMI160_ADDR, buffer, 6, false);
	*data1 = _combineBytes(buffer[0], buffer[1]);
	*data2 = _combineBytes(buffer[2], buffer[3]);
	*data3 = _combineBytes(buffer[4], buffer[5]);
}

RotationRates IMU::_gyro() {
	RotationRates gyroData;
	_getData(_GYRO_ADDR, &gyroData.pitch, &gyroData.roll, &gyroData.yaw);
	// 
	// 1 LSB = 1/16.4 degrees/sec
	return gyroData;
}

Gravity IMU::_acc() {
	Gravity accData;
	_getData(_ACC_ADDR, &accData.x, &accData.y, &accData.z);
	_accAvgX.popAndPush(accData.x);
	_accAvgY.popAndPush(accData.y);
	_accAvgZ.popAndPush(accData.z);
	// 1 LSB = +-2g
	return {_accAvgX.average(), _accAvgY.average(), _accAvgZ.average()};
}

// Public ----------------------------------------------------------------------
Angles IMU::getAngles(double deltaTime) {
	// Complementary filter
	constexpr double GYRO_WEIGHT = 0.95;
	constexpr double ACC_WEIGHT = 1 - GYRO_WEIGHT;

	// Get gyro angle estimation
	RotationRates curRotationRate = getRotationRates();
	_gyroAngleEstimation.pitch += curRotationRate.pitch * deltaTime;
	_gyroAngleEstimation.roll += curRotationRate.roll * deltaTime;

	// Get acc angle estimation
	Gravity curGravity = _acc();

	// Subtract offset
	curGravity.x -= _accOffset.x;
	curGravity.y -= _accOffset.y;
	curGravity.z -= _accOffset.z;

	// Convert to pitch and roll
	Angles accAngleEstimation;
	accAngleEstimation.pitch = _toDegrees(atan(-curGravity.x / hypot(curGravity.y, curGravity.z)));
	accAngleEstimation.roll = _toDegrees(atan(curGravity.y / hypot(curGravity.x, curGravity.z)));

	// Combine gyro and acc data
	Angles angleEstimation;
	angleEstimation.pitch = GYRO_WEIGHT * _gyroAngleEstimation.pitch + ACC_WEIGHT * accAngleEstimation.pitch;
	angleEstimation.roll = GYRO_WEIGHT * _gyroAngleEstimation.roll + ACC_WEIGHT * accAngleEstimation.roll;

	// Update gyro estimation with fused angle estimation to prevent drift over time
	_gyroAngleEstimation = angleEstimation;

	return angleEstimation;
}

RotationRates IMU::getRotationRates() {
	RotationRates curRotationRate = _gyro();

	// Subtract offset
	curRotationRate.pitch -= _gyroOffset.pitch;
	curRotationRate.roll -= _gyroOffset.roll;
	curRotationRate.yaw -= _gyroOffset.yaw;

	return curRotationRate;
}