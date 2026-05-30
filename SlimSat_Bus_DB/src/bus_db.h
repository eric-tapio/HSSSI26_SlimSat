/**
 * @file bus_db.h
 * @brief Bus Database Header
 * 
 * @details This header file defines the BusDb class which manages the
 * bus database including system mode, timing information,
 * and data streaming state. It provides centralized storage and access
 * to critical bus operational parameters.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef BUS_DB_HEADER
#define BUS_DB_HEADER

#include <Arduino.h>
#include <bus_time.h>
#include <bus_power_monitor.h>
#include <bus_flash_memory_if.h>


#define MIN_BUS_MODE_VALUE 0
#define MAX_BUS_MODE_VALUE 2
#define DEFAULT_SAFE_MODE_VOLTAGE_IN_V 2.85
#define NUM_MV_PER_V 1000

#define VERBOSE_BUS_DB_OUTPUT 0


/**
 * @brief Bus Operating Modes
 * @details Enumeration defining the possible operational states of the bus
 */
enum BusMode
{
	INITIALIZED,   // System initialized but not fully operational (value: 0)
	SAFE,          // Safe mode with limited functionality (value: 1)
	NOMINAL_ON     // Normal operational mode (value: 2)
};


/**
 * @brief Bus Database Class
 * 
 * @details The BusDb class manages the central database for bus
 * operations. It maintains system mode state, timing information, and data
 * streaming configuration. This class provides controlled access to critical
 * operational parameters and ensures data integrity through validation.
 */
class BusDb {

private:
	// Data Members
	float safe_mode_voltage_in_v;

public:
	// Data Member Objects
	BusMode Bus_Mode; // Bus operating mode
	BusTime Bus_Time; // Bus timing system object
	BusPowerMonitor Bus_Power_Monitor; // Note: The power monitor must be started so that power monitor devices are started
	FlashMemoryIf Flash_Memory;
	
	
	// Constructors
	/**
	 * @brief Default constructor for BusDb
	 * @details Initializes the bus database with default values.
	 * Sets bus mode to INITIALIZED and stream data mode to off.
	 */
	BusDb(void);
	
	// Methods
	/**
	 * @brief Print bus database information
	 * @details Outputs current database state including bus mode and
	 * stream data mode for debugging and diagnostic purposes.
	 */
	void print(void) const;
	
	/**
	 * @brief Get current bus mode
	 * @details Returns the current operational mode of the bus
	 * as an unsigned 8-bit integer value.
	 * @return Current bus mode (0=INITIALIZED, 1=SAFE, 2=NOMINAL_ON)
	 */
	uint8_t getBusMode(void) const;

	/**
	 * @brief Set bus mode
	 * @details Sets the operational mode of the bus with validation.
	 * Only accepts values within the defined range (0-2).
	 * @param mode New bus mode value to set
	 */
	void setBusMode(uint8_t mode);
	
	/**
	 * @brief Get bus safe mode voltage in millivolts
	 * @details Returns the voltage threshold for safe mode operation
	 * @return Safe mode voltage threshold in millivolts
	 */
	uint32_t getBusSafeModeVoltageInMv(void) const;
	
	/**
	 * @brief Set bus safe mode voltage in millivolts
	 * @details Sets the voltage threshold for safe mode operation
	 * @param sm_voltage_in_mv Safe mode voltage threshold in millivolts
	 */
	void setBusSafeModeVoltageInMv(uint32_t sm_voltage_in_mv);
	
	/**
	 * @brief Start power monitor
	 * @details Initializes and starts the bus power monitoring system
	 * @return Status code (0=success, non-zero=error)
	 */
	uint8_t startPowerMonitor(void);
};


#endif