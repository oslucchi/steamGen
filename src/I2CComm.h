/*
 * I2CComm.h
 *
 *  Created on: Feb 10, 2019
 *      Author: osvaldo
 */

#ifndef I2CCOMM_H_
#define I2CCOMM_H_
#include "Arduino.h"

#include <Wire.h>
#include <ArduinoControl.h>

#define I2C_BUS_ADDRESS	7

class I2CComm {
private:

public:
  	I2CComm(ArduinoControl *);
	void setCallback();
	virtual ~I2CComm();
};

#endif /* I2CCOMM_H_ */
