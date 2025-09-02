// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SLIMSAT_SIMPLE_TIMER_HEADER
#define SLIMSAT_SIMPLE_TIMER_HEADER

#include <Arduino.h>

#define SIMPLE_TIMER_VERBOSE_OUTPUT 0
#define DEFAULT_INTERVAL_IN_MS 2000L

// Max interval in ms is 20 minutes
#define MAX_INTERVAL_IN_MS 1200000L

class SimpleTimer {
private:
	// Data Members
	uint8_t timer_enabled;
	uint32_t elapse_time_in_ms;
	uint32_t interval_in_ms;
	  
	// Methods
	void initializeTimer(void);
	void updateElapseTime(void);

public:
	// Constructors 
	SimpleTimer(void);
	SimpleTimer(uint32_t interval_time_in_ms);
	  
	// Methods
	void enableTimer(void);
	void disableTimer(void);
	uint8_t isEnabled(void) const;
	uint8_t timerHasElapsed(void);
	uint32_t getIntervalInMs(void) const;
	uint8_t setIntervalInMs(uint32_t interval_time_in_ms);
	void enableAndStartTimer(void);
	void print(void) const;
};

#endif