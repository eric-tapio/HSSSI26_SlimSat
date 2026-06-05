/**
 * @file bus_power_monitor.cpp
 * @brief Bus Power Monitor Implementation
 * 
 * @details This file implements the BusPowerMonitor class methods for managing
 * multiple INA219 power monitoring sensors throughout the bus system. Provides
 * initialization, calibration, data collection, and power management
 * capabilities for comprehensive system power monitoring.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <bus_power_monitor.h>

/**
 * @brief Default constructor for Bus Power Monitor
 * @details Initializes the power monitor with default I2C addresses for
 * three INA219 sensors and sets up the pointer array for sensor management.
 */
BusPowerMonitor::BusPowerMonitor(void) : ina219_1(INA219_1_I2C_ADR), ina219_2(INA219_2_I2C_ADR), ina219_3(INA219_3_I2C_ADR), tmp75a_1(TMP75A_I2C_ADR) {
	// Default Constructor
	
	// Put a pointer to each INA219 into the pointer array
	ina219_ary_ptr[0] = &ina219_1;
	ina219_ary_ptr[1] = &ina219_2;
	ina219_ary_ptr[2] = &ina219_3;
	
	initializeDataMembers();
	
	return;
}


BusPowerMonitor::BusPowerMonitor(uint8_t ina_1_adr, uint8_t ina_2_adr, uint8_t ina_3_adr) : ina219_1(ina_1_adr), ina219_2(ina_2_adr), ina219_3(ina_3_adr) {
	// Input Specific Constructor for just INA I2C addresses
	// Put a pointer to each INA219 into the pointer array
	ina219_ary_ptr[0] = &ina219_1;
	ina219_ary_ptr[1] = &ina219_2;
	ina219_ary_ptr[2] = &ina219_3;
	
	// Initialize Power Monitor Data Members
	// INA219 data members get initialized during construction
	initializeDataMembers();
}


BusPowerMonitor::BusPowerMonitor(uint8_t ina_1_adr, uint8_t ina_2_adr, uint8_t ina_3_adr, uint8_t tmp_adr) : ina219_1(ina_1_adr), ina219_2(ina_2_adr), ina219_3(ina_3_adr), tmp75a_1(tmp_adr) {
	// Input Specific Constructor for both INA & TMP I2C addresses
	// Put a pointer to each INA219 into the pointer array
	ina219_ary_ptr[0] = &ina219_1;
	ina219_ary_ptr[1] = &ina219_2;
	ina219_ary_ptr[2] = &ina219_3;
	
	// Initialize Power Monitor Data Members
	// INA219 data members get initialized during construction
	initializeDataMembers();
}	


// Need to update and complete this prior to loading it on the SlimSat Prototype - TBD
uint8_t BusPowerMonitor::start(void) {
	// This method initializes the Bus Power Monitor
	
	uint8_t return_status_0 = 0;


	uint8_t return_status_1 = 0;
	uint8_t return_status_2 = 0;

	if (USING_SLIMSAT_MODULE_CONFIG == 0) {
		// Just 1 INA is present
		return_status_1 = HW_DEVICE_IS_PRESENT; // Simulate that this INA is present
		return_status_2 = HW_DEVICE_IS_PRESENT; // Simulate that this INA is present
	}
	
	uint8_t return_status_3 = 0;
	
	// Start the INAs
	return_status_0 = ina219_ary_ptr[0] -> begin();
	ina219_ary_ptr[0] -> setCalibration_32V_1A();
	return_status_0 = ina219_ary_ptr[0] -> success();
	
	if (USING_SLIMSAT_MODULE_CONFIG) {
		// // Uncomment these once they are present
		return_status_1 = ina219_ary_ptr[1] -> begin();
		ina219_ary_ptr[1] -> setCalibration_32V_1A();
		return_status_1 = ina219_ary_ptr[1] -> success();
		
		return_status_2 = ina219_ary_ptr[2] -> begin();
		ina219_ary_ptr[2] -> setCalibration_32V_1A();
		return_status_2 = ina219_ary_ptr[2] -> success();
	}
	
	// Start the tempeature sensor
	return_status_3 = tmp75a_1.begin();

	return_status = (return_status_0 && return_status_1 && return_status_2 && return_status_3);

	if (VERBOSE_POWER_MONITOR_OUTPUT) {
		if (return_status) {
			Serial.println(F(" ~ Bus Power Monitor successfully initialized\n"));
		}
		else {
			Serial.println(F(" ~ Bus Power Monitor initialization Failed!\n"));
			// if (RAISE_PM_HW_START_ERRORS) {
				// Serial.println(F(" ~ Error Encountered ..."));
				// while (RAISE_PM_HW_START_ERRORS) {
					// delay(10);  
				// }
			// }
		}
	}
	
	return return_status;
}


