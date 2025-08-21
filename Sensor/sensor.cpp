#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

constexpr bool SELECT = false;
constexpr bool DESELECT = true;

// Pins
constexpr uint TX_PIN = 19;
constexpr uint SCK_PIN = 18;
constexpr uint CS_PIN = 17;
constexpr uint RX_PIN = 16;

// Addresses
// Accelerometer
constexpr uint8_t ACC_X_ADDR = 0x12;
constexpr uint8_t ACC_Y_ADDR = 0x14;
constexpr uint8_t ACC_Z_ADDR = 0x16;
// Gyroscope
constexpr uint8_t GYRO_PITCH_ADDR = 0x0E;
constexpr uint8_t GYRO_ROLL_ADDR = 0x0C;
constexpr uint8_t GYRO_YAW_ADDR = 0x10;
/*
Micro-controller is the master.
BMI160 is the slave.
	- VIN - 3.3 - 6v
	- 3v3
	- GND
	- SCL - SCL I2C or SCK SPI
	- SDA - SDA I2C or TX SPI
	- CS  - CS SPI
	- SAO - Address select I2C or RX SPI
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
	- SAO - RX SPI
	- SDA - TX SPI
*/

uint16_t combineBytes(uint8_t lsb, uint8_t msb);
uint16_t getData(uint8_t reg);
void bmi160Init();

int main() {
	stdio_init_all(); // Init I/O for USB Serial

	spi_init(spi0, 1000 * 1000); // Init spi

	// Init GPIO pins for SPI
	gpio_set_function(TX_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(RX_PIN, GPIO_FUNC_SPI);

	// Config chip select
	gpio_init(CS_PIN);
	gpio_set_dir(CS_PIN, GPIO_OUT);
	gpio_put(CS_PIN, DESELECT);
	gpio_put(CS_PIN, SELECT);
	gpio_put(CS_PIN, DESELECT);

	//bmi160Init();

	while (true) {
		uint16_t pitch = getData(GYRO_PITCH_ADDR);
		uint16_t roll = getData(GYRO_ROLL_ADDR);
		uint16_t yaw = getData(GYRO_YAW_ADDR);
		printf("Pitch: %u, Roll: %u, Yaw: %u\n", pitch, roll, yaw);
		sleep_ms(1000);
	}
}

uint16_t getData(uint8_t reg) {
	reg |= 0x80;
	uint8_t data[2];
	gpio_put(CS_PIN, SELECT);
	spi_write_blocking(spi0, &reg, 1); // Send register address
	spi_read_blocking(spi0, 0, data, 2); // Read 2 bytes
	gpio_put(CS_PIN, DESELECT);
	return combineBytes(data[0], data[1]);
}

uint16_t combineBytes(uint8_t lsb, uint8_t msb) {
	uint16_t returnedVal = msb;
	returnedVal <<= 8;
	returnedVal |= lsb;
	return returnedVal;
}

void bmi160Init() {
	/*
		 - It is recommended to perform a SPI single read access to the ADDRESS 0x7F before the actual communication in order to use the SPI interface.
		 - Default mode is I2C.
		 - If toggling of the CSB bit is not possible without data communication, there is in addiction the spi_en bit in Register (0x70) NV_CONF, which can be used to permanently set the primary interface to SPI without the need to toggle the CSB pin at every power-up or reset.
		 - Max SPI 10MHz
	*/
    uint8_t regAddr = 0x70 & 0x7F;  // write, clear MSB
    uint8_t value   = 0x01;         // set spi_en bit
    uint8_t tx[2]   = { regAddr, value };

    gpio_put(CS_PIN, SELECT);
    spi_write_blocking(spi0, tx, 2);
    gpio_put(CS_PIN, DESELECT);

	// uint8_t dummyReg = 0x7F;
	// uint8_t dummyOut = 0;

	/*
bmi160_get_regs(BMI160_SPI_COMM_TEST_ADDR, &data, 1, dev);
int8_t bmi160_get_regs(uint8_t reg_addr, uint8_t *data, uint16_t len, const struct bmi160_dev *dev)
{
	*/

	// gpio_put(CS_PIN, SELECT);
	// spi_write_blocking(spi0, &dummyReg, 1); // Send register address
	// spi_read_blocking(spi0, 0, &dummyOut, 1); // Read 2 bytes
	// //spi_write_read_blocking(spi0, &dummyReg, &dummyOut, 1); // Dummy read to enable SPI
	// gpio_put(CS_PIN, DESELECT);
	// // uint8_t cmd[2] = { 0x7E, 0x15 }; // gyro normal mode
	// // gpio_put(CS_PIN, SELECT);
	// // spi_write_blocking(spi0, cmd, 2);
	// // gpio_put(CS_PIN, DESELECT);
	sleep_ms(1000); // let gyro start up
}