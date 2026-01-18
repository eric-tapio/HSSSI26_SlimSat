//
//  Bus Data Record
//
// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef BUS_DATA_HEADER
#define BUS_DATA_HEADER

#include <Arduino.h>

#define NUM_BUS_DATA_FLOAT_ELEMENTS 14 // This value is not to be changed unless the size of the Bus record changes


class BusDataRec {
public:
	uint32_t time;
	uint16_t bus_rec_number;
	float temp_C;
	float thermistor_V;
	float shunt_voltage_mV_1;
	float bus_voltage_V_1;
	float load_voltage_V_1;
	float current_mA_1;
	//float power_mW_1;
	float shunt_voltage_mV_2;
	float bus_voltage_V_2;
	float load_voltage_V_2;
	float current_mA_2;
	//float power_mW_2;
	float shunt_voltage_mV_3;
	float bus_voltage_V_3;
	float load_voltage_V_3;
	float current_mA_3;
	//float power_mW_3;
	
	BusDataRec(void);
	void printRecord(void);
	uint8_t getRecordSize(void);
};

#endif