#include <stdio.h>
#include "pico/stdlib.h"

#include "imu.h"
#include "controller.h"
#include "motor.h"
#include "pid.h"
#include "led.h"

int main() {
	stdio_init_all();

	// Hardware objects
	constexpr uint IMU_SDA_PIN = 16;
	constexpr uint IMU_SCL_PIN = 17;
	IMU imu(IMU_SDA_PIN, IMU_SCL_PIN);

	constexpr uint CONTROLLER_RX_PIN = 1;
	Controller controller(CONTROLLER_RX_PIN);

	constexpr uint FRONT_LEFT_PWM_PIN = 7;
	Motor frontLeftMotor(FRONT_LEFT_PWM_PIN);
	constexpr uint FRONT_RIGHT_PWM_PIN = 26;
	Motor frontRightMotor(FRONT_RIGHT_PWM_PIN);
	constexpr uint BACK_LEFT_PWM_PIN = 11;
	Motor backLeftMotor(BACK_LEFT_PWM_PIN);
	constexpr uint BACK_RIGHT_PWM_PIN = 20;
	Motor backRightMotor(BACK_RIGHT_PWM_PIN);

	LED led;

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
	Angles curAngles;
	StickValues controllerValues;
	uint64_t prevTime = time_us_64();
	double deltaTime = 0;

	/*
	led.on();
	// Wait until near full throttle, then near 0 throttle.
	while (true) {
		if (controller.getStickValues().throttle > 1750) break;
		sleep_ms(10);
	}
	led.off();
	sleep_ms(250);
	led.on();
	while (true) {
		if (controller.getStickValues().throttle < 1050) break;
		sleep_ms(10);
	}
	led.off();

	// Enable motors
	frontLeftMotor.enable();
	frontRightMotor.enable();
	backLeftMotor.enable();
	backRightMotor.enable();
	*/

	while (true) {
		// Get delta time
		deltaTime = (time_us_64() - prevTime) / 1'000'000.0; // Convert to seconds
		prevTime = time_us_64();

		// Get sensor and controller data
		curRotationRates = imu.getRotationRates();
		curAngles = imu.getAngles(deltaTime);
		controllerValues = controller.getStickValues();

		// Calculate control axes
		double throttle = controllerValues.throttle;
		double pitch = pitchPID.compute(deltaTime, curRotationRates.pitch, controllerValues.pitch) * PITCH_SCALE;
		double roll = rollPID.compute(deltaTime, curRotationRates.roll, controllerValues.roll) * ROLL_SCALE;
		double yaw = yawPID.compute(deltaTime, curRotationRates.yaw, controllerValues.yaw) * YAW_SCALE;
		// Change roll, pitch, and yaw to +/- 500
			// Controller range: -min angle to min angle
			// IMU range: angle
			// PID output range: 500 / min angle
		printf("%.12f\n", curAngles.pitch);
		// Pitch controller range: -75 to 75

		// Set motor outputs
		// frontLeftMotor.setOutput( throttle - roll - pitch - yaw);
		// frontRightMotor.setOutput(throttle + roll - pitch + yaw);
		// backLeftMotor.setOutput(  throttle - roll + pitch + yaw);
		// backRightMotor.setOutput( throttle + roll + pitch - yaw);
		/*




		printf("Hello, pico.\n");
		*/
		//sleep_ms(10);
	}
}