/**
 * @file sc_bus_time.h
 * @brief Spacecraft Bus Time Management Header
 * 
 * @details This header file defines the ScBusTime class which provides
 * time management functionality for the SlimSat spacecraft bus system.
 * It handles epoch time tracking, system uptime monitoring, and time
 * synchronization operations for mission timing requirements.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef SC_BUS_TIME_HEADER
#define SC_BUS_TIME_HEADER

#include <Arduino.h>

#define VERBOSE_SC_BUS_TIME_OUTPUT 0

/**
 * @brief Spacecraft Bus Time Management Class
 * 
 * @details The ScBusTime class provides comprehensive time management
 * functionality for the spacecraft bus system. It tracks epoch time,
 * system uptime, and provides time synchronization capabilities.
 * This class is still under development and may be expanded with
 * additional timing features as mission requirements evolve.
 */
class ScBusTime {
private:
	// Data Members
	uint32_t epoch_time_in_ms;
	uint32_t sc_bus_time_in_ms;
	uint32_t current_millis_in_ms;
	uint32_t millis_at_epoch_in_ms;

	// Methods
	/**
	 * @brief Get current milliseconds from system
	 * @details Retrieves the current millis count from the microcontroller
	 * which represents the system uptime since boot.
	 */
	void getMillis(void);

public:
	// Constructors
	/**
	 * @brief Default constructor for ScBusTime
	 * @details Initializes all time values to zero for default state
	 */
	ScBusTime(void);
	
	/**
	 * @brief Constructor with epoch time parameter
	 * @details Initializes the time system with a specific epoch time
	 * @param epoch_time Initial epoch time in milliseconds
	 */
	ScBusTime(uint32_t epoch_time);
	
	/**
	 * @brief Constructor with epoch time and millis at epoch
	 * @details Initializes the time system with epoch time and corresponding millis value
	 * @param epoch_time Initial epoch time in milliseconds
	 * @param millis_at_epoch_time Millis value when epoch was set
	 */
	ScBusTime(uint32_t epoch_time, uint32_t millis_at_epoch_time);

	// Methods
	/**
	 * @brief Get spacecraft bus time in milliseconds
	 * @details Returns the current spacecraft bus time with optional verbose output
	 * @return Current spacecraft bus time in milliseconds
	 */
	uint32_t getScBusTimeInMs(void) const;
	
	/**
	 * @brief Get spacecraft bus epoch time in milliseconds
	 * @details Returns the stored epoch time reference value
	 * @return Epoch time in milliseconds
	 */
	uint32_t getScBusEpochTimeInMs(void) const;
	
	/**
	 * @brief Set spacecraft bus epoch time in milliseconds
	 * @details Sets the epoch time reference for time calculations
	 * @param epoch_time New epoch time value in milliseconds
	 */
	void setScBusEpochTimeInMs(uint32_t epoch_time);
	
	/**
	 * @brief Print spacecraft bus time information
	 * @details Outputs all time-related data members for debugging and monitoring
	 */
	void print(void) const;
};

#endif
