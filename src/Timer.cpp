/*
 * Timer.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: osvaldo
 */
#include "Timer.h"

Timer::Timer() {
	duration = 0;
	startTime = 0;
	elapsed = 0;
	isRunning = false;
	isExpired = false;
	strcpy(name, "");
	isFront = false;
}

void Timer::setName(const char* n)
{
	strcpy(name, n);
}

void Timer::setDuration(unsigned long d)
{
	duration = d;
}

unsigned long Timer::getTimeToExpiry()
{
	return((int)(duration - elapsed));
}

unsigned long Timer::getTimeElapsed()
{
	return(elapsed);
}

void Timer::start(unsigned long mills)
{
	startTime = mills;
	isRunning = true;
	isExpired = false;
}

void Timer::stop()
{
	isRunning = false;
}

void Timer::reset()
{
	isRunning = false;
	isExpired = false;
	startTime = 0;
	elapsed = 0;
}

void Timer::resume()
{
	isRunning = true;
	startTime = millis();
}

void Timer::update(unsigned long  mills)
{
	isFront = false;
	if (isRunning)
	{
		if (mills > startTime)
			elapsed = mills - startTime;
	}
	else
		isFront = false;
	if ((elapsed >= duration) && isRunning)
	{
		isRunning = false;
		isExpired = true;
		isFront = false;
	}
}

uint8_t *Timer::getDuration()
{
	unsigned long units, decimals;
	static uint8_t retVal[2];
	if (duration >= (unsigned long) 24 * 60 * 60 * 1000)
	{
		retVal[0] = 192;
		units = duration / (unsigned long) 24 * 60 * 60 * 1000;
		decimals = (duration % 24 * 60 * 60 * 1000) / 60 * 60 * 1000;
	}
	else if (duration >= (unsigned long) 60 * 60 * 1000)
	{
		retVal[0] = 128;
		units = duration / (unsigned long) 60 * 60 * 1000;
		decimals = (duration % 60 * 60 * 1000) / 60 * 1000;
	}
	else if (duration >= (unsigned long) 60 * 1000)
	{
		retVal[0] = 64;
		units = duration / 60 * 1000;
		decimals = (duration % 60 * 1000) / 1000;
	}
	else
	{
		retVal[0] = 0;
		units = duration / 1000;
		decimals = (duration % 1000);
	}
	retVal[0] |= (units >> 1);
	retVal[1] = (((units & 0b00000001)<<7) | decimals);
	return(retVal);
}

char* Timer::getName()
{
	return(name);
}

bool Timer::getIsRunning()
{
	return(isRunning);
}

bool Timer::getIsExpired()
{
	return(isExpired);
}

unsigned long Timer::getElapsed()
{
	return(elapsed);
}

void Timer::restart(unsigned long mills)
{
	reset();
	start(mills);
}

bool Timer::getIsFront()
{
	return isFront;
}
char* Timer::toHex(uint8_t a)
{
	static char hex[6];
	hex[0] = '0';
	hex[1] = 'x';
	hex[2] = (int) (a / 16);
	if (hex[2] >= 10)
		hex[2] += 55;
	else
		hex[2] += 48;
	hex[3] = (a % 16);
	if (hex[3] >= 10)
		hex[3] += 55;
	else
		hex[3] += 48;
	hex[4] = ' ';
	hex[5] = '\0';
	return(hex);
}
void Timer::setDuration(uint8_t * t)
{
	unsigned long units, decimals;
	char msgOut[32];

	units = (unsigned long)(((t[0] & 0b00111111)<<1)+((t[1] & 0b10000000)>>7));
	decimals = (unsigned long)(t[1] & 0b01111111);
	sprintf(msgOut, "units %ld decimals %ld", units, decimals);
	debug(msgOut);

	switch((t[0] & 0b11000000)>>6)
	{
	case 0x00: // seconds
		units *= 1000;
		decimals *= 100;
		break;
	case 0x01: // minutes
		units *= (unsigned long) 60 * 1000;
		decimals *= 1000;
		break;
	case 0x02: // hours
		units *= (unsigned long) 60 * 60 * 1000;
		decimals *= (unsigned long) 60 * 1000;
		break;
	case 0x03: // days
		units *= (unsigned long) 24 * 60 * 60 * 1000;
		decimals *= (unsigned long) 60 * 60 * 1000;
		break;
	}
	duration = units + decimals;
	sprintf(msgOut, "duration set to %ld", duration);
	debug(msgOut);
}
