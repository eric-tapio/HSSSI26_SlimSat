/**
 * @file simple_timer.h
 * @brief Simple Timer Management Header
 * 
 * @details This header file defines the SimpleTimer class which provides
 * basic timer functionality for the SlimSat system. It supports configurable
 * intervals, enable/disable control, and elapsed time checking for various
 * timing operations throughout the bus system.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef SIMPLE_TIMER_HEADER
#define SIMPLE_TIMER_HEADER

#include <Arduino.h>

// Max interval in ms is 20 minutes
#define MAX_INTERVAL_IN_MS 1200000L
#define DEFAULT_INTERVAL_IN_MS 2000L
#define SIMPLE_TIMER_VERBOSE_OUTPUT 0

/**
 * @brief Simple Timer Class
 * 
 * @details The SimpleTimer class provides basic timer functionality with
 * configurable intervals and enable/disable control. It uses the Arduino
 * millis() function for timing and supports intervals up to 20 minutes.
 * The timer can be used for periodic operations, timeouts, and scheduling.
 */
class SimpleTimer {
private:
	// Data Members
	uint8_t timer_enabled; // Timer enable flag (0=disabled, 1=enabled)
	uint32_t elapse_time_in_ms; // Next elapse time in milliseconds
	uint32_t interval_in_ms; // Timer interval in milliseconds
	  
	// Methods
	/**
	 * @brief Initialize timer to default values
	 * @details Sets timer to disabled state with default interval
	 */
	void initializeTimer(void);

	/**
	 * @brief Update timer elapse time
	 * @details Calculates the next elapse time based on current millis and interval
	 */
	void updateElapseTime(void);

public:
	// Constructors 
	/**
	 * @brief Default constructor for SimpleTimer
	 * @details Initializes timer with default interval and disabled state
	 */
	SimpleTimer(void);

	/**
	 * @brief Constructor with interval parameter
	 * @details Initializes timer with specified interval in disabled state
	 * @param interval_time_in_ms Timer interval in milliseconds
	 */
	SimpleTimer(uint32_t interval_time_in_ms);
	  
	// Methods
	/**
	 * @brief Enable the timer
	 * @details Sets the timer to enabled state for operation
	 */
	void enableTimer(void);

	/**
	 * @brief Disable the timer
	 * @details Sets the timer to disabled state, stopping operation
	 */
	void disableTimer(void);

	/**
	 * @brief Check if timer is enabled
	 * @details Returns the current enable state of the timer
	 * @return Timer enable state (0=disabled, 1=enabled)
	 */
	uint8_t isEnabled(void) const;

	/**
	 * @brief Check if timer has elapsed
	 * @details Determines if the timer interval has elapsed and resets for next cycle
	 * @return Elapsed state (0=not elapsed, 1=elapsed)
	 */
	uint8_t timerHasElapsed(void);

	/**
	 * @brief Get timer interval
	 * @details Returns the current timer interval in milliseconds
	 * @return Timer interval in milliseconds
	 */
	uint32_t getIntervalInMs(void) const;

	/**
	 * @brief Set timer interval
	 * @details Sets the timer interval with validation (0 to MAX_INTERVAL_IN_MS)
	 * @param interval_time_in_ms New interval in milliseconds
	 * @return Success status (0=success, 1=invalid interval)
	 */
	uint8_t setIntervalInMs(uint32_t interval_time_in_ms);

	/**
	 * @brief Enable and start timer
	 * @details Enables the timer for immediate operation
	 */
	void enableAndStartTimer(void);

	/**
	 * @brief Print timer information
	 * @details Outputs timer state and configuration for debugging
	 */
	void print(void) const;
};

#endif