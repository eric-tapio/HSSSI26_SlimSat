// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <slimsat_bus.h>

#define STR_CMP_MATCHES_VALUE 0

// Construct the SlimSat Bus
SlimSatBus Slimsat;

uint8_t error_count = 0;
const char EMPTY_GPS_MSG[] = ",,,,";

void setup(void) {
	// Initialize the SlimSat SW Simulator

	// Open a serial port for SlimSat SW Simulator communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F(" ~ Pinging SlimSat Bus Sensors ... "));

	// Initialize the S/C Bus
	Slimsat.initializePinStates();

	Serial.println(F("\n ~ Starting LoRa Radio ..."));
	int lora_start_return_val = Slimsat.startLoRa();
	
	if (lora_start_return_val == 0) {
		Serial.println(F(" ~ LoRa started successfully!"));
	}
	else {
		Serial.println(F(" ~ Error: LoRa was not started."));
		error_count++;
	}

	Serial.println(F("\n ~ Starting Power Monitor (3x INA219 + TMP sensor) ..."));
	uint pm_start_return_val = Slimsat.Bus_database.startPowerMonitor();
	
	if (pm_start_return_val == 1) {
		Serial.println(F(" ~ Power Monitor started successfully!"));
	}
	else {
		Serial.println(F(" ~ Error: Power Monitor was not started."));
		error_count++;
	}


	Serial.println(F("\n ~ Starting GPS ..."));
	Slimsat.startGps();
	
	// Read a message from GPS
  uint gps_started = 0;
	char* rxd_gps_msg_ptr = Slimsat.Gps.getGpsPositionMessage();
	if ((rxd_gps_msg_ptr == nullptr) || (strcmp(rxd_gps_msg_ptr, EMPTY_GPS_MSG) == STR_CMP_MATCHES_VALUE)) {
		Serial.println(F(" ~ Error: No GPS Message Rx'd"));
		error_count++;
	}
	else {
		Serial.println(F(" ~ GPS Message Rx'd"));
		Serial.println(rxd_gps_msg_ptr);
	}

	Serial.println(F("\n ~ Ping of SlimSat Bus Sensors complete."));

	if (error_count == 0) {
			Serial.println(F("     No Errors were encountered!"));
	}
	else {
		Serial.print(F("     Number of Errors encoutered: "));
		Serial.println(error_count);
		Serial.print(F("\n ~ Please check your SlimSat connections and configuration and try again."));
	}
	
	return;
}


void loop(void) {
	// Infinite loop
}