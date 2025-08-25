#include "pico/stdlib.h"
#include <stdio.h>
#include "imu.h"

int main() {
	stdio_init_all();

	IMU imu;
	RotationRates curRotationRates;
	Angles curAngle;

	uint64_t prevTime = time_us_64();
	//uint64_t deltaTime;
	double deltaTime = 0;

	while (true) {
		// Get delta time
		deltaTime = (time_us_64() - prevTime) / 1'000'000.0;
		//deltaTime /= 1'000'000; // Convert to seconds
		prevTime = time_us_64();
		//printf("DeltaTime %f ", deltaTime);

		curRotationRates = imu.getRotationRates();
		curAngle = imu.getAngles(deltaTime);

		//printf("Rotation Rates: pitch(%f), roll(%f), yaw(%f)", curRotationRates.pitch, curRotationRates.roll, curRotationRates.yaw);
		//printf("Pitch %f ", curRotationRates.pitch);
		printf("Pitch %f \n", curAngle.pitch);
		//sleep_ms(100);
	}
}