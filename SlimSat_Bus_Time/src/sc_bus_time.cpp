// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "sc_bus_time.h"

// Note: This class is still under development

ScBusTime::ScBusTime(void) {
	// This is the default constructor for the S/C Bus Time class

	epoch_time_in_ms = 0;
	sc_bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = 0;
}


ScBusTime::ScBusTime(uint32_t epoch_time) {
	// This is the epoch time input specific constructor for the S/C Bus Time class

	epoch_time_in_ms = epoch_time;
	sc_bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = 0;
}


ScBusTime::ScBusTime(uint32_t epoch_time, uint32_t millis_at_epoch_time) {
	// This is the epoch time and millis at epoch time input specific constructor for the S/C Bus Time class

	epoch_time_in_ms = epoch_time;
	sc_bus_time_in_ms = 0;
	current_millis_in_ms = 0;
	millis_at_epoch_in_ms = millis_at_epoch_time;
}


void ScBusTime::getMillis(void) {
	// This method gets the current millis count from the micro-controller, which represents the up-time for the system 

	if (VERBOSE_SC_BUS_TIME_OUTPUT) {
		Serial.println(F("\n ~ Getting Millis ..."));
	}
	
	sc_bus_time_in_ms = uint32_t(millis());

	return;
}


uint32_t ScBusTime::getScBusEpochTimeInMs(void) const {
	return epoch_time_in_ms;
}


void ScBusTime::setScBusEpochTimeInMs(uint32_t epoch_time) {
	// This method sets the S/C Bus epoch time to the given epoch time

	if (VERBOSE_SC_BUS_TIME_OUTPUT) {
		Serial.println(F("\n ~ Setting Time with Epoch Time input ..."));
	}
	
	epoch_time_in_ms = epoch_time;

	return;
}


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