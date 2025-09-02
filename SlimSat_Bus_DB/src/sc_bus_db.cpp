// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <sc_bus_db.h>


ScBusDb::ScBusDb(void) {
	// This is the default constructor for the S/C Bus Database class
	
	Bus_mode = INITIALIZED;
	stream_data_mode = 0;

	return;
}


uint8_t ScBusDb::getScBusMode(void) const {
	// This method returns the  SC Bus State
	if (VERBOSE_SC_BUS_DB_OUTPUT) {
		Serial.println(F("\n ~ Getting S/C Bus State ... "));
		Serial.print(F(" ~ Bus Mode is: "));
		Serial.println(Bus_mode);
	}

	//return static_cast<uint8_t>(Bus_mode);
	return uint8_t(Bus_mode);
}


void ScBusDb::setScBusMode(uint8_t mode) {
	// This function sets the S/C Bus Mode

	if ((MIN_SC_BUS_MODE_VALUE <= mode) && (mode <= MAX_SC_BUS_MODE_VALUE)) {
		// Set the mode value

		if (VERBOSE_SC_BUS_DB_OUTPUT) {
			Serial.print(F("\n ~ Setting Validated S/C Bus Mode to the input value: "));
			Serial.println(mode);
		}
		
		Bus_mode = ScBusMode(mode);
	}
	else {
		if (VERBOSE_SC_BUS_DB_OUTPUT) {
			Serial.println(F(" ~ Ignoring Invalid S/C Bus Mode input value "));
		}
	}

	return;
}


void ScBusDb::print(void) const {
	// This method prints the S/C Bus class data members
	
	Serial.println(F("\n ~ Printing S/C Bus Object ..."));
	Serial.print(F("   Bus Mode is: "));
	Serial.println(Bus_mode);
	Serial.print(F("   stream_data_mode is: "));
	Serial.println(stream_data_mode);

	return;
}


void ScBusDb::setScBusStreamDataState(uint8_t state_value) {
	// This method sets the SC Bus Stream Data state
	if (VERBOSE_SC_BUS_DB_OUTPUT) {
		Serial.println(F("\n ~ Setting S/C Bus Stream Data State ... "));
		Serial.print(F("\n   state_value is: "));
		Serial.println(state_value);
	}

	if (state_value == 0) {
		stream_data_mode = 0;
	}
	else {
		stream_data_mode = 1;
	}

	return;
}


uint8_t ScBusDb::getScBusStreamDataState(void) const {
	// This method gets the SC Bus Stream Data State
	if (VERBOSE_SC_BUS_DB_OUTPUT) {
		Serial.println(F("\n ~ Getting S/C Bus Stream Data State ... "));
		Serial.print(F(" ~ S/C Bus Stream State is: "));
		Serial.println(stream_data_mode);
	}

	return stream_data_mode;
}