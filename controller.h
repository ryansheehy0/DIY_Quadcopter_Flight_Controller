#pragma once

struct StickValues {
	double throttle = 0;
	double pitch = 0;
	double roll = 0;
	double yaw = 0;
};

class Controller {
	private:
	public:
		StickValues getStickValues();
};