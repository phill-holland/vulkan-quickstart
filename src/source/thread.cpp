#include "thread.h"

std::chrono::milliseconds thread::milliseconds = std::chrono::milliseconds(100);

thread::thread()
{
	init = false;

	makeNull();

	running = false; stopped = false;
	paused = false; iampaused = false;
	requestedstop = false;

	neverstarted = true;
}

void thread::stop()
{
	if (running)
	{
		if (!requestedstop)
		{
			requestedstop = true;
		}

		int error = 0;
		while ((!stopped) && (error < 100)) { sleep(50); ++error; }

		if(handle->joinable()) 
			handle->join();
	}
	else if (neverstarted) stopped = true;
}

void thread::pause(bool s)
{
	paused = s;
}

bool thread::pauseAndWait(bool value, long time, long max)
{
	pause(value);

	if ((value) && (running))
	{
		long error = 0L;
		while ((!isPaused()) && (error < max))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
			++error;
		}

		if (error >= max) return false;
	}

	return true;
}

bool thread::WaitLongTimeWithInteruption(long time)
{
	const int wait = 200;
	int loops = time / wait;
	int counter = 0;

	while (!isPaused() && isRunning() && (counter < loops))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(wait));
		++counter;
	}

	return counter == loops;
}

bool thread::create()
{
	running = true; stopped = false;
	paused = false; iampaused = false;
	requestedstop = false;

	handle = new std::thread(thread::interrupt, this);
	if (handle == NULL) return false;

	return true;
}

void thread::cleanup()
{
	if (handle != NULL)
	{
		stop();

		delete handle;
	}
}