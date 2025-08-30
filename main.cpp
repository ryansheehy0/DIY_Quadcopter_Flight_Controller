#include <stdio.h>
#include "IMU/imu.h"
#include "controller.h"
#include "motor.h"
#include "pid.h"
#include "pico/stdlib.h"
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
	const PIDValues PITCH_GAINS = {.p = 1.0, .i = 0.02, .d = 20.0};
	const PIDValues ROLL_GAINS  = {.p = 1.0, .i = 0.02, .d = 20.0};
	const PIDValues YAW_GAINS   = {.p = 1.0, .i = 0.02, .d = 20.0};

	// PID objects
	PID pitchPID(PITCH_GAINS);
	PID rollPID(ROLL_GAINS);
	PID yawPID(YAW_GAINS);

	// Scale PID output to motor input
	constexpr double PITCH_SCALE = 1.00;
	constexpr double ROLL_SCALE = 1.00;
	constexpr double YAW_SCALE = 1.00;

	// Variables
	RotationRates curRotationRates;
	//Angles curAngles;
	StickValues controllerValues;
	uint64_t prevTime = time_us_64();
	double deltaTime = 0;

	// Show led
	// Wait to move stick all the way down in order to start(safety)
	// Turn led off

	while (true) {
		// Get delta time
		deltaTime = (time_us_64() - prevTime) / 1'000'000; // Convert to seconds
		prevTime = time_us_64();

		// Get sensor and controller data
		curRotationRates = imu.getRotationRates();
		// curAngles = imu.getAngles(deltaTime);
		controllerValues = controller.getStickValues();

		// Calculate control axes
		double throttle = controllerValues.throttle;
		double pitch = pitchPID.compute(deltaTime, curRotationRates.pitch, controllerValues.pitch) * PITCH_SCALE;
		double roll = rollPID.compute(deltaTime, curRotationRates.roll, controllerValues.roll) * ROLL_SCALE;
		double yaw = yawPID.compute(deltaTime, curRotationRates.yaw, controllerValues.yaw) * YAW_SCALE;

		// Set motor outputs
		frontLeftMotor.setOutput(throttle - roll - pitch - yaw);
		frontRightMotor.setOutput(throttle + roll + pitch - yaw);
		backLeftMotor.setOutput(throttle - roll + pitch + yaw);
		backRightMotor.setOutput(throttle + roll - pitch + yaw);
	}
}