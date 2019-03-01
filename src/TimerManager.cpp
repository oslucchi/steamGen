/*
 * TimerManager.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: osvaldo
 */
#include <TimerManager.h>
#include "Arduino.h"

TimerManager::TimerManager() {
	for(int i = 0; i < HOW_MANY; i++)
	{
		timers[i].timer = nullptr;
		timers[i].allocated = false;
	}
}
void TimerManager::update(unsigned long now)
{
	for(int i =0; i < HOW_MANY; i++)
	{
		if (timers[i].timer != nullptr)
		{
			timers[i].timer->update(now);
		}
	}
}
void TimerManager::dropTimer(Timer* timer)
{
	for(int i =0; i < HOW_MANY; i++)
	{
		if (timers[i].timer == timer)
		{
			timers[i].timer->reset();
			timers[i].allocated = false;
			break;
		}
	}
}

Timer* TimerManager::getNewTimer(const char *n)
{
	for(int i =0; i < HOW_MANY; i++)
	{
		if (!timers[i].allocated)
		{
			timers[i].allocated = true;
			timers[i].timer = new Timer();
			timers[i].timer->setName(n);
			return(timers[i].timer);
		}
	}
	return(nullptr);
}

void TimerManager::dump()
{
	char msgOut[64];
	for(int i =0; i < HOW_MANY; i++)
	{
		if (timers[i].allocated)
		{
			sprintf(msgOut, "Timer %s - dur %ld, els %ld, status %c, exp %c",
					timers[i].timer->getName(),
					timers[i].timer->getDuration(),
					timers[i].timer->getElapsed(),
					(timers[i].timer->getIsRunning() ? 'Y' : 'N'),
					(timers[i].timer->getIsExpired() ? 'Y' : 'N'));
			Serial.println(msgOut);
		}
	}
}
