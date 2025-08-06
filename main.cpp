#include "imu.h"
#include "controller.h"
#include <stdio.h>
#include <hardware/timer.h>

int main() {
	IMU imu;
	Controller controller;
	Angles controllerAngles;
	Angles curAngles;
	RotationRates curRotationRates;

	uint64_t prevTime = time_us_64();

	while (true) {
		curAngles = imu.getAngles();
		curRotationRates = imu.getRotationRate();
		controllerAngles = controller.getAngles();

	}
}

	while (true) {
		tuple<Pitch, Roll, Yaw, bool> userValues = readUserValues();
		controlMotors(sensorData, userValues);
	}