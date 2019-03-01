/*
 * Command.h
 *
 *  Created on: Feb 15, 2019
 *      Author: osvaldo
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include "Arduino.h"

#include <Timer.h>
#include <TimerManager.h>

class Command {
public:
	uint8_t ctrlId;
	uint8_t ctrlType;
	uint8_t *pin;
	uint8_t *upperBound;
	uint8_t *lowerBound;
	uint8_t *actualValue;
	Timer **timer;
	TimerManager* timerManager;

	Command();
	virtual ~Command();
	virtual void setup(unsigned long mills) = 0;
	virtual void loop(unsigned long mills) = 0;

	virtual void handleCommand(const uint8_t *inBuf, uint8_t * outBuf) = 0;

	Timer *getTimer(int idx);
	void setTimer(Timer **timer);
	void setTimerManager(TimerManager* tm);
	uint8_t* getUpperBound();
	virtual void setUpperBound(uint8_t *) = 0;
	uint8_t* getLowerBound();
	virtual void setLowerBound(uint8_t *) = 0;
	uint8_t* getActualValue();
	virtual void setActualValue(uint8_t *) = 0;
	virtual void setPin(uint8_t *) = 0;
	virtual void initializeControl(const uint8_t *) = 0;
};

#endif /* COMMAND_H_ */
