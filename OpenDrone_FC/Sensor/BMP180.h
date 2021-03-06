/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "AbstractSensor/Barometer.h"

#pragma once
class BMP180 : public virtual Barometer
{
public:
	BMP180();
	double *getBarometerValues();
	void runBarometer();
	void interruptBaromter();
	~BMP180();

private:
	bool run = false;
	double temperature = 0, pressure = 0;
	void calcBaromter();
};

