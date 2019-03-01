/*
 * ArduinoControl.cpp
 *
 *  Created on: Feb 16, 2019
 *      Author: osvaldo
 */

#include <ArduinoControl.h>

ArduinoControl::ArduinoControl() {
	uint8_t pin[3], upperBound[3], lowerBound[3], actualValue[3];

	initialized = false;
	debug("ArduinoControl: registering controls");

	pin[VALVE_WAY_A] = 7;
	pin[VALVE_WAY_B] = 8;
	pin[2] = -1;
	upperBound[0] = 1;
	upperBound[1] = -1;
	upperBound[2] = -1;
	lowerBound[0] = 0;
	lowerBound[1] = -1;
	lowerBound[2] = 1;
	actualValue[0] = 0;
	actualValue[1] = -1;
	actualValue[2] = -1;
	steamLigthsSensors.addCommand(CTRLID_STEAM_WATER_IN, (uint8_t) TYPE_VALVE_TRISTATE, pin,
								  upperBound, lowerBound, actualValue, &timerManager);

	pin[0] = 12;
	pin[1] = -1;
	steamLigthsSensors.addCommand(CTRLID_STEAM_WATER_OUT, (uint8_t) TYPE_DIGITAL_OUT, pin,
								  upperBound, lowerBound, actualValue, &timerManager);

	pin[0] = 4;
	steamLigthsSensors.addCommand(CTRLID_STEAM_POWER, (uint8_t) TYPE_DIGITAL_OUT, pin,
								  upperBound, lowerBound, actualValue, &timerManager);

	pin[0] = 13;
	steamLigthsSensors.addCommand(CTRLID_HUMIDITY_SENSOR, (uint8_t) TYPE_DIGITAL_IN, pin,
								  upperBound, lowerBound, actualValue, &timerManager);
}

ArduinoControl::~ArduinoControl() {
	// TODO Auto-generated destructor stub
}

char* ArduinoControl::toHex(uint8_t a)
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

void ArduinoControl::setup(unsigned long now)
{
	cmdList *current = steamLigthsSensors.getController();
	while((current = current->next) != nullptr)
	{
		current->cmd->setup(now);
	}
}

void ArduinoControl::loop(unsigned long now)
{
	if (writeIdx != readIdx)
	{
		handleI2CMessage();
	}
	timerManager.update(now);

	cmdList *current = steamLigthsSensors.getController();
	while((current = current->next) != nullptr)
	{
		current->cmd->loop(now);
	}
}

Command* ArduinoControl::getCommand(unsigned char ctrlId)
{
	cmdList *current = steamLigthsSensors.getController();
	while((current = current->next) != nullptr)
	{
		if (current->cmd->ctrlId == ctrlId)
			return(current->cmd);
	}
	return(nullptr);
}

void ArduinoControl::handleCommand(const uint8_t * command, uint8_t * response)
{
	switch(command[0])
	{
	case 0x01:
		debug("I2CCMD_KEEP_ALIVE");
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;

	case 0x02:
		debug("I2CCMD_INITIALIZE");
		initializeControls(&command[1], response);
		response[0] = 2;
		response[1] = I2CCMD_ACK;
		break;
	}
}

int ArduinoControl::handleI2CMessage()
{
	uint8_t localBuf[32];
	char msgOut[64];

	if (readIdx == COMM_BUF_SIZE - 1) readIdx = 0;
	uint8_t cmdLen = commandBuf[readIdx];

	sprintf(msgOut, "Received command (l %d, r %d, w %d):",
			cmdLen, readIdx, writeIdx);
	debug(msgOut);

	for(int i = 0; i < cmdLen; i++)
	{
		if (readIdx == COMM_BUF_SIZE - 1) readIdx = 0;
		localBuf[i] = commandBuf[readIdx++];
		debug_c(toHex(localBuf[i]));
	}
	sprintf(msgOut, "\nIndexes after reading: r%d, w%d", readIdx, writeIdx);
	debug(msgOut);

	switch(localBuf[1])
	{
	case 0:
		// Command to the arduino control
		handleCommand(&localBuf[2], responseBuf);
		break;

	default:
		Command* ctrl = getCommand(localBuf[1]);
		if ((ctrl == nullptr) || !initialized)
		{
			responseBuf[0] = (uint8_t) 0x02;
			responseBuf[1] = (uint8_t) I2CCMD_NACK;
		}
		else
		{
			ctrl->handleCommand(&localBuf[2], responseBuf);
		}
	}
	responseLen = responseBuf[0];
	return responseBuf[0];
}

int ArduinoControl::initializeControls(const uint8_t * command, uint8_t * response)
{
	Command* ctrl;
	ctrl = getCommand(CTRLID_STEAM_WATER_IN);
	((ValveTriState *)ctrl)->initializeControl(command);

	ctrl = getCommand(CTRLID_STEAM_WATER_OUT);
	((ValveTriState *)ctrl)->initializeControl(command);

//	Valve steamWaterIn
//		byte 2 which way A / B (0x01 / 0x02)
//		byte 3 auto/manual (0x00 / 0x01)
//		byte 3-4 timer
//			2 bits timer units (0x00 seconds, 0x01 minutes, 0x10 hours, 0x11 days)
//			7 bits # of units
//			7 bits # decimals to units (e.g. if units is second, this is 1/10 of seconds, if is minutes, this is seconds etc)

//	offset += 18;
//	ctrl = getCommand(CTRLID_STEAM_WATER_IN);
//	((ValveTriState *)ctrl)->setWay(command[offset + 2]);
//	((ValveTriState *)ctrl)->setManual(command[offset + 3]);
//	((ValveTriState *)ctrl)->setTimer((unsigned char *)&command[offset + 4]);

//	Valve steamWaterOut
//		byte 2-3 timer
//			2 bits timer units (0x00 seconds, 0x01 minutes, 0x10 hours, 0x11 days)
//			7 bits # of units
//			7 bits # decimals to units (e.g. if units is second, this is 1/10 of seconds, if is minutes, this is seconds etc)

//	Humidity sensor
//		byte 2-5 counter value (2 most significant / 5 less significant)

	response[1] = I2CCMD_ACK;
	response[0] = 2;
	initialized = true;
	return(2);
}

void ArduinoControl::setInitialized(bool flag)
{
	initialized = flag;
}

bool ArduinoControl::getInitialized()
{
	return initialized;
}