void BusPowerMonitor::initializeDataMembers(void) {
	// This method initializes power Monitor data members
	shunt_voltage_mV = 0.0;
	bus_voltage_V = 0.0;
	load_voltage_V = 0.0;
	current_mA = 0.0;
	power_mW = 0.0;
	return_status = 0;

	return;
}


uint8_t BusPowerMonitor::begin(uint8_t ina219_index) {
	// This method starts the INA219 device
	// Validate the input
	if ((0 <= ina219_index) && (ina219_index <= NUM_INA219_DEVICES)) {
		return_status = ina219_ary_ptr[ina219_index] -> begin();
		
		if (VERBOSE_POWER_MONITOR_OUTPUT) {
			Serial.print(F(" ~ INA219 Begin Return Status: "));
			Serial.println(return_status);
		}
		
		if (RAISE_PM_HW_START_ERRORS) {
			if (!HW_DEVICE_IS_PRESENT) {
				Serial.println(F(" ~ Error Encountered ..."));
				while (!HW_DEVICE_IS_PRESENT) {
					delay(10);  
				}
			}
		}
		
		return return_status;
	}
	else {
		return 0;
	}
}


uint8_t BusPowerMonitor::success(uint8_t ina219_index) {
	// This method gets the INA219 success state
	// Validate the input
	if ((0 <= ina219_index) && (ina219_index <= NUM_INA219_DEVICES)) {
		return_status = ina219_ary_ptr[ina219_index] -> success();
		
		if (VERBOSE_POWER_MONITOR_OUTPUT) {
			Serial.print(F(" ~ INA219 Success Return Status: "));
			Serial.println(return_status);
		}
		
		return return_status;
	}
	else {
		return 0;
	}
}


float BusPowerMonitor::getCurrent(uint8_t ina219_index) {
	// This method gets the INA219 measured Current in mA


	// Ask Nick about this - TBD
	// // Compute the average current
	// current_ma_total = 0;
	// for (int k = 0; k < NUM_CURRENT_ITERATIONS; k++) {  
    // current_ma = ina219_1.getCurrent_mA();
    // current_ma_total = current_ma_total + current_ma;
	// }

	// // Compute the average current - Though why multiply by 0.5? TBD
	// current_ma = 0.5 * (current_ma_total / NUM_CURRENT_ITERATIONS);
  	// Validate the input
	if ((0 <= ina219_index) && (ina219_index <= NUM_INA219_DEVICES)) {
		//current_mA = ina219_ary_ptr[ina219_index] -> getCurrent();

		float current_mA_total = 0;
		
		for (uint8_t i=0; i<NUM_CURRENT_ITERATIONS; i++) {  
			current_mA_total += ina219_ary_ptr[ina219_index] -> getCurrent();
		}

		// Compute the average current
		current_mA = current_mA_total / NUM_CURRENT_ITERATIONS;
		
		return current_mA;
	}
	else {
		return -1.0;
	}
}


float BusPowerMonitor::getShuntVoltage(uint8_t ina219_index) {
	// This method gets the INA219 measured Shunt Voltage in V
	// Validate the input
	if ((0 <= ina219_index) && (ina219_index <= NUM_INA219_DEVICES)) {
		
		shunt_voltage_mV = ina219_ary_ptr[ina219_index] -> getShuntVoltage();
		
		return shunt_voltage_mV;
	}
	else {
		return -1.0;
	}
}


float BusPowerMonitor::getBusVoltage(uint8_t ina219_index) {
	// This method gets the INA219 measured Bus Voltage in V
	// Validate the input
	if ((0 <= ina219_index) && (ina219_index <= NUM_INA219_DEVICES)) {
		bus_voltage_V = ina219_ary_ptr[ina219_index] -> getBusVoltage();

		return bus_voltage_V;
	}
	else {
		return -1.0;
	}
}


float BusPowerMonitor::getLoadVoltage(uint8_t ina219_index) {
	// This method gets the INA219 measured Load Voltage in V
	// Validate the input
	if ((0 <= ina219_index) && (ina219_index <= NUM_INA219_DEVICES)) {
		load_voltage_V = ina219_ary_ptr[ina219_index] -> getLoadVoltage();

		return load_voltage_V;
	}
	else {
		return -1.0;
	}
}


float BusPowerMonitor::getPower(uint8_t ina219_index) {
	// This method gets the INA219 measured Power in mW

	// Validate the input
	if ((0 <= ina219_index) && (ina219_index <= NUM_INA219_DEVICES)) {
		power_mW = ina219_ary_ptr[ina219_index] -> getPower();

		return power_mW;
	}
	else {
		return -1.0;
	}
}


