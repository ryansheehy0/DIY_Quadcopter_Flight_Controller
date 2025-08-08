#pragma once

#include <stdint.h>

struct PIDValues {
	double p = 0;
	double i = 0;
	double d = 0;
};

class PID {
	private:
		PIDValues _gains;
		double _integral = 0;
		double _prevError = 0;

	public:
		PID(PIDValues gains) : _gains(gains) {}

		double calcOutput(uint64_t deltaTime, double measuredValue, double setPoint);
};

class StabilizedPID {
	private:
		PID _stabilizedPID;
		PID _ratePID;

	public:
		StabilizedPID(PIDValues stabilizedGains, PIDValues rateGains) : _stabilizedPID(stabilizedGains), _ratePID(rateGains) {}

		double calcOutput(uint64_t deltaTime, double measuredAngle, double measuredRotationRate, double controllerSetPoint);
};