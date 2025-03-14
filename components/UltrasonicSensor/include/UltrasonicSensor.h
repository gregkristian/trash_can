#ifndef UltrasonicSensor_H
#define UltrasonicSensor_H
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <inttypes.h>

//#define PULSE_TIMEOUT 150000L	// 100ms
#define DEFAULT_DELAY 10
#define DEFAULT_PINGS 5
class UltrasonicSensor {
public:
	
	/**
	* Constructor
	* Ultrasonic sensor has 4 connections pins: VCC, ECHO, TRIGGER, GND
	* 
	* \param echoPin digital INPUT-Pin for measuring distance
	* \param triggerPin if 10us high a trigger signal is generated from 
	*					UltrasonicSensor
	*
	* \return void
	*/
	UltrasonicSensor(gpio_num_t echoPin, gpio_num_t triggerPin);

	/**
	* Do a measurment for this sensor. Return distance as long
	* in centimenter
	* \return long distance in centimeter
	*/
	long Distance();
	
private:
	/**
	* Do the measurement calculation and return result in centimeter
	* UltrasonicSensor measure echo time to obstacle and return way. 
	* Sound travels with 340m/sec
	* Example: Obstace 100cm away from UltrasonicSensor. Measure time is 100cm to
	* obstacle and 100cm return = 200cm
	* <br>
	* 1sec = 1000ms = 1.000.000uS
	* 1.000.000 / 340 = Distance in microseconds for 100cm
	* 2941uS fuer 100cm = 5882 uS fuer 200cm
	*
	* duration / 5882 * 100 = distance in cm
	*/	
	long MicrosecondsToCentimeter(long duration);
	
	long _currentDistance;
	gpio_num_t _echoPin, _triggerPin;
	long _duration, _distance;
	bool _autoMode;
};
#endif



