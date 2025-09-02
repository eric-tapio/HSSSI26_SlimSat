// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SC_BUS_TIME_HEADER
#define SC_BUS_TIME_HEADER

#include <Arduino.h>

#define VERBOSE_SC_BUS_TIME_OUTPUT 0

class ScBusTime {
private:
	// Data Members
	uint32_t epoch_time_in_ms;
	uint32_t sc_bus_time_in_ms;
	uint32_t current_millis_in_ms;
	uint32_t millis_at_epoch_in_ms;

	// Methods
	void getMillis(void);

public:
	// Constructors
	ScBusTime(void);
	ScBusTime(uint32_t epoch_time);
	ScBusTime(uint32_t epoch_time, uint32_t millis_at_epoch_time);

	// Methods
	uint32_t getScBusTimeInMs(void) const;
	uint32_t getScBusEpochTimeInMs(void) const;
	void setScBusEpochTimeInMs(uint32_t epoch_time);
	void print(void) const;
};

#endif
