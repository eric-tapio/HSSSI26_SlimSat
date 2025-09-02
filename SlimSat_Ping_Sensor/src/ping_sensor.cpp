// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <ping_sensor.h>


Ping_Sensor::Ping_Sensor(void) {
	// This is the Ping Sensor default constructor, which is called when no input arguments are given
	duration = 0;
    range = 0;
	ping_pin = DEFAULT_PING_PIN;
}


Ping_Sensor::Ping_Sensor(uint8_t ping_hw_pin) {
	// This is the Ping Sensor constructor, which is called when a Ping HW Pin Input is given
	duration = 0;
    range = 0;
	ping_pin = ping_hw_pin;
}


void Ping_Sensor::print(void) const {
	// This method prints the Ping Sensor Class data members
    Serial.println(F("\n ~ Printing Ping Data Members:"));
    Serial.print(F("      Ping Pin is: "));
	Serial.println(ping_pin);
    Serial.print(F( "      Last Duration is: "));
	Serial.println(duration);
	Serial.println(F("")); 
    return;
}

void Ping_Sensor::getPulseDuration(void) {
	// This method gets the measured duration of the Ping sensor's ping
	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(ping_pin, OUTPUT);
	digitalWrite(ping_pin, LOW);
	// Note on using delaymicroseconds() - be aware that it has a range where it is accurate and it is between 3 and 16383 micro seconds
	//delayMicroseconds(2); //Original Value 
	delay(3); //This does work
	digitalWrite(ping_pin, HIGH);
	//delayMicroseconds(5); //Original Value
	delay(5);  // This does work
	digitalWrite(ping_pin, LOW);

	// The same pin is used to read the signal from the PING))): a HIGH pulse
	// whose duration is the time (in microseconds) from the sending of the ping
	// to the reception of its echo off of an object.
	pinMode(ping_pin, INPUT);
	duration = pulseIn(ping_pin, HIGH);
	  
	return;
}


void Ping_Sensor::setPingPin(uint8_t pin) {
	// This method enables the ping pin to be set by the user
	ping_pin = pin;
	
	return;
}


uint32_t Ping_Sensor::getRangeMeasurementInCm(void) {
	// This method returns the range measurement in cm
	getPulseDuration();
	range = microsecondsToCentimeters();

	if (PING_VERBOSE_OUTPUT) {
		Serial.print(F(" ~ Distance to object is: "));
		Serial.print(range);
		Serial.println(F(" cm"));
	}
	
    return range;
}


uint32_t Ping_Sensor::getRangeMeasurementInIn(void) {
	// This method returns the range measurement in in
	getPulseDuration();
	range = microsecondsToInches();

	if (PING_VERBOSE_OUTPUT) {
		Serial.print(F(" ~ Distance to object is: "));
		Serial.print(range);
		Serial.println(F(" in"));
	}
	
	return range;
}


uint32_t Ping_Sensor::microsecondsToInches(void) const {
	// This method converts the measured duration to a range measurement in inches
	// According to Parallax's datasheet for the PING))), there are 73.746
	// microseconds per inch (i.e. sound travels at 1130 feet per second).
	// This gives the distance travelled by the ping, outbound and return,
	// so we divide by 2 to get the distance of the obstacle.
	// See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
	return duration / 74 / 2;
}

uint32_t Ping_Sensor::microsecondsToCentimeters(void) const {
	// This method converts the measured duration to a range measurement in cm
	// The speed of sound is 340 m/s or 29 microseconds per centimeter.
	// The ping travels out and back, so to find the distance of the object we
	// take half of the distance travelled.
	return duration / 29 / 2;
}