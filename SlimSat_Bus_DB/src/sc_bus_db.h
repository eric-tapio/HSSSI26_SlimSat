// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SC_BUS_DB_HEADER
#define SC_BUS_DB_HEADER

#include <Arduino.h>
#include <sc_bus_time.h>

#define MIN_SC_BUS_MODE_VALUE 0
#define MAX_SC_BUS_MODE_VALUE 2

#define VERBOSE_SC_BUS_DB_OUTPUT 0

enum ScBusMode
{
	INITIALIZED,   // Default value is 0
	SAFE,          // Default value is 1
	NOMINAL_ON     // Default value is 2
};


class ScBusDb {

private:
	// Data Members 
	uint8_t stream_data_mode;
public:
	// Data Member Objects
	ScBusMode Bus_mode;
	ScBusTime Sc_Time;
	
	// Constructors
	ScBusDb(void);
	
	// Methods
	void print(void) const;
	uint8_t getScBusMode(void) const;
	void setScBusMode(uint8_t mode);
	void setScBusStreamDataState(uint8_t state_value);
	uint8_t getScBusStreamDataState(void) const;
};


#endif