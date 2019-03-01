/*
 * CommandHandler.h
 *
 *  Created on: Feb 15, 2019
 *      Author: osvaldo
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_
#include "Command.h"

#define  I2CCMD_ACK (uint8_t)0x00
#define  I2CCMD_NACK (uint8_t)0xFF
#define  I2CCMD_NOT_READY	(uint8_t)0xFE

typedef struct _cmdList {
	Command *cmd;
	struct _cmdList* next;
} cmdList;

class CommandHandler {
	cmdList controllerRoot;
	cmdList *current;
	TimerManager *timerManager;

public:
	CommandHandler();
	virtual ~CommandHandler();
	void addCommand(unsigned char ctrlId, byte ctrlType, uint8_t* pin, uint8_t* upperBound,
					uint8_t *lowerBound, uint8_t* actualValue, TimerManager *tm);
	cmdList *getController();
};

#endif /* COMMANDHANDLER_H_ */
