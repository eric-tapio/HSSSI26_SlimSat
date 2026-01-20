/**
 * @file sc_bus_db.cpp
 * @brief Spacecraft Bus Database Implementation
 * 
 * @details This file implements the ScBusDb class methods for managing
 * spacecraft bus database operations including mode management, timing
 * coordination, and data streaming control.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <sc_bus_db.h>

/**
 * @brief Default constructor for the Spacecraft Bus Database class
 * @details Initializes the spacecraft bus database with default operational
 * parameters. Sets the bus mode to INITIALIZED state and disables data
 * streaming mode for safe startup configuration.
 */
ScBusDb::ScBusDb(void) {
	// This is the default constructor for the S/C Bus Database class
	
	Bus_mode = INITIALIZED;
	stream_data_mode = 0;

	return;
}


/**
 * @brief Get current spacecraft bus mode
 * @details Returns the current operational mode of the spacecraft bus.
 * Provides optional verbose output for debugging purposes when enabled.
 * @return Current bus mode as uint8_t (0=INITIALIZED, 1=SAFE, 2=NOMINAL_ON)
 */
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


/**
 * @brief Set spacecraft bus mode
 * @details Sets the operational mode of the spacecraft bus with input validation.
 * Only accepts values within the defined range (MIN_SC_BUS_MODE_VALUE to
 * MAX_SC_BUS_MODE_VALUE). Invalid values are ignored with optional warning output.
 * @param mode New bus mode value to set (0=INITIALIZED, 1=SAFE, 2=NOMINAL_ON)
 */
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


/**
 * @brief Print spacecraft bus database information
 * @details Outputs current database state including bus mode and stream data mode.
 * Provides comprehensive diagnostic information for debugging and system monitoring.
 */
void ScBusDb::print(void) const {
	// This method prints the S/C Bus class data members
	
	Serial.println(F("\n ~ Printing S/C Bus Object ..."));
	Serial.print(F("   Bus Mode is: "));
	Serial.println(Bus_mode);
	Serial.print(F("   stream_data_mode is: "));
	Serial.println(stream_data_mode);

	return;
}


/**
 * @brief Set spacecraft bus stream data state
 * @details Configures the data streaming mode for the spacecraft bus.
 * Controls whether the bus streams data continuously or on-demand.
 * Non-zero values enable streaming, zero disables it.
 * @param state_value Stream data state (0=off, non-zero=on)
 */
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


/**
 * @brief Get spacecraft bus stream data state
 * @details Returns the current data streaming mode state with optional
 * verbose output for debugging purposes when enabled.
 * @return Stream data state (0=off, 1=on)
 */
uint8_t ScBusDb::getScBusStreamDataState(void) const {
	// This method gets the SC Bus Stream Data State
	if (VERBOSE_SC_BUS_DB_OUTPUT) {
		Serial.println(F("\n ~ Getting S/C Bus Stream Data State ... "));
		Serial.print(F(" ~ S/C Bus Stream State is: "));
		Serial.println(stream_data_mode);
	}

	return stream_data_mode;
}