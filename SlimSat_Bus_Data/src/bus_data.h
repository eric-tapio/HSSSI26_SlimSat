/**
 * @file bus_data.h
 * @brief Bus Data Record Header
 * 
 * @details This header file defines the BusDataRec class which provides
 * a structured data record for storing bus telemetry information including
 * temperature readings, voltage measurements, current monitoring, and
 * power consumption data from multiple sensors and monitoring systems.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef BUS_DATA_HEADER
#define BUS_DATA_HEADER

#include <Arduino.h>

#define NUM_BUS_DATA_FLOAT_ELEMENTS 14 // This value is not to be changed unless the size of the Bus record changes

/**
 * @brief Bus Data Record Class
 * 
 * @details The BusDataRec class provides a structured container for bus
 * telemetry data including timestamps, temperature measurements, voltage
 * readings, current monitoring, and power consumption from multiple sensors.
 * This class serves as the primary data structure for bus health monitoring
 * and telemetry collection throughout the SlimSat system.
 */
class BusDataRec {
public:
	// Data Members
	uint32_t time;                    ///< Timestamp for the data record
	uint16_t bus_rec_number;          ///< Sequential record number
	float temp_C;                     ///< Temperature in Celsius
	float thermistor_V;               ///< Thermistor voltage reading
	float shunt_voltage_mV_1;         ///< Shunt voltage sensor 1 in millivolts
	float bus_voltage_V_1;            ///< Bus voltage sensor 1 in volts
	float load_voltage_V_1;           ///< Load voltage sensor 1 in volts
	float current_mA_1;               ///< Current sensor 1 in milliamps
	//float power_mW_1;               ///< Power sensor 1 in milliwatts (unused)
	float shunt_voltage_mV_2;         ///< Shunt voltage sensor 2 in millivolts
	float bus_voltage_V_2;            ///< Bus voltage sensor 2 in volts
	float load_voltage_V_2;           ///< Load voltage sensor 2 in volts
	float current_mA_2;               ///< Current sensor 2 in milliamps
	//float power_mW_2;               ///< Power sensor 2 in milliwatts (unused)
	float shunt_voltage_mV_3;         ///< Shunt voltage sensor 3 in millivolts
	float bus_voltage_V_3;            ///< Bus voltage sensor 3 in volts
	float load_voltage_V_3;           ///< Load voltage sensor 3 in volts
	float current_mA_3;               ///< Current sensor 3 in milliamps
	//float power_mW_3;               ///< Power sensor 3 in milliwatts (unused)
	
	// Methods
	/**
	 * @brief Default constructor for BusDataRec
	 * @details Initializes all data members to default values
	 */
	BusDataRec(void);
	
	/**
	 * @brief Print bus data record
	 * @details Outputs all data members for debugging and monitoring
	 */
	void printRecord(void);
	
	/**
	 * @brief Get record size
	 * @details Returns the size of the data record structure
	 * @return Size of the record in bytes
	 */
	uint8_t getRecordSize(void);
};

#endif