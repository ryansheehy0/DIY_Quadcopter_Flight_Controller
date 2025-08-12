#include "imu.h"
#include "controller.h"
#include "motor.h"
#include "pid.h"
#include "pico/stdlib.h"

int main() {
	stdio_init_all();

	// Hardware objects
	IMU imu;
	Controller controller;
	Motor frontMotor;
	Motor backMotor;
	Motor leftMotor;
	Motor rightMotor;

	// PID objects
	const PIDValues pitchStabilized = {.p = 2.0, .i = 2.0, .d = 2.0};
	const PIDValues pitchRate       = {.p = 2.0, .i = 2.0, .d = 2.0};
	StabilizedPID pitchPID(pitchStabilized, pitchRate);

	const PIDValues rollStabilized = {.p = 2.0, .i = 2.0, .d = 2.0};
	const PIDValues rollRate       = {.p = 2.0, .i = 2.0, .d = 2.0};
	StabilizedPID rollPID(rollStabilized, rollRate);

	const PIDValues yawRate = {.p = 2.0, .i = 2.0, .d = 2.0};
	PID yawPID(yawRate);

	// Variables
	Angles curAngles;
	RotationRates curRotationRates;
	StickValues controllerValues;
	uint64_t prevTime = time_us_64();
	uint64_t deltaTime;

	while (true) {
		// Get delta time
		deltaTime = time_us_64() - prevTime;
		deltaTime /= 1'000'000; // Convert to seconds
		prevTime = time_us_64();
		printf(deltaTime);
		printf("ms\n");

		// Get sensor and controller data
		curRotationRates = imu.getRotationRate();
		curAngles = imu.getAngles(deltaTime);
		controllerValues = controller.getStickValues();

		// Calculate control axes
		double throttle = controllerValues.throttle;
		double pitch = pitchPID.calcOutput(deltaTime, curAngles.pitch, curRotationRates.pitch, controllerValues.pitch);
		double roll = rollPID.calcOuput(deltaTime, curAngles.roll, curRotationRates.roll, controllerValues.roll);
		double yaw = yawPID.calcOutput(deltaTime, curRotationRates.yaw, controllerValues.yaw);

		// Set motor outputs
		frontMotor.setOutput(throttle - roll - pitch - yaw);
		backMotor.setOutput( throttle + roll + pitch - yaw);
		leftMotor.setOutput( throttle - roll + pitch + yaw);
		rightMotor.setOutput(throttle + roll - pitch + yaw);
	}
}