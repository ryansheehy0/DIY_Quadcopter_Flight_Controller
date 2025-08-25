#include "IMU/imu.h"
#include "controller.h"
#include "motor.h"
#include "pid.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "normalize_sensor.h"

int main() {
	stdio_init_all();

	// Hardware objects
	IMU imu;
	Controller controller;
	Motor frontLeftMotor;
	Motor frontRightMotor;
	Motor backLeftMotor;
	Motor backRightMotor;

	// PID constants
	const PIDValues pitchGains = {.p = 1.0, .i = 0.02, .d = 20.0};
	const PIDValues rollGains  = {.p = 1.0, .i = 0.02, .d = 20.0};
	const PIDValues yawGains   = {.p = 1.0, .i = 0.02, .d = 20.0};

	// PID objects
	PID pitchPID(pitchGains);
	PID rollPID(rollGains);
	PID yawPID(yawGains);

	// Variables
	RotationRates curRotationRates;
	//Angles curAngles;
	StickValues controllerValues;
	uint64_t prevTime = time_us_64();
	double deltaTime = 0;

	while (true) {
		// Get delta time
		deltaTime = (time_us_64() - prevTime) / 1'000'000; // Convert to seconds
		prevTime = time_us_64();

		// Get sensor and controller data
		curRotationRates = imu.getRotationRates();
		// curAngles = imu.getAngles(deltaTime);
		controllerValues = controller.getStickValues();

		// Normalize sensor data
		//normalizedRotationRates = NormalizeIMU::

		// Calculate control axes
		double throttle = controllerValues.throttle;
		double pitch = pitchPID.compute(deltaTime, curRotationRates.pitch, controllerValues.pitch);
		double roll = rollPID.compute(deltaTime, curRotationRates.roll, controllerValues.roll);
		double yaw = yawPID.compute(deltaTime, curRotationRates.yaw, controllerValues.yaw);

		// Set motor outputs
		frontLeftMotor.setOutput(throttle - roll - pitch - yaw);
		frontRightMotor.setOutput(throttle + roll + pitch - yaw);
		backLeftMotor.setOutput(throttle - roll + pitch + yaw);
		backRightMotor.setOutput(throttle + roll - pitch + yaw);
	}
}