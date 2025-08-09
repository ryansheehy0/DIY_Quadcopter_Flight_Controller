#include "pid.h"

// Public ----------------------------------------------------------------------

double PID::calcOutput(uint64_t deltaTime, double measuredValue, double setPoint) {
	double error = setPoint - measuredValue;

	// Get each term
	double proportional = _gains.p * error;
	_integral += _gains.i * error * deltaTime;
	double derivative = _gains.d * ((error - _prevError) / deltaTime);
	_prevError = error;

	return proportional + _integral + derivative;
}

double StabilizedPID::calcOutput(
	uint64_t deltaTime,
	double measuredAngle,
	double measuredRotationRate,
	double controllerSetPoint
) {
	double rotationRateSetPoint = _stabilizedPID.calcOutput(deltaTime, measuredAngle, controllerSetPoint);
	return _ratePID.calcOutput(deltaTime, measuredRotationRate, rotationRateSetPoint);
}