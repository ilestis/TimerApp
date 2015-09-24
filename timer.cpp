#include "timer.h"
#include <sstream>
#include <iomanip>

// Constructor
Timer::Timer()
{
}

// Destructor
Timer::~Timer()
{
}

void Timer::initialise(int seconds)
{
	initialSeconds = seconds;
	remainingSeconds = seconds;
	running = false;
}

// Start the timer
void Timer::start()
{
	running = true;
}

// Reset the timer to it's initial countdown
bool Timer::reset()
{
	// Don't reset while the clock is runnin
	if (running) {
		return false;
	}
	// If the timer is stoped, we can reset the value
	remainingSeconds = initialSeconds;
	return true;
}

// Decrease the timer (if it's above 0)
bool Timer::decrease()
{
	if (remainingSeconds > 0) {
		remainingSeconds--;
		return true;
	}
	// We've droped to 0, stop updating the timer
	return false;
}

// Stop the timer
void Timer::stop()
{
	running = false;
}

// Switch the running state of the timer.
bool Timer::switchState()
{
	if (running) {
		running = false;
	}
	else {
		running = true;
	}
	return running;
}

// Format a number into a 2 digit string with a leading 0 if required.
std::string Timer::formatNumber(int number, int width = 2)
{
	std::stringstream sNumber;
	sNumber << std::setw(width) << std::setfill('0') << number;
	return sNumber.str();
}

// Format the remaining timer into pretty text
std::string Timer::formatedString()
{
	int hours, minutes, seconds;
	std::ostringstream sTimer;

	hours = remainingSeconds / 3600;
	minutes = (remainingSeconds - (hours * 3600)) / 60;
	seconds = remainingSeconds - (hours * 3600) - (minutes * 60);

	sTimer << formatNumber(hours) << ":" << formatNumber(minutes) << ":" << formatNumber(seconds);
	
	return sTimer.str();
}