/*
 * ValveBiState.h
 *
 *  Created on: Feb 18, 2019
 *      Author: osvaldo
 */

#ifndef DIGITALOUTPUT_H_
#define DIGITALOUTPUT_H_

#include "Arduino.h"
#include <Command.h>
#include <TimerManager.h>
#include <CommandHandler.h>

class DigitalOutput: public Command {
	uint8_t pin;
	uint8_t mode = 0;
	uint8_t status = 0;
public:
	DigitalOutput(TimerManager *);
	virtual ~DigitalOutput();
	int getActualValues(unsigned char *);
	void setup(unsigned long mills);
	void loop(unsigned long mills);
	void handleCommand(const uint8_t *inBuf, uint8_t * outBuf);
	void switchOn();
	void switchOff();
	void turnManualOn();
	void turnAutoOn(unsigned long now);
	void fade();
	void setUpperBound(uint8_t *);
	void setLowerBound(uint8_t *);
	void setSpeed(uint8_t *);
	void setActualValue(uint8_t *);
	void setPin(uint8_t*);
	void setTimerDuration(uint8_t *);
	void initializeControl(const uint8_t *);
};

#endif /* DIGITALOUTPUT_H_ */
