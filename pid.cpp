#include "pid.h"

// Public ----------------------------------------------------------------------

double PID::compute(uint64_t deltaTime, double measuredValue, double setPoint) {
	double error = setPoint - measuredValue;

	// Get each term
	double proportional = _gains.p * error;
	_integral += _gains.i * error * deltaTime;
	double derivative = _gains.d * ((error - _prevError) / deltaTime);
	_prevError = error;

	return proportional + _integral + derivative;
}

double StabilizedPID::compute(
	uint64_t deltaTime,
	double measuredAngle,
	double measuredRotationRate,
	double controllerSetPoint
) {
	double rotationRateSetPoint = _stabilizedPID.compute(deltaTime, measuredAngle, controllerSetPoint);
	return _ratePID.compute(deltaTime, measuredRotationRate, rotationRateSetPoint);
}