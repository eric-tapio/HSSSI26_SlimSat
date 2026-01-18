/**
 * @file ping_sensor.cpp
 * @brief Ping Ultrasonic Sensor Implementation
 * 
 * @details This file implements the Ping_Sensor class methods for ultrasonic
 * distance measurement using a Parallax PING sensor. Provides pulse timing,
 * distance calculations, and pin management functionality.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <ping_sensor.h>

/**
 * @brief Default constructor for Ping Sensor
 * @details Initializes the sensor with default pin (pin 7) and clears
 * duration and range values. Called when no input arguments are given.
 */
Ping_Sensor::Ping_Sensor(void) {
	// This is the Ping Sensor default constructor, which is called when no input arguments are given
	duration = 0;
    range = 0;
	ping_pin = DEFAULT_PING_PIN;
}


/**
 * @brief Constructor with pin parameter
 * @details Initializes the sensor with a specified digital pin and clears
 * duration and range values. Called when a pin number is provided.
 * @param ping_hw_pin Digital pin number for PING sensor
 */
Ping_Sensor::Ping_Sensor(uint8_t ping_hw_pin) {
	// This is the Ping Sensor constructor, which is called when a Ping HW Pin Input is given
	duration = 0;
    range = 0;
	ping_pin = ping_hw_pin;
}


/**
 * @brief Print sensor information
 * @details Outputs comprehensive sensor data including pin configuration
 * and last measurement duration for debugging and diagnostic purposes.
 */
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

/**
 * @brief Get pulse duration from PING sensor
 * @details Triggers the PING sensor and measures the echo pulse duration.
 * The sensor is triggered by a HIGH pulse of 2+ microseconds, then measures
 * the time for the echo to return. Uses modified timing for reliable operation.
 */
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


/**
 * @brief Set PING sensor pin
 * @details Configures which digital pin to use for the PING sensor.
 * Allows runtime reconfiguration of the sensor pin assignment.
 * @param pin Digital pin number to use for the sensor
 */
void Ping_Sensor::setPingPin(uint8_t pin) {
	// This method enables the ping pin to be set by the user
	ping_pin = pin;
	
	return;
}


/**
 * @brief Get range measurement in centimeters
 * @details Performs a complete measurement cycle including pulse generation,
 * echo timing, and distance calculation. Returns distance in centimeters.
 * @return Distance to object in centimeters
 */
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


/**
 * @brief Get range measurement in inches
 * @details Performs a complete measurement cycle including pulse generation,
 * echo timing, and distance calculation. Returns distance in inches.
 * @return Distance to object in inches
 */
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


/**
 * @brief Convert microseconds to inches
 * @details Converts pulse duration to distance in inches using Parallax's
 * specification of 73.746 microseconds per inch (sound at 1130 ft/sec).
 * Divides by 2 since the pulse travels to object and back.
 * @return Distance in inches
 */
uint32_t Ping_Sensor::microsecondsToInches(void) const {
	// This method converts the measured duration to a range measurement in inches
	// According to Parallax's datasheet for the PING))), there are 73.746
	// microseconds per inch (i.e. sound travels at 1130 feet per second).
	// This gives the distance travelled by the ping, outbound and return,
	// so we divide by 2 to get the distance of the obstacle.
	// See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
	return duration / 74 / 2;
}

/**
 * @brief Convert microseconds to centimeters
 * @details Converts pulse duration to distance in centimeters using the
 * speed of sound (340 m/s or 29 microseconds per cm). Divides by 2
 * since the pulse travels to the object and back.
 * @return Distance in centimeters
 */
uint32_t Ping_Sensor::microsecondsToCentimeters(void) const {
	// This method converts the measured duration to a range measurement in cm
	// The speed of sound is 340 m/s or 29 microseconds per centimeter.
	// The ping travels out and back, so to find the distance of the object we
	// take half of the distance travelled.
	return duration / 29 / 2;
}