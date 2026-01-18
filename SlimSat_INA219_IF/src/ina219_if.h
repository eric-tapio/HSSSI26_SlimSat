// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SLIMSAT_INA219_IF_HEADER
#define SLIMSAT_INA219_IF_HEADER

#include <Arduino.h>
#include <Adafruit_INA219.h>

#define INA219_I2C_ADR 0x40
#define NUM_MILLVOLTS_PER_VOLT 1000

#define VERBOSE_INA219_OUTPUT 0

class Ina219If {
private:
	Adafruit_INA219 ina219;
	uint16_t address;
	float shunt_voltage_mV;
	float bus_voltage_V;
	float load_voltage_V;
	float current_mA;
	float power_mW;
	uint8_t return_status;

public:
	Ina219If(void);
	Ina219If(uint8_t adr);
	uint8_t begin(void);
	uint8_t success(void);
	uint8_t initializeDevice(void);
	float getCurrent(void);
	float getShuntVoltage(void);
	float getBusVoltage(void);
	float getLoadVoltage(void);
	float getPower(void);
	void setCalibration_32V_1A(void);
	void print(void) const;  

private:
	void initializeDataMembers(void);
};

#endif


