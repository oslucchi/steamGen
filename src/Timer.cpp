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
		elapsed = mills - startTime;
	}
	if ((elapsed >= duration) && isRunning)
	{
		isRunning = false;
		isExpired = true;
		isFront = false;
	}
}

unsigned long	Timer::getDuration(){
	return(duration);
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

	units = (unsigned long)(((t[0] & 0b00111111)<<1)+(t[1] & 0b10000000));
	decimals = (unsigned long)(t[1] & 0b01111111);

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
}
