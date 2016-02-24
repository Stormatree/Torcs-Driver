#pragma once

#include <WrapperBaseDriver.h>
#include <chrono>
#include <glm\glm.hpp>
#include <vector>

class MyDriver : public WrapperBaseDriver{
	typedef chrono::high_resolution_clock Clock;
	typedef chrono::duration<double, milli> Milliseconds;

	Clock::time_point _timeCurrent = Clock::now();
	Clock::time_point _timePrevious = _timeCurrent;

	Milliseconds _dt = _timeCurrent - _timePrevious;
	Milliseconds _runtime = Milliseconds::zero();

	float MyDriver::_speedDefault;
	float MyDriver::_speedBoost;
	float MyDriver::_speedBrake;


	//static const float _speedDefault;
	//
	//static const float _speedBoost;
	//static const float _speedBrake;

	static const float _forwardDivisor;

	static const int _gearUp[6];
	static const int _gearDown[6];

	static const float _obstacleDistance;
	static const float _obstacleDivisor;
	static const float _obstacleBrake;

	bool _clutching = false;
	float _trackOffset = 0.f;
	glm::vec2 _steerPush = { 0.f, 0.f };
	float _obstacleAhead = 0.f;

	std::vector<float> _turningAngleHistory;
	std::vector<glm::vec2> _steerPushHistory;

	bool _log;	

	void _steerClamps(CarState& cs);
	float _checkObstacles(CarState& cs, unsigned int start, unsigned end, float distance);
	void _lineSteering(CarState& cs, CarControl& cc);
	void _smartSteering(CarState& cs, CarControl& cc);
	void _pedals(CarState& cs, CarControl& cc);
	void _engageGear(CarControl& cc, int gear = 1);

public:
	MyDriver(bool log = false);
	~MyDriver();

	void onRestart();
	void onShutdown();

	void init(float* angles);
	CarControl wDrive(CarState cs);
};