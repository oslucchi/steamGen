/*
 * ValveTriState.h
 *
 *  Created on: Feb 18, 2019
 *      Author: osvaldo
 */

#ifndef VALVETRISTATE_H_
#define VALVETRISTATE_H_
#include "Arduino.h"
#include <steamGen.h>
#include <Command.h>
#include <CommandHandler.h>

#define	VALVE_WAY_A	0x00
#define	VALVE_WAY_B	0x01
#define	COMMAND_OFF	0x00
#define	COMMAND_ON	0x01
#define	MODE_MANUAL	0x00
#define	MODE_AUTO	0x01

class ValveTriState: public Command {
	uint8_t way = 0;
	uint8_t mode = 0;
	uint8_t status = 0;
	uint8_t pin[2];

public:
	ValveTriState(TimerManager *);
	virtual ~ValveTriState();
	void setWay(uint8_t);
	int getActualValues(uint8_t *);
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

#endif /* VALVETRISTATE_H_ */
