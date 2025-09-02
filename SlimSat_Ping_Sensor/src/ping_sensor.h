// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef PING_SENSOR_HEADER
#define PING_SENSOR_HEADER

// This is the ping sensor class
// The majority of the code is simply the Arduino Ping Sensor example code refactored into a class

#include <Arduino.h> // Arduino header file must be included to get the uintx_t types

#define DEFAULT_PING_PIN 7

#define PING_VERBOSE_OUTPUT 1

class Ping_Sensor {
private:
	// Data Members
	uint8_t ping_pin;
	uint32_t duration;
	uint32_t range;
	
	// Methods
	void getPulseDuration(void);
	uint32_t microsecondsToInches(void) const;
	uint32_t microsecondsToCentimeters(void) const;
	
public:
	// Constructors
	Ping_Sensor(void);
	Ping_Sensor(uint8_t ping_pin);
	
	// Methods
	void print(void) const;
	uint32_t getRangeMeasurementInCm(void);
	uint32_t getRangeMeasurementInIn(void);
	void setPingPin(uint8_t pin);
};

#endif