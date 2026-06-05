/**
 * @file bus_db.cpp
 * @brief Bus Database Implementation
 * 
 * @details This file implements the BusDb class methods for managing
 * bus database operations including mode management, timing
 * coordination, and data streaming control.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <bus_db.h>

/**
 * @brief Default constructor for the Bus Database class
 * @details Initializes the bus database with default operational
 * parameters. Sets the bus mode to INITIALIZED state and disables data
 * streaming mode for safe startup configuration.
 */
BusDb::BusDb(void) {
	// This is the default constructor for the Bus Database class
	
	Bus_Mode = INITIALIZED;
	safe_mode_voltage_in_v = DEFAULT_SAFE_MODE_VOLTAGE_IN_V;
	
	return;
}


/**
 * @brief Get current bus mode
 * @details Returns the current operational mode of the bus.
 * Provides optional verbose output for debugging purposes when enabled.
 * @return Current bus mode as uint8_t (0=INITIALIZED, 1=SAFE, 2=NOMINAL_ON)
 */
uint8_t BusDb::getBusMode(void) const {
	// This method returns the  Bus State
	if (VERBOSE_BUS_DB_OUTPUT) {
		Serial.println(F("\n ~ Getting Bus State ..."));
		Serial.print(F(" ~ Bus Mode is: "));
		Serial.println(Bus_Mode);
	}

	return uint8_t(Bus_Mode);
}


/**
 * @brief Set bus mode
 * @details Sets the operational mode of the bus with input validation.
 * Only accepts values within the defined range (MIN_BUS_MODE_VALUE to
 * MAX_BUS_MODE_VALUE). Invalid values are ignored with optional warning output.
 * @param mode New bus mode value to set (0=INITIALIZED, 1=SAFE, 2=NOMINAL_ON)
 */
void BusDb::setBusMode(uint8_t mode) {
	// This method sets the Bus Mode

	if ((MIN_BUS_MODE_VALUE <= mode) && (mode <= MAX_BUS_MODE_VALUE)) {
		// Set the mode value

		if (VERBOSE_BUS_DB_OUTPUT) {
			Serial.print(F("\n ~ Setting Validated Bus Mode to the input value: "));
			Serial.println(mode);
		}
		
		Bus_Mode = BusMode(mode);
	}
	else {
		if (VERBOSE_BUS_DB_OUTPUT) {
			Serial.println(F(" ~ Ignoring Invalid Bus Mode input value "));
		}
	}

	return;
}


/**
 * @brief Print bus database information
 * @details Outputs current database state including bus mode and stream data mode.
 * Provides comprehensive diagnostic information for debugging and system monitoring.
 */
void BusDb::print(void) const {
	// This method prints the Bus class data members
	
	Serial.println(F("\n ~ Printing Bus Object ..."));
	Serial.print(F("   Bus Mode is: "));
	Serial.println(Bus_Mode);
	Serial.print(F("   Safe Mode Voltage in Volts is: "));
	Serial.println(safe_mode_voltage_in_v);	

	return;
}


/**
 * @brief Get bus safe mode voltage in millivolts
 * @details Returns the voltage threshold for safe mode operation with
 * optional verbose output for debugging purposes when enabled.
 * @return Safe mode voltage threshold in millivolts
 */
uint32_t BusDb::getBusSafeModeVoltageInMv(void) const {
	// This method gets the Safe Mode Voltage level
	if (VERBOSE_BUS_DB_OUTPUT) {
		Serial.println(F("\n ~ Getting Safe Mode Voltage level ..."));
	}
	
	return uint32_t(safe_mode_voltage_in_v * NUM_MV_PER_V);
}


/**
 * @brief Set bus safe mode voltage in millivolts
 * @details Sets the voltage threshold for safe mode operation with
 * conversion from millivolts to volts for internal storage.
 * @param sm_voltage_in_mv Safe mode voltage threshold in millivolts
 */
void BusDb::setBusSafeModeVoltageInMv(uint32_t sm_voltage_in_mv) {
	// This method sets the Safe Mode Voltage level
	if (VERBOSE_BUS_DB_OUTPUT) {
		Serial.println(F("\n ~ Setting Safe Mode Voltage level ..."));
	}
	
	safe_mode_voltage_in_v = float(sm_voltage_in_mv) / NUM_MV_PER_V;
	
	return;
}


/**
 * @brief Start power monitor
 * @details Initializes and starts the bus power monitoring system.
 * Returns the status code from the power monitor initialization.
 * @return Status code (0=success, non-zero=error)
 */
uint8_t BusDb::startPowerMonitor(void) {
	// This method starts the Power Monitor
	if (VERBOSE_BUS_DB_OUTPUT) {
		Serial.println(F("\n ~ Starting Power Monitor ..."));
	}
	
	uint8_t return_result = Bus_Power_Monitor.start();
	
	if (0) {
		Serial.print(F(" ~ Return Result is: "));
		Serial.println(return_result);	
	}
	
	return return_result;
}