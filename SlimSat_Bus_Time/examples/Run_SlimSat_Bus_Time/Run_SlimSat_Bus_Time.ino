// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <sc_bus_time.h>

// Construct the S/C Bus Time Object
ScBusTime ScTime;

void setup() {
  // Open a serial port for SlimSat communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F(" ~ Running SlimSat S/C Bus Time ..."));
}

void loop() {
	// Exercise the various SlimSat S/C Bus Time methods

	ScTime.print();

	Serial.println("");
	
	Serial.println(ScTime.getScBusTimeInMs());
	ScTime.setScBusEpochTimeInMs(12345);
	Serial.println(ScTime.getScBusEpochTimeInMs());

	Serial.println(F("\n ~ Done."));
  
	delay(30000);
}