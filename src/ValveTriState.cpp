/*
 * ValveTriState.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: osvaldo
 */

#include <ValveTriState.h>

ValveTriState::ValveTriState(TimerManager *tm) {
	timer = (Timer **) malloc(sizeof (Timer *));
	timer[0] = tm->getNewTimer("WAY_A");
	timer[1] = tm->getNewTimer("WAY_B");
	way = 0;
}

ValveTriState::~ValveTriState() {
	// TODO Auto-generated destructor stub
}
void ValveTriState::handleCommand(const uint8_t * command, uint8_t * response)
{
	unsigned long now = millis();

	switch(command[0])
	{
	case 0x01:
		debug("ValveTriState: GET_ACTUAL");
		response[0] = 9;
		response[1] = I2CCMD_ACK;
		response[2] = way;
		response[3] = status;
		response[4] = mode;
		memcpy(&response[5],timer[0]->getDuration(), 2);
		memcpy(&response[7],timer[1]->getDuration(), 2);
		break;

	case 0x02:
		debug("ValveTriState: SET_MANUAL");
		way = command[1];
		turnManualOn();
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x03:
		debug("ValveTriState: SET_AUTO");
		switchOff();
		way = command[1];
		setTimerDuration((uint8_t *) &command[2]);
		turnAutoOn(now);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x04:
		debug("ValveTriState: RESET_TIMERS");
		timer[0]->restart(now);
		timer[1]->restart(now);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x05:
		debug("ValveTriState: SET_TIMERS");
		setTimerDuration((uint8_t *)&command[1]);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x06:
		debug("ValveTriState: SWITCH_ON_OFF");
		if (command[1] == 1)
		{
			way = (command[2] == VALVE_WAY_A ? VALVE_WAY_A : VALVE_WAY_B);
			switchOn();
		}
		else
		{
			way = (command[2] == VALVE_WAY_A ? VALVE_WAY_A : VALVE_WAY_B);
			switchOff();
		}
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x07:
		break;

	case 0x08:
		break;

	case 0x09:
		break;
	}
}

void ValveTriState::turnManualOn()
{
	mode = MODE_MANUAL;
	status = COMMAND_ON;
}

void ValveTriState::turnAutoOn(unsigned long now)
{
	char msgOut[24];
	mode = MODE_AUTO;
	status = COMMAND_ON;
	digitalWrite(pin[0], 0);
	digitalWrite(pin[1], 0);
	sprintf(msgOut, "Opening way %d", way);
	debug(msgOut);
	digitalWrite(pin[way], 1);
	timer[way]->restart(now);
	debug(timer[way]->getIsRunning());
	debug(timer[way]->getIsExpired());
}

void ValveTriState::setTimerDuration(uint8_t* timerBuf)
{
	timer[way]->setDuration(timerBuf);
}

void ValveTriState::switchOn()
{
	status = COMMAND_ON;
}


void ValveTriState::switchOff()
{
	digitalWrite(pin[VALVE_WAY_A], 0);
	digitalWrite(pin[VALVE_WAY_B], 0);
	status = COMMAND_OFF;
}

void ValveTriState::setWay(uint8_t  w)
{
	way = w;
}

void ValveTriState::setup(unsigned long now)
{
	digitalWrite(pin[0], 0);
	digitalWrite(pin[1], 0);
	status = COMMAND_OFF;
}

void ValveTriState::loop(unsigned long now)
{
	if (mode == MODE_AUTO)
	{
		if ((timer[way]->getIsExpired()) && (status == 1))
		{
			debug(timer[way]->getIsFront());
			status = COMMAND_OFF;
		}
	}
	digitalWrite(pin[way], status);
}

void ValveTriState::setUpperBound(uint8_t *)
{
	;
}

void ValveTriState::setLowerBound(uint8_t *)
{
	;
}

void ValveTriState::setActualValue(uint8_t *)
{
	;
}

void ValveTriState::setPin(uint8_t *pin)
{
	this->pin[VALVE_WAY_A] = pin[0];
	this->pin[VALVE_WAY_B] = pin[1];
}

void ValveTriState::initializeControl(const uint8_t * command)
{
	;
}

