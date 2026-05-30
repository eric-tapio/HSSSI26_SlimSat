//
// Bus Power Monitor.h
//
// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef BUS_POWER_MONITOR_HEADER
#define BUS_POWER_MONITOR_HEADER

#include <Arduino.h>
#include <ina219_if.h>
#include <tmp75a_if.h>
#include <bus_flash_memory_if.h>
#include <bus_data.h>
#include <bus_pin_definitions.h>

#define NUM_INA219_DEVICES 3
#define NUM_CURRENT_ITERATIONS 5

#define MV_PER_LSB 3600.0F/1024.0F
#define V_PER_LSB 3.6F/1024.0F

#define RAISE_PM_HW_START_ERRORS 1
#define HW_DEVICE_IS_PRESENT 1

#define VERBOSE_POWER_MONITOR_OUTPUT 0


class BusPowerMonitor {
private:
	Ina219If* ina219_ary_ptr[NUM_INA219_DEVICES];
	Ina219If ina219_1;
	Ina219If ina219_2;
	Ina219If ina219_3;
	Tmp75aIf tmp75a_1;
	
	//ScBusData sc_bus_data;
	float shunt_voltage_mV;
	float bus_voltage_V;
	float load_voltage_V;
	float current_mA;
	float power_mW;
	uint8_t return_status;

public:
	BusPowerMonitor(void);
	BusPowerMonitor(uint8_t ina_1_adr, uint8_t ina_2_adr, uint8_t ina_3_adr);
	BusPowerMonitor(uint8_t ina_1_adr, uint8_t ina_2_adr, uint8_t ina_3_adr, uint8_t tmp_adr);
	uint8_t begin(uint8_t ina219_index);
	uint8_t success(uint8_t ina219_index);
	uint8_t start(void);
	float getCurrent(uint8_t ina219_index);
	float getShuntVoltage(uint8_t ina219_index);
	float getBusVoltage(uint8_t ina219_index);
	float getLoadVoltage(uint8_t ina219_index);
	float getPower(uint8_t ina219_index);
	void setCalibration_32V_1A(void);
	void print(void) const;
	float readThermistor(void);
	void collectBusData(BusDataRec& Bus_data);
	float getShuntVoltage1(void);
	float getTemperature(void);
	
	void rebootBus(void);
	void initiateCutdown(void);

private:
	void initializeDataMembers(void);
};

#endif


