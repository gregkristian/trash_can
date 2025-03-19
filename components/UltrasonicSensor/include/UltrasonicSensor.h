#ifndef UltrasonicSensor_H
#define UltrasonicSensor_H
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <inttypes.h>

#define DEFAULT_DELAY 10
#define DEFAULT_PINGS 5
class UltrasonicSensor {
public:
	/**
	* Constructor
	* UltrasonicSensor measure echo time to obstacle and return way. 
	* It has 4 connections pins: VCC, ECHO, TRIGGER, GND
	* triggerPin generates trigger signal from UltrasonicSensor
	* echoPin is the digital INPUT-Pin for measuring distance
	*/
	UltrasonicSensor(gpio_num_t echoPin, gpio_num_t triggerPin);

	// Do a measurment for this sensor. Return distance in cm
	long measureDistance();
	
private:
	gpio_num_t echoPin, triggerPin;

	// Sensor measure time, so conversion is needed to cm
	long convertMicrosecToCm(long duration);
};
#endif