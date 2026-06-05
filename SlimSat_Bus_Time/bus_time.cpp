/**
 * @file bus_time.cpp
 * @brief Bus Time Management Implementation
 * 
 * @details This file implements the BusTime class methods for managing
 * timing operations within the bus system. Provides epoch time
 * management, current time tracking, and time synchronization capabilities.
 * 
 * Note: This class is still under development
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include "bus_time.h"


/**
 * @brief Default constructor for the Bus Time class
 * @details Initializes all time-related data members to zero values.
 * Sets up the time system in a clean initial state.
 */
BusTime::BusTime(void) {
	// This is the default constructor for the Bus Time class

	epoch_time_in_ms = 0;
	bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = 0;
}


/**
 * @brief Constructor with epoch time parameter
 * @details Initializes the time system with a specific epoch time value.
 * Other time values are set to zero for clean initialization.
 * @param epoch_time Initial epoch time in milliseconds
 */
BusTime::BusTime(uint32_t epoch_time) {
	// This is the epoch time input specific constructor for the Bus Time class

	epoch_time_in_ms = epoch_time;
	bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = 0;
}


/**
 * @brief Constructor with epoch time and millis at epoch
 * @details Initializes the time system with both epoch time and the
 * corresponding system millis() value for proper time synchronization.
 * @param epoch_time Initial epoch time in milliseconds
 * @param millis_at_epoch_time System millis() value at epoch time
 */
BusTime::BusTime(uint32_t epoch_time, uint32_t millis_at_epoch_time) {
	// This is the epoch time and millis at epoch time input specific constructor for the Bus Time class

	epoch_time_in_ms = epoch_time;
	bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = millis_at_epoch_time;
}


/**
 * @brief Get current millis count from microcontroller
 * @details Updates the bus_time_in_ms with the current system uptime
 * from the Arduino millis() function. This represents the system uptime
 * since power-on or reset.
 */
void BusTime::getMillis(void) {
	// This method gets the current millis count from the micro-controller, which represents the up-time for the system 

	if (VERBOSE_BUS_TIME_OUTPUT) {
		Serial.println(F("\n ~ Getting Millis ..."));
	}
	
	bus_time_in_ms = uint32_t(millis());

	return;
}


/**
 * @brief Get bus epoch time
 * @details Returns the current epoch time reference value in milliseconds.
 * @return Epoch time in milliseconds
 */
uint32_t BusTime::getBusEpochTimeInMs(void) const {
	return epoch_time_in_ms;
}


/**
 * @brief Set bus epoch time
 * @details Sets the epoch time reference for the bus system.
 * Updates the internal epoch time value with optional verbose output.
 * @param epoch_time New epoch time in milliseconds
 */
void BusTime::setBusEpochTimeInMs(uint32_t epoch_time) {
	// This method sets the Bus epoch time to the given epoch time

	if (VERBOSE_BUS_TIME_OUTPUT) {
		Serial.println(F("\n ~ Setting Time with Epoch Time input ..."));
	}
	
	epoch_time_in_ms = epoch_time;

	return;
}


/**
 * @brief Get current bus time
 * @details Returns the current bus time in milliseconds.
 * Provides optional verbose output showing the current time value.
 * @return Current bus time in milliseconds
 */
uint32_t BusTime::getBusTimeInMs(void) const {
	// This method gets the Bus time 
	
	if (VERBOSE_BUS_TIME_OUTPUT) {
		Serial.println(F(" ~ Getting Time ..."));;
		Serial.print(F("    Bus Time is: "));
		Serial.print(bus_time_in_ms);
		Serial.println(F(" ms"));
	}

	return bus_time_in_ms;
}


/**
 * @brief Print time system information
 * @details Outputs comprehensive time-related data including epoch time,
 * millis at epoch, current bus time, and current system millis for
 * debugging and diagnostic purposes.
 */
void BusTime::print(void) const {
	// This method prints the Bus Time class data members
	
	Serial.println(F("\n ~ Printing Time data ..."));

	Serial.print(F("     Epoch Time in ms is: ")); 
	Serial.println(epoch_time_in_ms);
	Serial.print(F("     Millis at Epoch Time in ms is: ")); 
	Serial.println(millis_at_epoch_in_ms);
	Serial.print(F("     Bus Time in ms is: ")); 
	Serial.println(bus_time_in_ms);
	Serial.print(F("     Current Millis in ms is: ")); 
	Serial.println(current_millis_in_ms);
	
	return;
}