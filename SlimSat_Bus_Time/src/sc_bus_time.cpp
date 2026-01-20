/**
 * @file sc_bus_time.cpp
 * @brief Spacecraft Bus Time Management Implementation
 * 
 * @details This file implements the ScBusTime class methods for managing
 * spacecraft bus time operations including epoch time tracking, system
 * uptime monitoring, and time synchronization functionality.
 * 
 * Note: This class is still under development
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include "sc_bus_time.h"

/**
 * @brief Default constructor for the Spacecraft Bus Time class
 * @details Initializes all time-related data members to zero values
 * for a clean startup state. This provides a baseline for time tracking.
 */
ScBusTime::ScBusTime(void) {
	// This is the default constructor for the S/C Bus Time class

	epoch_time_in_ms = 0;
	sc_bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = 0;
}


/**
 * @brief Constructor with epoch time parameter
 * @details Initializes the spacecraft bus time system with a specific epoch time.
 * Other time values are set to zero for clean initialization.
 * @param epoch_time Initial epoch time value in milliseconds
 */
ScBusTime::ScBusTime(uint32_t epoch_time) {
	// This is the epoch time input specific constructor for the S/C Bus Time class

	epoch_time_in_ms = epoch_time;
	sc_bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = 0;
}


/**
 * @brief Constructor with epoch time and millis at epoch parameters
 * @details Initializes the spacecraft bus time system with both epoch time
 * and the corresponding millis value when the epoch was established.
 * @param epoch_time Initial epoch time value in milliseconds
 * @param millis_at_epoch_time Millis value when epoch time was set
 */
ScBusTime::ScBusTime(uint32_t epoch_time, uint32_t millis_at_epoch_time) {
	// This is the epoch time and millis at epoch time input specific constructor for the S/C Bus Time class

	epoch_time_in_ms = epoch_time;
	sc_bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = millis_at_epoch_time;
}


/**
 * @brief Get current milliseconds from microcontroller
 * @details Retrieves the current millis count from the microcontroller,
 * which represents the system uptime since boot. Updates the internal
 * spacecraft bus time with this value.
 */
void ScBusTime::getMillis(void) {
	// This method gets the current millis count from the micro-controller, which represents the up-time for the system 

	if (VERBOSE_SC_BUS_TIME_OUTPUT) {
		Serial.println(F("\n ~ Getting Millis ..."));
	}
	
	sc_bus_time_in_ms = uint32_t(millis());

	return;
}


/**
 * @brief Get spacecraft bus epoch time in milliseconds
 * @details Returns the stored epoch time reference value used for
 * time calculations and synchronization.
 * @return Epoch time in milliseconds
 */
uint32_t ScBusTime::getScBusEpochTimeInMs(void) const {
	return epoch_time_in_ms;
}


/**
 * @brief Set spacecraft bus epoch time in milliseconds
 * @details Sets the epoch time reference value used for time calculations.
 * Provides optional verbose output for debugging when enabled.
 * @param epoch_time New epoch time value in milliseconds
 */
void ScBusTime::setScBusEpochTimeInMs(uint32_t epoch_time) {
	// This method sets the S/C Bus epoch time to the given epoch time

	if (VERBOSE_SC_BUS_TIME_OUTPUT) {
		Serial.println(F("\n ~ Setting Time with Epoch Time input ..."));
	}
	
	epoch_time_in_ms = epoch_time;

	return;
}


/**
 * @brief Get spacecraft bus time in milliseconds
 * @details Returns the current spacecraft bus time with optional verbose
 * output for debugging and monitoring purposes when enabled.
 * @return Current spacecraft bus time in milliseconds
 */
uint32_t ScBusTime::getScBusTimeInMs(void) const {
	// This method gets the S/C Bus time 
	
	if (VERBOSE_SC_BUS_TIME_OUTPUT) {
		Serial.println(F(" ~ Getting Time ..."));;
		Serial.print(F("    SC Bus Time is: "));
		Serial.print(sc_bus_time_in_ms);
		Serial.println(F(" ms"));
	}

	return sc_bus_time_in_ms;
}


/**
 * @brief Print spacecraft bus time information
 * @details Outputs comprehensive time data including epoch time, millis at epoch,
 * current spacecraft bus time, and current millis for debugging and monitoring.
 */
void ScBusTime::print(void) const {
	// This method prints the S/C Bus Time class data members
	
	Serial.println(F("\n ~ Printing Time data ..."));

	Serial.print(F("     Epoch Time in ms is: ")); 
	Serial.println(epoch_time_in_ms);
	Serial.print(F("     Millis at Epoch Time in ms is: ")); 
	Serial.println(millis_at_epoch_in_ms);
	Serial.print(F("     SC Bus Time in ms is: ")); 
	Serial.println(sc_bus_time_in_ms);
	Serial.print(F("     Current Millis in ms is: ")); 
	Serial.println(current_millis_in_ms);
	
	return;
}