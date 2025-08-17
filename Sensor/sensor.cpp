#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

constexpr bool SELECT = false;
constexpr bool DESELECT = true;

// Pins
constexpr uint SPI_CS_PIN = 19;
constexpr uint SPI_SCK_PIN = 16;
constexpr uint SPI_MOSI_PIN = 17;
constexpr uint SPI_MISO_PIN = 18;

// Addresses
	// LSB - Least Significant Byte
	// MSB - Most Significant Byte
// Accelerometer
constexpr uint8_t ACC_X_LSB_ADD = 0x12;
constexpr uint8_t ACC_X_MSB_ADD = 0x13;
constexpr uint8_t ACC_Y_LSB_ADD = 0x14;
constexpr uint8_t ACC_Y_MSB_ADD = 0x15;
constexpr uint8_t ACC_Z_LSB_ADD = 0x16;
constexpr uint8_t ACC_Z_MSB_ADD = 0x17;
// Gyroscope
constexpr uint8_t GYRO_PITCH_LSB_ADD = 0x0E;
constexpr uint8_t GYRO_PITCH_MSB_ADD = 0x0F;
constexpr uint8_t GYRO_ROLL_LSB_ADD = 0x0C;
constexpr uint8_t GYRO_ROLL_MSB_ADD = 0x0D;
constexpr uint8_t GYRO_YAW_LSB_ADD = 0x10;
constexpr uint8_t GYRO_YAW_MSB_ADD = 0x11;

/*
Micro-controller is the master.
BMI160 is the slave.
	- VIN - 3.3 - 6v
	- 3v3
	- GND
	- SCL - SCL I2C or SCK SPI
	- SDA - SDA I2C or MOSI SPI
	- CS  - CS SPI
	- SAO - Address select I2C or MISO SPI
	- OCS - Optical Image Stabilization that connects to the camera.
	- INT2 - Interrupt pin 2
	- INT1 - Interrupt pin 1
	- SCX - Magnetometer data out
	- SDX - Magnetometer data I/O

SPI
	- 3v3
	- GND
	- CS  - CS SPI
	- SCL - SCK SPI
	- SAO - MISO SPI
*/

struct RotationRates {
	uint16_t pitch;
	uint16_t roll;
	uint16_t yaw;
};

RotationRates getGyroData();
uint16_t combineBytes(uint8_t lsb, uint8_t msb);
uint8_t getByte(uint8_t reg);
uint16_t getData(uint8_t lsbReg, uint8_t msbReg);

int main() {
	stdio_init_all();
	spi_init(spi0, 1000 * 1000);
	gpio_set_function(SPI_CS_PIN, GPIO_FUNC_SIO);
	gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);

	gpio_set_dir(SPI_CS_PIN, true); // Set CS pin out
	gpio_put(SPI_CS_PIN, DESELECT);

	RotationRates gyroData;

	while (true) {
		gyroData = getGyroData();
		printf("Pitch: %u, Roll: %u, Yaw: %u\n", gyroData.pitch, gyroData.roll, gyroData.yaw);
		sleep_ms(1000);
	}
}

RotationRates getGyroData() {
	RotationRates gyroData;
	gpio_put(SPI_CS_PIN, SELECT);
	gyroData.pitch = getData(GYRO_PITCH_LSB_ADD, GYRO_PITCH_MSB_ADD);
	gyroData.roll = getData(GYRO_ROLL_LSB_ADD, GYRO_ROLL_MSB_ADD);
	gyroData.yaw = getData(GYRO_YAW_LSB_ADD, GYRO_YAW_MSB_ADD);
	gpio_put(SPI_CS_PIN, DESELECT);
	return gyroData;
}

uint16_t combineBytes(uint8_t lsb, uint8_t msb) {
	uint16_t returnedVal = msb;
	returnedVal <<= 8;
	returnedVal |= lsb;
	return returnedVal;
}

uint8_t getByte(uint8_t reg) {
	uint8_t data;
	spi_write_blocking(spi0, &reg, 1);
	spi_read_blocking(spi0, 0, &data, 1);
	return data;
}

uint16_t getData(uint8_t lsbReg, uint8_t msbReg) {
	uint8_t lsb = getByte(lsbReg);
	uint8_t msb = getByte(msbReg);
	return combineBytes(lsb, msb);
}