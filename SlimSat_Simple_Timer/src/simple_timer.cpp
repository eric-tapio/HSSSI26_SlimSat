/**
 * @file simple_timer.cpp
 * @brief Simple Timer Management Implementation
 * 
 * @details This file implements the SimpleTimer class methods for providing
 * basic timer functionality throughout the SlimSat system. Includes interval
 * management, enable/disable control, and elapsed time checking capabilities.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <simple_timer.h>

/**
 * @brief Default constructor for the SimpleTimer class
 * @details Initializes the timer with default settings when no input
 * arguments are provided. Sets up timer in disabled state with default interval.
 */
SimpleTimer::SimpleTimer(void) { 
	// This is the default constructor for the SimpleTimer class when no input arguments are provided
	initializeTimer();
}


/**
 * @brief Constructor with interval parameter
 * @details Initializes the timer with a specified interval time in milliseconds.
 * Sets timer to disabled state with the provided interval value.
 * @param interval_time_in_ms Timer interval in milliseconds
 */
SimpleTimer::SimpleTimer(uint32_t interval_time_in_ms) { 
	// This is the constructor used for the SimpleTimer class when an interval time argument is provided
  
	timer_enabled = 0;
	interval_in_ms = interval_time_in_ms;
	elapse_time_in_ms = interval_time_in_ms;
}


/**
 * @brief Initialize timer to default values
 * @details Sets the timer to disabled state with default interval (2000ms)
 * and initializes the elapse time to the default interval value.
 */
void SimpleTimer::initializeTimer(void) {
	// This method initializes the timer to default values
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Initializing Timer ..."));
	}

	timer_enabled = 0;
	interval_in_ms = DEFAULT_INTERVAL_IN_MS;
	elapse_time_in_ms = DEFAULT_INTERVAL_IN_MS;

  return;
}


/**
 * @brief Print timer information
 * @details Outputs comprehensive timer state including enable status,
 * interval, and elapse time for debugging and diagnostic purposes.
 */
void SimpleTimer::print(void) const {
	// This method prints the Simple Timer data members and their values
	Serial.println(F("\n ~ Printing Simple Timer"));
	Serial.print(F("    Timer Enabled is: "));
	Serial.println(timer_enabled);
	Serial.print(F("    Timer Interval in ms is: "));
	Serial.println(interval_in_ms);
	Serial.print(F("    Timer Elapse Time in ms is: "));
	Serial.println(elapse_time_in_ms);
	Serial.println(F(""));

	return;
}


/**
 * @brief Enable the timer
 * @details Sets the timer to enabled state, allowing it to track elapsed time.
 * Provides optional verbose output for debugging.
 */
void SimpleTimer::enableTimer(void) {
	// This method enables the timer
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Enabling Timer ..."));
	}
	  
	timer_enabled = 1;
	return;
}


/**
 * @brief Disable the timer
 * @details Sets the timer to disabled state, stopping elapsed time tracking.
 * Provides optional verbose output for debugging.
 */
void SimpleTimer::disableTimer(void) {
	// This method disables the timer
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Disabling Timer ..."));
	}
	  
	timer_enabled = 0;
	return;
}


/**
 * @brief Check if timer is enabled
 * @details Returns the current enable state of the timer with optional
 * verbose output for debugging purposes.
 * @return Timer enable state (0=disabled, 1=enabled)
 */
uint8_t SimpleTimer::isEnabled(void) const {
	// This method returns whether or not the timer is enabled
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Getting the Is Enabled state of the timer ..."));
	}
	  
	return timer_enabled;
}


/**
 * @brief Get timer interval
 * @details Returns the current timer interval in milliseconds with
 * optional verbose output for debugging.
 * @return Timer interval in milliseconds
 */
uint32_t SimpleTimer::getIntervalInMs(void) const {
	// This method returns the incremental interval used by the timer
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Getting the timer Interval in ms ..."));
	}
	  
	return interval_in_ms;
}


/**
 * @brief Set timer interval
 * @details Sets the timer interval with validation. Accepts values from
 * 0 to MAX_INTERVAL_IN_MS (20 minutes). Invalid values are rejected.
 * @param interval_time_in_ms New interval in milliseconds
 * @return Success status (0=success, 1=invalid interval)
 */
uint8_t SimpleTimer::setIntervalInMs(uint32_t interval_time_in_ms) {
	// This method sets the timer interval in milliseconds 
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Setting the timer Interval in ms ..."));
	}
	  
	if ((0 <= interval_time_in_ms) && (interval_time_in_ms <= MAX_INTERVAL_IN_MS)) {
		// Then the interval time is valid
		interval_in_ms = interval_time_in_ms;
		return 0;
	}
	else {
		return 1;
	}
}


/**
 * @brief Update timer elapse time
 * @details Calculates the next elapse time by adding the interval to
 * the current millis() value. This tracks the next time to act.
 */
void SimpleTimer::updateElapseTime(void) {
	// This method updates the timer elapse time 
	// Does this by keeping track of the next time to act
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Updating Timer Elapse Time ..."));
	}
	
	elapse_time_in_ms = millis() + interval_in_ms;

	return;
}


/**
 * @brief Check if timer has elapsed
 * @details Determines if the timer interval has elapsed by comparing current
 * millis() with the elapse time. If elapsed, updates the elapse time for
 * the next cycle and returns true. Only functions when timer is enabled.
 * @return Elapsed state (0=not elapsed, 1=elapsed)
 */
uint8_t SimpleTimer::timerHasElapsed(void) {
	// This method determines if the timer has elapsed or not
	if (timer_enabled) {
		if (millis() > elapse_time_in_ms) {
			if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
				Serial.println(F("\n ~ The timer has elapsed and it is time to take action!"));
			}
		  
		// Then update the timer elapse time, and return 1 to indicate that timerHasElapsed is true
		updateElapseTime();

		return 1;
		}
		else {
			// Else return 0 to indicate that timerHasElapsed is false
			return 0;
		}
	}
	else {
		// Timer is disabled
		return 0;
	}
}


/**
 * @brief Enable and start timer
 * @details Enables the timer for immediate operation. This method performs
 * the same function as enableTimer() and is provided for convenience.
 */
void SimpleTimer::enableAndStartTimer(void) {
	// This method enables and starts the timer. It performs the same function as enableTimer
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Enabling and Starting Timer ..."));
	}

	enableTimer();

	return;
}