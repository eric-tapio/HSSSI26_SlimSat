//
// Bus Data Class
//
// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <bus_data.h>


BusDataRec::BusDataRec(void) {
	// Default Constructor
	time = 1234;
	bus_rec_number = 0; // This value gets populated by the bus before being written to flash memory
	temp_C = 23.1;
	thermistor_V = 1.23;
	shunt_voltage_mV_1 = 0.1;
	bus_voltage_V_1 = 2.3;
	load_voltage_V_1 = 3.4;
	current_mA_1 = 4.5;
	//power_mW_1 = 0.0;
	shunt_voltage_mV_2 = 5.6;
	bus_voltage_V_2 = 6.7;
	load_voltage_V_2 = 7.8;
	current_mA_2 = 9.0;
	//power_mW_2 = 0.0;
	shunt_voltage_mV_3 = 0.1;
	bus_voltage_V_3 = 1.2;
	load_voltage_V_3 = 2.3;
	current_mA_3 = 3.4;
	//power_mW_3 = 0.0;
}


void BusDataRec::printRecord(void) { 
	Serial.println(F("\n ~ Printing Bus Data Record ..."));
	
	Serial.print(F("    Number Float Data Elements is: "));
	Serial.println(NUM_BUS_DATA_FLOAT_ELEMENTS);
	Serial.print(F("    Bus Data Record Size in Bytes is: "));
	Serial.println(getRecordSize());	
		
	Serial.print(F("     Time: "));
	Serial.println(time);
	Serial.print(F("     Bus Rec Number: "));
	Serial.println(bus_rec_number);
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


uint8_t BusDataRec::getRecordSize(void) {
	// This method computes the size of a Bus Data Record
	return NUM_BUS_DATA_FLOAT_ELEMENTS * sizeof(float) + sizeof(uint32_t) + sizeof(uint16_t);
}