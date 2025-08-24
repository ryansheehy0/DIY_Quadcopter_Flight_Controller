#include "IMU/imu.h"

class NormalizeSensors {
	private:
		RotationRates _maxRotationRates;
		Angles _maxAngles;

	public:
		NormalizeSensors(
			RotationRates maxRotationRates = {
				.pitch = ,
				.roll = ,
				.yaw =
			},
			Angles maxAngles = {
				.pitch = ,
				.roll = ,
				.yaw =
			}
		) {
			_maxRotationRates = maxRotationRates;
			_maxAngles = maxAngles;
		}

		RotationRates normalizeRotationRates(RotationRates rawRotationRates) const;
		Angles normalizeAngles(Angles rawAngles) const;
};