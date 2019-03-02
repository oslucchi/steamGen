/*
 * Input.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: osvaldo
 */

#include <DigitalInput.h>

DigitalInput::DigitalInput(TimerManager *) {
	// TODO Auto-generated constructor stub

}

DigitalInput::~DigitalInput() {
	// TODO Auto-generated destructor stub
}

void DigitalInput::handleCommand(const uint8_t * command, uint8_t * response)
{
	unsigned long now = millis();

	switch(command[0])
	{
	case 0x01:
		debug("ValveTriState: GET_ACTUAL");
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x02:
		debug("ValveTriState: SET_MANUAL");
		turnManualOn();
		switchOff();
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x03:
		debug("ValveTriState: SET_AUTO");
		setTimerDuration((uint8_t *) &command[1]);
		turnAutoOn(now);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x04:
		debug("LedRGB: RESET_TIMERS");
		timer[0]->restart(now);
		timer[1]->restart(now);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x05:
		debug("LedRGB: SET_TIMERS");
		setTimerDuration((uint8_t *)&command[1]);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x06:
		break;

	case 0x07:
		break;

	case 0x08:
		break;

	case 0x09:
		break;
	}
}

void DigitalInput::setUpperBound(uint8_t *)
{
	;
}

void DigitalInput::setup(unsigned long now)
{
}

void DigitalInput::loop(unsigned long now)
{
	if (timer[0]->getIsExpired() && timer[0]->getIsFront())
	{
		switchOff();
	}
}
void DigitalInput::turnManualOn()
{
	mode = MODE_MANUAL;
}

void DigitalInput::turnAutoOn(unsigned long now)
{
	mode = MODE_AUTO;
	timer[0]->restart(now);
}

void DigitalInput::setTimerDuration(uint8_t* timerBuf)
{
	unsigned long now = millis();
	timer[0]->setDuration(timerBuf);
}

void DigitalInput::switchOn()
{
	;
}


void DigitalInput::switchOff()
{
	;
}

void DigitalInput::setLowerBound(uint8_t *)
{
	;
}

void DigitalInput::setActualValue(uint8_t *)
{
	;
}

void DigitalInput::setPin(uint8_t *pin)
{
	this->pin = pin[0];
}

void DigitalInput::initializeControl(const uint8_t * command)
{
	;
}

