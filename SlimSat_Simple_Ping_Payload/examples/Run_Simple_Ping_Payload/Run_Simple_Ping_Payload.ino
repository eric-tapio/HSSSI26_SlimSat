// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Adafruit_TinyUSB.h> // for Serial
#include <simple_ping_payload.h>

// Construct the Payload object
Payload Ping_Payload;

// Construct the Payload Data object
PlDataRec pl_data;


void setup() {
	// Put your setup code here, to run once:
	
	// Open a serial port for SlimSat communication
	Serial.begin(115200);
	while (!Serial) {
		// Wait for Serial
		delay(10);
	}

	Serial.println(" ~ Running Simple Ping Sensor Payload ...");

	// Initialize the payload
	Ping_Payload.initializePayload();

	return;
}


void loop() {
	// Test The Ping Payload Interface
	Ping_Payload.performPayloadLoopIteration(pl_data);
	
	// Print the range results
	pl_data.printArray();

	delay(1000);
}