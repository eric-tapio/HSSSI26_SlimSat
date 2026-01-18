/**
 * @file ping_sensor.h
 * @brief Ping Ultrasonic Sensor Header
 * 
 * @details This header file defines the Ping_Sensor class which provides
 * ultrasonic distance measurement capabilities using a Parallax PING sensor.
 * The majority of the code is Arduino Ping Sensor example code refactored
 * into a class structure for integration with the SlimSat system.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef PING_SENSOR_HEADER
#define PING_SENSOR_HEADER

// This is the ping sensor class
// The majority of the code is simply the Arduino Ping Sensor example code refactored into a class

#include <Arduino.h> // Arduino header file must be included to get the uintx_t types

// Put HW pin def into a common header file - TBD
//#define DEFAULT_PING_PIN 7
#define DEFAULT_PING_PIN A3

#define PING_VERBOSE_OUTPUT 0

/**
 * @brief Ping Ultrasonic Sensor Class
 * 
 * @details The Ping_Sensor class provides ultrasonic distance measurement
 * functionality using a Parallax PING sensor. It handles pulse generation,
 * echo timing, and distance calculations in both inches and centimeters.
 * The class manages the sensor pin configuration and timing operations.
 */
class Ping_Sensor {
private:
	// Data Members
	uint8_t ping_pin; // Digital pin number for PING sensor
	uint32_t duration; // Pulse duration in microseconds
	uint32_t range; // Calculated range measurement
	
	// Methods
	/**
	 * @brief Get pulse duration from PING sensor
	 * @details Triggers the sensor and measures echo pulse duration
	 */
	void getPulseDuration(void);

	/**
	 * @brief Convert microseconds to inches
	 * @details Converts pulse duration to distance in inches
	 * @return Distance in inches
	 */
	uint32_t microsecondsToInches(void) const;

	/**
	 * @brief Convert microseconds to centimeters
	 * @details Converts pulse duration to distance in centimeters
	 * @return Distance in centimeters
	 */
	uint32_t microsecondsToCentimeters(void) const;
	
public:
	// Constructors
	/**
	 * @brief Default constructor for Ping_Sensor
	 * @details Initializes sensor with default pin (pin 7)
	 */
	Ping_Sensor(void);

	/**
	 * @brief Constructor with pin parameter
	 * @details Initializes sensor with specified digital pin
	 * @param ping_pin Digital pin number for PING sensor
	 */
	Ping_Sensor(uint8_t ping_pin);
	
	// Methods
	/**
	 * @brief Print sensor information
	 * @details Outputs sensor configuration and last measurement data
	 */
	void print(void) const;

	/**
	 * @brief Get range measurement in centimeters
	 * @details Performs measurement and returns distance in centimeters
	 * @return Distance to object in centimeters
	 */
	uint32_t getRangeMeasurementInCm(void);

	/**
	 * @brief Get range measurement in inches
	 * @details Performs measurement and returns distance in inches
	 * @return Distance to object in inches
	 */
	uint32_t getRangeMeasurementInIn(void);

	/**
	 * @brief Set PING sensor pin
	 * @details Configures which digital pin to use for the sensor
	 * @param pin Digital pin number to use
	 */
	void setPingPin(uint8_t pin);
};

#endif