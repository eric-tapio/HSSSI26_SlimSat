// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <tmp75a_if.h>


// Constructors
Tmp75aIf::Tmp75aIf(void) : lm75a(TMP75A_I2C_ADR) {

	address = TMP75A_I2C_ADR;
	initializeDataMembers();
}


Tmp75aIf::Tmp75aIf(uint8_t i2c_adr) : lm75a(i2c_adr) {

	address = i2c_adr;
	initializeDataMembers();
}


void Tmp75aIf::initializeDataMembers(void) {
	// This method initializes the private data members
	
	temperature_in_deg_c = 0.0;
    is_connected = 0;
	
	return;
}


bool Tmp75aIf::begin(void) {
	// This method begins the temp sensor 
	lm75a.begin();
	
	// Check to see if the device is connected
	is_connected = lm75a.isConnected();
	
	if (is_connected) {
        if (VERBOSE_TMP75A_OUTPUT) {
			Serial.println(F(" ~ TMP75A Temp Sensor Started!"));
		}
	}
	else {
        if (VERBOSE_TMP75A_OUTPUT) {
			Serial.println(F(" ~ Error, failed to initialize TMP75A Temp Sensor!"));
		}
		if (RAISE_TMP75_HW_START_ERRORS) {
			Serial.println(F(" ~ Error, failed to initialize TMP75A Temp Sensor!"));
			//Serial.println(F(" ~ Error Encountered ..."));
			while (RAISE_TMP75_HW_START_ERRORS) {
				delay(10);  
			}
		}
	}
	
	return is_connected;
}


float Tmp75aIf::getTemperatureInDegC(void) {
	// This method gets and returns the measured temperature in degrees C
	temperature_in_deg_c = lm75a.getTemperature();
	
    if (VERBOSE_TMP75A_OUTPUT) {
		Serial.print(F(" ~ Temp in Deg C is: "));
		Serial.println(temperature_in_deg_c);
	}
		
    return temperature_in_deg_c;
}


bool Tmp75aIf::isConnected(void) const {
	// This method returns the is connected state
	if (VERBOSE_TMP75A_OUTPUT) {
		Serial.print(F(" ~ Is Connected is: "));
		Serial.println(is_connected);
	}
	
	return is_connected;
}
	
	
void Tmp75aIf::print(void) const {
	Serial.println(F(" ~ Printing TMP75A Temperature Sensor Interface Data Members ..."));
	Serial.print(F("     Address: "));
	Serial.println(address);
	Serial.print(F("     Is Connected: "));
	Serial.println(is_connected);
	Serial.print(F("     Temperature in Deg C: "));
	Serial.println(temperature_in_deg_c);
	Serial.print(F(""));
	
    return;
}