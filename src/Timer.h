/*
 * Timer.h
 *
 *  Created on: Feb 10, 2019
 *      Author: osvaldo
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "Arduino.h"
#include <steamBath.h>

class Timer {
	unsigned long startTime;
	unsigned long duration;
	unsigned long elapsed = 0;
	bool isRunning;
	bool isExpired;
	bool isFront;
	char name[8];

private:
	;
public:
	Timer();
	void setDuration(unsigned long);
	void setDuration(uint8_t *);
	void setName(const char *);
	unsigned long getTimeToExpiry();
	unsigned long getTimeElapsed();
	unsigned long getDuration();
	char *getName();
	bool getIsRunning();
	bool getIsExpired();
	bool getIsFront();
	unsigned long getElapsed();
	void start(unsigned long);
	void stop();
	void reset();
	void resume();
	void restart(unsigned long);
	void update(unsigned long);
	char* toHex(uint8_t a);
};

#endif /* TIMER_H_ */
