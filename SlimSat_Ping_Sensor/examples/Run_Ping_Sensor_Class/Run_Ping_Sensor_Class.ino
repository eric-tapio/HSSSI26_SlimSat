// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <ping_sensor.h>

// Construct the Ping Sensor Object
Ping_Sensor ping;


void setup() {
  // Open a serial port for SlimSat communication
  Serial.begin(115200);
	while (!Serial) {
		// Wait for Serial
		delay(10);
	}

	Serial.print(" ~ Running Ping Sensor (Class version) ...");

	// Print the ping sensor data members and their values
	ping.print();
}


void loop() {
	// Test The Ping Sensor Interface
	ping.getRangeMeasurementInCm();
	delay(1000);
}
