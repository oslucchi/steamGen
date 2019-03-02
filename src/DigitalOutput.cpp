/*
 * ValveBiState.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: osvaldo
 */

#include <DigitalOutput.h>

DigitalOutput::DigitalOutput(TimerManager *) {
	// TODO Auto-generated constructor stub

}

DigitalOutput::~DigitalOutput() {
	// TODO Auto-generated destructor stub
}

void DigitalOutput::handleCommand(const uint8_t * command, uint8_t * response)
{
	unsigned long now = millis();

	switch(command[0])
	{
	case 0x01:
		debug("DigitalOutput: GET_ACTUAL");
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x02:
		debug("DigitalOutput: SET_MANUAL");
		turnManualOn();
		switchOff();
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x03:
		debug("DigitalOutput: SET_AUTO");
		setTimerDuration((uint8_t *) &command[1]);
		turnAutoOn(now);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x04:
		debug("DigitalOutput: RESET_TIMERS");
		timer[0]->restart(now);
		timer[1]->restart(now);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x05:
		debug("DigitalOutput: SET_TIMERS");
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

void DigitalOutput::setUpperBound(uint8_t *)
{
	;
}

void DigitalOutput::setup(unsigned long now)
{
}

void DigitalOutput::loop(unsigned long now)
{
	if (timer[0]->getIsExpired() && timer[0]->getIsFront())
	{
		switchOff();
	}
}
void DigitalOutput::turnManualOn()
{
	mode = MODE_MANUAL;
}

void DigitalOutput::turnAutoOn(unsigned long now)
{
	mode = MODE_AUTO;
	timer[VALVE_WAY_B]->restart(now);
	timer[VALVE_WAY_B]->restart(now);
}

void DigitalOutput::setTimerDuration(uint8_t* timerBuf)
{
	unsigned long now = millis();
	timer[VALVE_WAY_A]->setDuration(timerBuf);
	timer[VALVE_WAY_A]->restart(now);
	timer[VALVE_WAY_B]->setDuration((uint8_t *) (timerBuf + 2));
	timer[VALVE_WAY_B]->restart(now);
}

void DigitalOutput::switchOn()
{
	;
}


void DigitalOutput::switchOff()
{
	digitalWrite(pin, 0);
}

void DigitalOutput::setLowerBound(uint8_t *)
{
	;
}

void DigitalOutput::setActualValue(uint8_t *)
{
	;
}

void DigitalOutput::setPin(uint8_t *pin)
{
	this->pin = pin[0];
}

void DigitalOutput::initializeControl(const uint8_t * command)
{
	;
}

