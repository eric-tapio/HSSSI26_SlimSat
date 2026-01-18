/**
 * @file bus_time.h
 * @brief Bus Time Management Header
 * 
 * @details This header file defines the BusTime class which manages
 * timing operations for the bus system. It handles epoch time
 * management, current time tracking, and time synchronization operations.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef BUS_TIME_HEADER
#define BUS_TIME_HEADER

#include <Arduino.h>

#define VERBOSE_BUS_TIME_OUTPUT 0


/**
 * @brief Bus Time Management Class
 * 
 * @details The BusTime class provides comprehensive time management
 * functionality for the bus system. It maintains epoch time
 * references, tracks current system time, and provides time conversion
 * and synchronization capabilities. This class is still under development.
 */
class BusTime {
private:
	// Data Members
	uint32_t epoch_time_in_ms; // Epoch time reference in milliseconds
	uint32_t bus_time_in_ms; // Current bus time in milliseconds
	uint32_t current_millis_in_ms; // Current system millis() value
	uint32_t millis_at_epoch_in_ms; // System millis() value at epoch time

	// Methods
	/**
	 * @brief Get current millis count from microcontroller
	 * @details Updates the bus_time_in_ms with current system uptime
	 */
	void getMillis(void);

public:
	// Constructors
	/**
	 * @brief Default constructor for BusTime
	 * @details Initializes all time values to zero
	 */
	BusTime(void);

	/**
	 * @brief Constructor with epoch time parameter
	 * @details Initializes the time system with a specific epoch time
	 * @param epoch_time Initial epoch time in milliseconds
	 */
	BusTime(uint32_t epoch_time);

	/**
	 * @brief Constructor with epoch time and millis at epoch
	 * @details Initializes with both epoch time and corresponding system millis
	 * @param epoch_time Initial epoch time in milliseconds
	 * @param millis_at_epoch_time System millis() value at epoch time
	 */
	BusTime(uint32_t epoch_time, uint32_t millis_at_epoch_time);
	
	// Methods
	/**
	 * @brief Get current bus time
	 * @details Returns the current bus time in milliseconds
	 * @return Current bus time in milliseconds
	 */
	uint32_t getBusTimeInMs(void) const;

	/**
	 * @brief Get bus epoch time
	 * @details Returns the epoch time reference in milliseconds
	 * @return Epoch time in milliseconds
	 */
	uint32_t getBusEpochTimeInMs(void) const;

	/**
	 * @brief Set bus epoch time
	 * @details Sets the epoch time reference for the bus
	 * @param epoch_time New epoch time in milliseconds
	 */
	void setBusEpochTimeInMs(uint32_t epoch_time);

	/**
	 * @brief Print time system information
	 * @details Outputs all time-related data members for debugging
	 */
	void print(void) const;
	
	void collectBusData(void);
		
};

#endif
