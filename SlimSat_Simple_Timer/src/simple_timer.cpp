// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <simple_timer.h>


SimpleTimer::SimpleTimer(void) { 
	// This is the default constructor for the SimpleTimer class when no input arguments are provided
	initializeTimer();
}


SimpleTimer::SimpleTimer(uint32_t interval_time_in_ms) { 
	// This is the constructor used for the SimpleTimer class when an interval time argument is provided
  
	timer_enabled = 0;
	interval_in_ms = interval_time_in_ms;
	elapse_time_in_ms = interval_time_in_ms;
}


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


void SimpleTimer::enableTimer(void) {
	// This method enables the timer
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Enabling Timer ..."));
	}
	  
	timer_enabled = 1;
	return;
}


void SimpleTimer::disableTimer(void) {
	// This method disables the timer
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Disabling Timer ..."));
	}
	  
	timer_enabled = 0;
	return;
}


uint8_t SimpleTimer::isEnabled(void) const {
	// This method returns whether or not the timer is enabled
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Getting the Is Enabled state of the timer ..."));
	}
	  
	return timer_enabled;
}


uint32_t SimpleTimer::getIntervalInMs(void) const {
	// This method returns the incremental interval used by the timer
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Getting the timer Interval in ms ..."));
	}
	  
	return interval_in_ms;
}


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


void SimpleTimer::updateElapseTime(void) {
	// This method updates the timer elapse time 
	// Does this by keeping track of the next time to act
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Updating Timer Elapse Time ..."));
	}
	
	elapse_time_in_ms = millis() + interval_in_ms;

	return;
}


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


void SimpleTimer::enableAndStartTimer(void) {
	// This method enables and starts the timer. It performs the same function as enableTimer
	if (SIMPLE_TIMER_VERBOSE_OUTPUT) {
		Serial.println(F("\n ~ Enabling and Starting Timer ..."));
	}

	enableTimer();

	return;
}