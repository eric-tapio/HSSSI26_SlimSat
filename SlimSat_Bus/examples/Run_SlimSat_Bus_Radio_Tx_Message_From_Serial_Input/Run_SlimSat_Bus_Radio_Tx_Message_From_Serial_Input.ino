// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <slimsat_bus.h>

// Construct the SlimSat Bus Object
SlimSatBus Slimsat;


void setup(void) {
	// Open a serial port for SlimSat SW Simulator communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F("\n ~ Running SlimSat Bus/LoRa Radio Transmit Message From Serial Input ...\n"));

	// Initialize the S/C Bus
	Slimsat.initializeBus();

	Serial.println(F(" ~ Enter a message to transmit: "));

	return;
}


void loop(void) {
	Slimsat.transmitMessageFromSerialTermToLoraRadio();
}