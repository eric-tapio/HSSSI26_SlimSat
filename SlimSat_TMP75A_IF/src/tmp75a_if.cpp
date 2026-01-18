// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <tmp75a_if.h>


// Constructors
Tmp75aIf::Tmp75aIf(void) : lm75a(TMP75A_I2C_ADR){

	temperature_in_deg_c = 0.0;
    is_connected = 0;
	
	// Automatically start the temp sensor
	//begin();
}


bool Tmp75aIf::begin(void) {
// This method begins the temp sensor 
	lm75a.begin();
	
	// Check to see if the device is connected
	is_connected = lm75a.isConnected();
	
	if (is_connected) {
        if (VERBOSE_TMP75A_OUTPUT) {
			Serial.println(" ~ TMP75A Temp Sensor Started!");
		}
	}
	else {
        if (VERBOSE_TMP75A_OUTPUT) {
			Serial.println(" ~ Error, failed to initialize TMP75A Temp Sensor!");
		}
	}
	
	return is_connected;
}


float Tmp75aIf::getTemperatureInDegC(void) {
	// This method gets and returns the measured temperature in degrees C
	temperature_in_deg_c = lm75a.getTemperature();
	
    if (VERBOSE_TMP75A_OUTPUT) {
		Serial.print(" ~ Temp in Deg C is: ");
		Serial.println(temperature_in_deg_c);
	}
		
    return temperature_in_deg_c;
}


bool Tmp75aIf::isConnected(void) const {
	// This method returns the is connected state
	if (VERBOSE_TMP75A_OUTPUT) {
		Serial.print(" ~ Is Connected is: ");
		Serial.println(is_connected);
	}
	
	return is_connected;
}
	
	
void Tmp75aIf::print(void) const {
	Serial.println(F(" ~ Printing TMP75A Temperature Sensor Interface Data Members ..."));
	Serial.print(F("     Is Connected: "));
	Serial.println(is_connected);
	Serial.print(F("     Temperature in Deg C: "));
	Serial.println(temperature_in_deg_c);
	Serial.print(F(""));
	
    return;
}