void BusPowerMonitor::setCalibration_32V_1A(void) {
	// This method sets the INA219 for Calibration 32V 1A
	if (VERBOSE_POWER_MONITOR_OUTPUT) {
		Serial.println(F(" ~ Setting Calibration to 32V 1A ..."));
	}
	
	for (uint8_t i=0; i<NUM_INA219_DEVICES; i++) {
		ina219_ary_ptr[NUM_INA219_DEVICES] -> setCalibration_32V_1A();
	}
	
	return;
}


void BusPowerMonitor::print(void) const {
	// This method prints the Bus Power Monitor data members
	
	for (uint8_t i=0; i<NUM_INA219_DEVICES; i++) {
		Serial.print(F(" ~ Printing Bus Power Monitor Data Members for INA219 "));
		Serial.println(i);
		
		ina219_ary_ptr[i] -> print();
	}
		
    return;
}


float BusPowerMonitor::getTemperature(void) {
	// Note the temperature sensor needs to be started
	return tmp75a_1.getTemperatureInDegC();
}


void BusPowerMonitor::collectBusData(BusDataRec& Bus_data) { 
	
	if (VERBOSE_POWER_MONITOR_OUTPUT) {
		Serial.println(F(" ~ Collecting Bus Data ..."));
	}

	Bus_data.bus_rec_number = 0; // Note: This value gets populated by the Bus outside this method
	Bus_data.time = 0; // Note: This value gets populated by the Bus outside this method
		
	Bus_data.temp_C = getTemperature();
	Bus_data.thermistor_V = readThermistor();

	Bus_data.shunt_voltage_mV_1 = getShuntVoltage(0);	
	Bus_data.bus_voltage_V_1 = getBusVoltage(0);
	Bus_data.load_voltage_V_1 = getLoadVoltage(0);
	Bus_data.current_mA_1 = getCurrent(0);

	#if (USING_SLIMSAT_MODULE_CONFIG == 1)
		// All 3 INAs are present
		Bus_data.shunt_voltage_mV_2 = getShuntVoltage(1);
		Bus_data.bus_voltage_V_2 = getBusVoltage(1);
		Bus_data.load_voltage_V_2 = getLoadVoltage(1);
		Bus_data.current_mA_1 = getCurrent(1);
		//Bus_data.power_mW_1 = getPower(1);
		
		Bus_data.shunt_voltage_mV_3 = getShuntVoltage(2);
		Bus_data.bus_voltage_V_3 = getBusVoltage(2);
		Bus_data.load_voltage_V_3 = getLoadVoltage(2);
		Bus_data.current_mA_3 = getCurrent(2);
		//Bus_data.power_mW_1 = getPower(2);
	#else
		// Just 1 INA is present, so use index 0 for all
		Bus_data.shunt_voltage_mV_2 = getShuntVoltage(0);
		Bus_data.bus_voltage_V_2 = getBusVoltage(0);
		Bus_data.load_voltage_V_2 = getLoadVoltage(0);
		Bus_data.current_mA_2 = getCurrent(0);
		
		Bus_data.shunt_voltage_mV_3 = getShuntVoltage(0);
		Bus_data.bus_voltage_V_3 = getBusVoltage(0);
		Bus_data.load_voltage_V_3 = getLoadVoltage(0);
		Bus_data.current_mA_3 = getCurrent(0);
	#endif
	
	return;
}


float BusPowerMonitor::readThermistor(void) {
	// This method reads the thermistor using the built-in analogRead function 

	// The nRF52840 has a 10-bit ADC, so the max value is 1023.
	// The voltage range is 0 to 3.6V, so the conversion factor is 3.6V/1024 steps.
	// float mv_per_lsb = 3600.0F/1024.0F;

	uint16_t adc_value = analogRead(THERMISOTOR_AIN_PIN);
	float thermistor_V = (float)adc_value * V_PER_LSB;
	
	return thermistor_V;
}


void BusPowerMonitor::rebootBus(void) {
	if (VERBOSE_POWER_MONITOR_OUTPUT) {
		Serial.println(F("\n ~ Rebooting the SlimSat ..."));
	}

	#if (USING_SLIMSAT_MODULE_CONFIG == 1)
		digitalWrite(BUS_WAG_WDT_PIN, LOW);
	#else
		digitalWrite(SW_DEV_BUS_RESET_PIN, LOW);
	#endif
	
	return;
}


void BusPowerMonitor::initiateCutdown(void) {
	// This method initiates a cutdown by starting the burn wire
	if (VERBOSE_POWER_MONITOR_OUTPUT) {
		Serial.println(F("\n ~ Initiating Cutdown (Burn wire) ..."));
	}
	
	// Put a timer on this, so that it turns off at some point?
	digitalWrite(CUTDOWN_DO_PIN, HIGH);
	
	return;
}