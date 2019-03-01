/*
 * I2CComm.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: osvaldo
 */

#include <I2CComm.h>

ArduinoControl * arCtrl;



void clearChannel()
{
	debug("Clearing channel...");
	while( Wire.available())
	{
		Wire.read();
	}
}

void receiveEvent(int howMany)
{
	char msgOut[32];
	uint8_t localBuf[COMM_BUF_SIZE];
	uint8_t bytesRead = (uint8_t) Wire.readBytes(localBuf, howMany);

	uint8_t writeIdxBck = arCtrl->writeIdx;
	sprintf(msgOut, "New msg (l %d, r %d, w %d)", howMany, arCtrl->readIdx, arCtrl->writeIdx);
	debug(msgOut);
	for(int i = 0; i < bytesRead; i++)
	{
		if (arCtrl->writeIdx == COMM_BUF_SIZE - 1) arCtrl->writeIdx = 0;
		arCtrl->commandBuf[arCtrl->writeIdx] = localBuf[i];
		debug_c(arCtrl->toHex(localBuf[i]));
		arCtrl->writeIdx++;
		if (arCtrl->writeIdx == arCtrl->readIdx)
		{
			arCtrl->writeIdx = writeIdxBck;
			arCtrl->responseBuf[0] = 0x02;
			arCtrl->responseBuf[1] = I2CCMD_NACK;
			arCtrl->responseLen = 2;
			return;
		}
	}
	sprintf(msgOut, " (r %d, w %d)", arCtrl->readIdx, arCtrl->writeIdx);
	debug(msgOut);
	return;
}

void sendEvent()
{
	char msgOut[16];
	if (arCtrl->responseLen == 0)
	{
		debug("Answer not ready");
		uint8_t buffer[2];
		buffer[0] = 2;
		buffer[1] = I2CCMD_NOT_READY;
		Wire.write(buffer, 2);
	}
	else
	{
		debug("Response pending:");
		for(int y = 0; y < arCtrl->responseLen; y++)
		{
			if (isprint(arCtrl->responseBuf[y]))
			{
				sprintf(msgOut, "%c ", arCtrl->responseBuf[y]);
				debug_c(msgOut);
			}
			else
			{
				debug_c(arCtrl->toHex(arCtrl->responseBuf[y]));
			}
		}
		debug("");
		Wire.write(arCtrl->responseBuf, arCtrl->responseLen);
		memset(arCtrl->responseBuf, '\0', arCtrl->responseLen);
		arCtrl->responseLen = 0;
	}
}


I2CComm::I2CComm(ArduinoControl * control)
{
	char msgOut[48];
	arCtrl = control;
	Wire.begin(I2C_BUS_ADDRESS);
	sprintf(msgOut, "On the i2c bus with address %d", I2C_BUS_ADDRESS);
	debug(msgOut);
	clearChannel();
}

void I2CComm::setCallback()
{
	Wire.onReceive(receiveEvent);
	Wire.onRequest(sendEvent);
}

I2CComm::~I2CComm() {
}
