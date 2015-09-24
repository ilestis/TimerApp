#pragma once

#include <string>

class Timer
{
public:
	Timer();
	~Timer();

	void initialise(int);

	bool decrease();
	bool reset();
	void start();
	void stop();
	bool switchState();
	std::string formatNumber(int, int);
	std::string formatedString();

private:
	int initialSeconds;
	int remainingSeconds;
	bool running;
};