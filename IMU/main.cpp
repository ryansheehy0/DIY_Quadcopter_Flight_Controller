#include "pico/stdlib.h"
#include <stdio.h>
#include "imu.h"

int main() {
	stdio_init_all();

	IMU imu;
	RotationRates curRotationRates;
	Angles curAngle;

	uint64_t prevTime = time_us_64();
	double deltaTime = 0;

	while (true) {
		// Get delta time
		deltaTime = (time_us_64() - prevTime) / 1'000'000.0;
		prevTime = time_us_64();

		curRotationRates = imu.getRotationRates();
		curAngle = imu.getAngles(deltaTime);

		//printf("Pitch %f \n", curAngle.pitch);
		printf("Roll %f \n", curAngle.roll);
	}
}