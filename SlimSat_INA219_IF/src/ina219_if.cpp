/**
 * @file ina219_if.cpp
 * @brief INA219 Interface Implementation
 * 
 * @details This file implements the Ina219If class methods for interfacing
 * with INA219 current/power monitoring sensors. Provides device initialization,
 * calibration, measurement collection, and data processing capabilities
 * for power monitoring throughout the SlimSat system.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <ina219_if.h>

/**
 * @brief Default constructor for INA219 Interface
 * @details Initializes the INA219 interface with the default I2C address
 * using the member initializer list for the INA219 object.
 */
Ina219If::Ina219If(void) : ina219(INA219_I2C_ADR) {
}


Ina219If::Ina219If(uint8_t adr) : ina219(adr) {
	address = adr;
	initializeDataMembers();
}


uint8_t Ina219If::initializeDevice(void) {

	// Automatically start the device
	begin();
	setCalibration_32V_1A();
	success();
	
	return return_status;
}


void Ina219If::initializeDataMembers(void) {
	// This function initializes object data members
	shunt_voltage_mV = 0.0;
	bus_voltage_V = 0.0;
	load_voltage_V = 0.0;
	current_mA = 0.0;
	power_mW = 0.0;
	return_status = 0;

	return;
}


uint8_t Ina219If::begin(void) {

	return_status = ina219.begin();
	
	if (VERBOSE_INA219_OUTPUT) {
		Serial.print(F(" ~ INA219 Begin Return Status: "));
		Serial.println(return_status);
	}
	
	return return_status;
}


uint8_t Ina219If::success(void) {

	return_status = ina219.success();
	
	if (VERBOSE_INA219_OUTPUT) {
		Serial.print(F(" ~ INA219 Success Return Status: "));
		Serial.println(return_status);
	}
	
	return return_status;
}


float Ina219If::getCurrent(void) {
	current_mA = ina219.getCurrent_mA();
	return current_mA;
}


float Ina219If::getShuntVoltage(void) {
	shunt_voltage_mV = ina219.getShuntVoltage_mV();
	return shunt_voltage_mV;
}


float Ina219If::getBusVoltage(void) {
	bus_voltage_V = ina219.getBusVoltage_V();
	return bus_voltage_V;
}


float Ina219If::getLoadVoltage(void) {

	bus_voltage_V = ina219.getBusVoltage_V();
	shunt_voltage_mV = ina219.getShuntVoltage_mV();

	load_voltage_V = bus_voltage_V + (shunt_voltage_mV / NUM_MILLVOLTS_PER_VOLT);

	return load_voltage_V;
}


float Ina219If::getPower(void) {
	power_mW = ina219.getPower_mW();
	return power_mW;
}


void Ina219If::setCalibration_32V_1A(void) {
	ina219.setCalibration_32V_1A();
	return;
}


void Ina219If::print(void) const {
	Serial.println(F(" ~ Printing INA219 Voltage Current Sensor Interface Data Members ..."));
	Serial.print(F("     Address: "));
	Serial.println(address, HEX);
	Serial.print(F("     Return Status: "));
	Serial.println(return_status);
	Serial.print(F("     Shunt Voltage in mV: "));
	Serial.println(shunt_voltage_mV);
	Serial.print(F("     Bus Voltage in V: "));
	Serial.println(bus_voltage_V);
	Serial.print(F("     Load Voltage in V: "));
	Serial.println(load_voltage_V);
	Serial.print(F("     Current in mA: "));
	Serial.println(current_mA);
	Serial.print(F("     Power in mV: "));
	Serial.println(power_mW);
	Serial.println(F(""));
	
    return;
}