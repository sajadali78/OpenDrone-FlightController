/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#include "FlightController.h"
#include "Sensor/HCSR04.h"
#include "Sensor/AbstractSensor/Ultrasonic.h"
#include "Sensor/AbstractSensor/Gyroscope.h"
#include "Sensor/AbstractSensor/Accelerometer.h"
#include "Sensor/AbstractSensor/Barometer.h"
#include "Sensor/AbstractSensor/Magnetometer.h"
#include "Sensor/BMP180.h"
#include "Sensor/MPU6050.h"
#include "Sensor/HMC5883L.h"
#include "Motor/PWMMotorTest.h"
#include "Network/TCPServer.h"
#include "Network/Modbus.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <pthread.h>
#include <wiringPi.h>
using namespace std;

TCPServer tcpReceive, tcpSend;
PWMMotorTest* pw = new PWMMotorTest();

FlightController::FlightController()
{
}

static void *runUltrasonic(void *interval)
{
	//Initializing the wiringPi-Gpio's
	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		cout << "Failed to wiringPiSetupGpio()\n";
		exit(1);
	}

	//Initialize the sensors
	int len = 2;
	Ultrasonic* sensors[len];
	sensors[0] = new HCSR04(17, 27, 1);
	sensors[1] = new HCSR04(17, 27, 1);

	//Infinite loop to keep measuring --> TODO: Need to be changed
	double curDistance;
	while (1)
	{
		for (int i = 0; i < len; i++)
		{
			curDistance = sensors[i]->distance();
			cout << "Ultrasonic " << sensors[i]->getId() << ": Distance: " << curDistance << "\n";
			delay(2);
		}
		delay((int)interval);
	}
}

static void *runGyroAccelerometer(void *interval)
{
	//Initializing the sensor
	Gyroscope *gyro = new MPU6050();
	Accelerometer *acc = new MPU6050();

	//Infinite loop to keep measuring --> TODO: Need to be changed
	while (1)
	{
		double *valuesGyro = gyro->getGyroscopeValues();
		double *valuesAcc = acc->getAccelerometerValues();

		//cout << "Gyroscope x=" << valuesGyro[1] << " y=" << valuesGyro[2] << " z=" << valuesGyro[3] << "\n";
		cout << "Acc x=" << valuesAcc[1] << " y=" << valuesAcc[2] << " z=" << valuesAcc[3] << "\n";

		double pitch = 180 * atan(valuesAcc[1] / sqrt(valuesAcc[2] * valuesAcc[2] + valuesAcc[3] * valuesAcc[3])) / M_PI;
		double roll = 180 * atan(valuesAcc[2] / sqrt(valuesAcc[1] * valuesAcc[1] + valuesAcc[3] * valuesAcc[3])) / M_PI;
		cout << "Pitch: " << pitch << " Roll: " << roll << "\n";

		//myfile << values[0] << ";" << values[1] << ";" << values[2] << ";" << values[3] << ";" << values[4] << ";" << values[5] << ";" << values[6] << ";\n";
		delay((int)interval);
	}
}

static void *runBarometer(void *interval)
{
	//Initializing the sensor
	Barometer *barometer = new BMP180();

	//Infinite loop to keep measuring --> TODO: Need to be changed
	while (1)
	{
		int *values = barometer->getBarometerValues();
		cout << "Barometer Temperature: " << values[0] << " Pressure: " << values[1] << "\n";
		delay((int)interval);
	}
}

static void *runMagnetometer(void *interval)
{
	//Initializing the sensor
	Magnetometer *magnetometer = new HMC5883L();

	//Infinite loop to keep measuring --> TODO: Need to be changed
	while (1)
	{
		double *values = magnetometer->getMagnetometerValues();
		cout << "Magnet x=" << values[0] << " y=" << values[1] << " z=" << values[2] << "\n";
		delay((int)interval);
	}
}

int FlightController::run()
{
	//Creating the threads
	int len = 1;
	pthread_t threadIds[len];
    pthread_t msgReceive, msgSend;
	int threads[len];

	/*//Starts the Thread with the threadId in arg[0], arg[2] is the method, which is called by the thread, arg[3] the arguments of the method
	threads[0] = pthread_create(&threadIds[0], NULL, runUltrasonic, (void *)500);
	threads[1] = pthread_create(&threadIds[1], NULL, runGyroAccelerometer, (void *)500);
	threads[2] = pthread_create(&threadIds[2], NULL, runBarometer, (void *)500);
	threads[3] = pthread_create(&threadIds[3], NULL, runMagnetometer, (void *)500);

	//Checking if there was a problem with creating the Threads
	for (int i = 0; i < len; i++)
	{
		if (threads[i] != 0)
		{
			cout << "Error while creating the Thread " << i << " Error Code " << threads[i];
			return (1);
		}
	}
    */
    TCPServer *server = new TCPServer();
    server->startUp();
    /*
	//Waits until the threads are interrupted
	pthread_join(threadIds[0], (void**)1);
	pthread_join(threadIds[1], (void**)1);
	pthread_join(threadIds[2], (void**)1);
	pthread_join(threadIds[3], (void**)1);*/

	
	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		cout << "Failed to wiringPiSetupGpio()\n";
		exit(1);
	}

	//pw->ArmMotor(); //Start Motors !!ONLY one time!!
	//delay(5000);

	return (0);
}

FlightController::~FlightController()
{
}
