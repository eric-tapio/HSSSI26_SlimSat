/**
 * @file sc_bus_db.h
 * @brief Spacecraft Bus Database Header
 * 
 * @details This header file defines the ScBusDb class which manages the
 * spacecraft bus database including system mode, timing information,
 * and data streaming state. It provides centralized storage and access
 * to critical spacecraft bus operational parameters.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef SC_BUS_DB_HEADER
#define SC_BUS_DB_HEADER

#include <Arduino.h>
#include <sc_bus_time.h>

#define MIN_SC_BUS_MODE_VALUE 0
#define MAX_SC_BUS_MODE_VALUE 2

#define VERBOSE_SC_BUS_DB_OUTPUT 0

/**
 * @brief Spacecraft Bus Operating Modes
 * @details Enumeration defining the possible operational states of the spacecraft bus
 */
enum ScBusMode
{
	INITIALIZED,   // System initialized but not fully operational (value: 0)
	SAFE,          // Safe mode with limited functionality (value: 1)
	NOMINAL_ON     // Normal operational mode (value: 2)
};


/**
 * @brief Spacecraft Bus Database Class
 * 
 * @details The ScBusDb class manages the central database for spacecraft
 * bus operations. It maintains system mode state, timing information, and data
 * streaming configuration. This class provides controlled access to critical
 * operational parameters and ensures data integrity through validation.
 */
class ScBusDb {

private:
	// Data Members 
	uint8_t stream_data_mode;
public:
	// Data Member Objects
	ScBusMode Bus_mode;
	ScBusTime Sc_Time;
	
	// Constructors
	/**
	 * @brief Default constructor for ScBusDb
	 * @details Initializes the spacecraft bus database with default values.
	 * Sets bus mode to INITIALIZED and stream data mode to off.
	 */
	ScBusDb(void);
	
	// Methods
	/**
	 * @brief Print spacecraft bus database information
	 * @details Outputs current database state including bus mode and
	 * stream data mode for debugging and diagnostic purposes.
	 */
	void print(void) const;
	
	/**
	 * @brief Get current spacecraft bus mode
	 * @details Returns the current operational mode of the spacecraft bus
	 * as an unsigned 8-bit integer value.
	 * @return Current bus mode (0=INITIALIZED, 1=SAFE, 2=NOMINAL_ON)
	 */
	uint8_t getScBusMode(void) const;
	
	/**
	 * @brief Set spacecraft bus mode
	 * @details Sets the operational mode of the spacecraft bus with validation.
	 * Only accepts values within the defined range (0-2).
	 * @param mode New bus mode value to set
	 */
	void setScBusMode(uint8_t mode);
	
	/**
	 * @brief Set spacecraft bus stream data state
	 * @details Configures the data streaming mode for the spacecraft bus.
	 * Controls whether the bus streams data continuously or on-demand.
	 * @param state_value Stream data state (0=off, 1=on)
	 */
	void setScBusStreamDataState(uint8_t state_value);
	
	/**
	 * @brief Get spacecraft bus stream data state
	 * @details Returns the current data streaming mode state.
	 * @return Stream data state (0=off, 1=on)
	 */
	uint8_t getScBusStreamDataState(void) const;
};


#endif