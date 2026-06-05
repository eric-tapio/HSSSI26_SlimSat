/**
 * @file bus_data.cpp
 * @brief Bus Data Record Implementation
 * 
 * @details This file implements the BusDataRec class methods for managing
 * bus telemetry data including initialization, printing, and size calculation.
 * The class provides structured storage for temperature, voltage, current,
 * and power measurements from multiple sensors throughout the bus system.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <bus_data.h>

/**
 * @brief Default constructor for the Bus Data Record class
 * @details Initializes all data members with default test values for
 * system validation and testing purposes. These values will be replaced
 * with actual sensor readings during normal operation.
 */
BusDataRec::BusDataRec(void) {
	// Default Constructor
	bus_rec_number = 0; // This value gets populated by the bus before being written to flash memory
	time = 0;
	temp_C = 0.0;
	thermistor_V = 0.0;
	shunt_voltage_mV_1 = 0.0;
	bus_voltage_V_1 = 0.0;
	load_voltage_V_1 = 0.0;
	current_mA_1 = 0.0;
	//power_mW_1 = 0.0;
	shunt_voltage_mV_2 = 0.0;
	bus_voltage_V_2 = 0.0;
	load_voltage_V_2 = 0.0;
	current_mA_2 = 0.0;
	//power_mW_2 = 0.0;
	shunt_voltage_mV_3 = 0.0;
	bus_voltage_V_3 = 0.0;
	load_voltage_V_3 = 0.0;
	current_mA_3 = 0.0;
	//power_mW_3 = 0.0;
}


/**
 * @brief Print bus data record
 * @details Outputs all data members including timestamps, sensor readings,
 * and calculated values for debugging and monitoring purposes.
 */
void BusDataRec::printRecord(void) { 
	Serial.println(F("\n ~ Printing Bus Data Record ..."));
	
	Serial.print(F("    Number Float Data Elements is: "));
	Serial.println(NUM_BUS_DATA_FLOAT_ELEMENTS);
	Serial.print(F("    Bus Data Record Size in Bytes is: "));
	Serial.println(getRecordSize());	
		
	Serial.print(F("     Bus Rec Number: "));
	Serial.println(bus_rec_number);
	Serial.print(F("     Time: "));
	Serial.println(time);
	Serial.print(F("     Temp in deg C: "));
	Serial.println(temp_C);
	Serial.print(F("     thermistor_V: "));
	Serial.println(thermistor_V);
	Serial.print(F("     Shunt Voltage in mV 1: "));
	Serial.println(shunt_voltage_mV_1);
	Serial.print(F("     Bus Voltage in V 1: "));
	Serial.println(bus_voltage_V_1);
	Serial.print(F("     Load Voltage in V 1: "));
	Serial.println(load_voltage_V_1);
	Serial.print(F("     Current in mA 1: "));
	Serial.println(current_mA_1);
	//Serial.print(F("     Power in mV: "));
	//Serial.println(power_mW);
	Serial.print(F("     Shunt Voltage in mV 2: "));
	Serial.println(shunt_voltage_mV_2);
	Serial.print(F("     Bus Voltage in V 2: "));
	Serial.println(bus_voltage_V_2);
	Serial.print(F("     Load Voltage in V 2: "));
	Serial.println(load_voltage_V_2);
	Serial.print(F("     Current in mA 2: "));
	Serial.println(current_mA_2);
	//Serial.print(F("     Power in mV: "));
	//Serial.println(power_mW);
	Serial.print(F("     Shunt Voltage in mV 3: "));
	Serial.println(shunt_voltage_mV_3);
	Serial.print(F("     Bus Voltage in V 3: "));
	Serial.println(bus_voltage_V_3);
	Serial.print(F("     Load Voltage in V 3: "));
	Serial.println(load_voltage_V_3);
	Serial.print(F("     Current in mA 3: "));
	Serial.println(current_mA_3);
	//Serial.print(F("     Power in mV: "));
	//Serial.println(power_mW);
	
	Serial.println(F(""));
	
	return;
}


/**
 * @brief Get record size in bytes
 * @details Computes the total size of a Bus Data Record including all
 * float elements, timestamp, and record number for memory allocation.
 * @return Total size of the record in bytes
 */
uint8_t BusDataRec::getRecordSize(void) {
	// This method computes the size of a Bus Data Record
	return NUM_BUS_DATA_FLOAT_ELEMENTS * sizeof(float) + sizeof(uint32_t) + sizeof(uint16_t);
}