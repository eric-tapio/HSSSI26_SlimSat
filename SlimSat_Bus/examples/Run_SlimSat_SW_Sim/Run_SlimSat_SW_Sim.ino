// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <slimsat_bus.h>

#define DEFAULT_PAYLOAD_OP_PERIOD_IN_MS 5000L
#define DEFAULT_BEACON_PERIOD_IN_MS 33000L

// Construct the SlimSat Bus
SlimSatBus Slimsat;

void setup() {
	// Initialize the SlimSat SW Simulator

	// Open a serial port for SlimSat SW Simulator communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F(" ~ Running SlimSat SW Simulator ... "));
  
	// Set the payload and beacon Timer periods
	Slimsat.Payload_timer.setIntervalInMs(DEFAULT_PAYLOAD_OP_PERIOD_IN_MS);
	Slimsat.Beacon_timer.setIntervalInMs(DEFAULT_BEACON_PERIOD_IN_MS);
}


void loop() {
	// Infinite loop - This is where SlimSat operations gets performed
	Slimsat.performScBusOperationIteration();

	if (Slimsat.Payload_timer.timerHasElapsed()) {
		Serial.println("\n ~ Performing Payload Op ...");
		Slimsat.Payload1.performPayloadOperationIteration();
	}

	if (Slimsat.Beacon_timer.timerHasElapsed()) {
		Serial.println("\n ~ Transmitting Beacon Message ...");
		Slimsat.transmitBeaconMessage();
	}
}