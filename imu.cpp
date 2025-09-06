#include "imu.h"
#include <cmath> // atan, hypot

// Constructor -----------------------------------------------------------------
IMU::IMU() {
	// I2C consts
	constexpr uint SCL_PIN = 17;
	constexpr uint SDA_PIN = 16;
	constexpr uint BAUD_RATE = 400'000;

	// Addresses
	constexpr uint8_t CMD_ADDR = 0x7E;
	constexpr uint8_t GYRO_NORMAL_MODE = 0x15;
	constexpr uint8_t ACC_NORMAL_MODE = 0x11;
	constexpr uint8_t GYRO_RANGE = 0x43;
	constexpr uint8_t GYRO_RANGE_500_DEG_PER_SEC = 0x02;
	constexpr uint8_t ACC_RANGE = 0x41;
	constexpr uint8_t ACC_RANGE_8G = 0x08;

	// Other
	constexpr int CALIBRATION_SAMPLES = 2000;

	// Init I2C
	i2c_init(_I2C, BAUD_RATE);
	gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
	gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(SCL_PIN);
	gpio_pull_up(SDA_PIN);

	// Set Gyro to normal mode
	uint8_t gyroCmd[2] = {CMD_ADDR, GYRO_NORMAL_MODE};
	i2c_write_blocking(_I2C, _BMI160_ADDR, gyroCmd, 2, false);
	sleep_ms(500);

	// Set Acc to normal mode
	uint8_t accCmd[2] = {CMD_ADDR, ACC_NORMAL_MODE};
	i2c_write_blocking(_I2C, _BMI160_ADDR, accCmd, 2, false);
	sleep_ms(500);

	// Set Gyro range to +/- 500 degrees per sec
	uint8_t gyroRangeCmd[2] = {GYRO_RANGE, GYRO_RANGE_500_DEG_PER_SEC};
	i2c_write_blocking(_I2C, _BMI160_ADDR, gyroRangeCmd, 2, false);
	sleep_ms(500);

	// Set Acc range to +/- 8g
	uint8_t accRangeCmd[2] = {ACC_RANGE, ACC_RANGE_8G};
	i2c_write_blocking(_I2C, _BMI160_ADDR, accRangeCmd, 2, false);
	sleep_ms(500);

	// Calibrate gyroscope
	RotationRates avgRates;
	for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
		RotationRates gyroRates = _gyro();
		avgRates.pitch += gyroRates.pitch;
		avgRates.roll += gyroRates.roll;
		avgRates.yaw += gyroRates.yaw;
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
	}
	_accOffset.x = avgGravity.x / CALIBRATION_SAMPLES;
	_accOffset.y = avgGravity.y / CALIBRATION_SAMPLES;
	_accOffset.z = (avgGravity.z / CALIBRATION_SAMPLES) + 1;
}

// Private ---------------------------------------------------------------------
RawData IMU::_getRawData(uint8_t reg) const {
	RawData rawData;
	uint8_t buffer[6];
	i2c_write_blocking(_I2C, _BMI160_ADDR, &reg, 1, true);
	i2c_read_blocking(_I2C, _BMI160_ADDR, buffer, 6, false);
	rawData.data1 = _combineBytes(buffer[0], buffer[1]);
	rawData.data2 = _combineBytes(buffer[2], buffer[3]);
	rawData.data3 = _combineBytes(buffer[4], buffer[5]);
	return rawData;
}

RotationRates IMU::_gyro() {
	// Gyro consts
	constexpr uint8_t GYRO_ADDR = 0x0C;
	constexpr double LSB_TO_DEG_PER_SEC = 65.6; // 1 gyro LSB = 1/65.6 degrees/sec

	// Get raw data
	RawData gyroRawData = _getRawData(GYRO_ADDR);

	// Convert raw data to degrees/sec
	RotationRates gyroData;
	gyroData.roll = gyroRawData.data1 / LSB_TO_DEG_PER_SEC;
	gyroData.pitch = gyroRawData.data2 / LSB_TO_DEG_PER_SEC;
	gyroData.yaw = gyroRawData.data3 / LSB_TO_DEG_PER_SEC;

	return gyroData;
}

Gravity IMU::_acc() {
	// Acc consts
	constexpr uint8_t ACC_ADDR = 0x12;
	constexpr double LSB_TO_GRAVITY = 4096; // 1 acc LSB = 1/4096 g

	// Get raw data
	RawData accRawData = _getRawData(ACC_ADDR);

	// Convert raw data to gravity
	Gravity accData;
	accData.x = accRawData.data1 / LSB_TO_GRAVITY;
	accData.y = accRawData.data2 / LSB_TO_GRAVITY;
	accData.z = accRawData.data3 / LSB_TO_GRAVITY;

	// Calculate and return average
	_accAvgX.popAndPush(accData.x);
	_accAvgY.popAndPush(accData.y);
	_accAvgZ.popAndPush(accData.z);
	return {_accAvgX.average(), _accAvgY.average(), _accAvgZ.average()};
}

// Public ----------------------------------------------------------------------
RotationRates IMU::getRotationRates() {
	RotationRates curRotationRate = _gyro();

	// Subtract offsets
	curRotationRate.pitch -= _gyroOffset.pitch;
	curRotationRate.roll -= _gyroOffset.roll;
	curRotationRate.yaw -= _gyroOffset.yaw;

	return curRotationRate;
}

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
	accAngleEstimation.pitch = _toDegrees(atan2(-curGravity.x, hypot(curGravity.y, curGravity.z)));
	accAngleEstimation.roll = _toDegrees(atan2(curGravity.y, hypot(curGravity.x, curGravity.z)));

	// Combine gyro and acc data
	Angles angleEstimation;
	angleEstimation.pitch = GYRO_WEIGHT * _gyroAngleEstimation.pitch + ACC_WEIGHT * accAngleEstimation.pitch;
	angleEstimation.roll = GYRO_WEIGHT * _gyroAngleEstimation.roll + ACC_WEIGHT * accAngleEstimation.roll;

	// Update gyro estimation with fused angle estimation to prevent drift over time
	_gyroAngleEstimation = angleEstimation;

	return angleEstimation;
}