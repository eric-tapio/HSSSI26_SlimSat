// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// This is a LoRa Radio V&V sketch. Run this RX sketch using one system with a LoRa radio and
// run the TX counterpart sketch on another system with a LoRa radio to ensure connectivity

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

	Serial.println(F("\n ~ Running SlimSat Bus/LoRa Radio Receive Message Test ...\n"));

	// Initialize the SlimSat Bus
	Slimsat.initializeBus();

	return;
}


void loop(void) {
	Slimsat.getReceivedRadioMessage();
	delay(1000);
}