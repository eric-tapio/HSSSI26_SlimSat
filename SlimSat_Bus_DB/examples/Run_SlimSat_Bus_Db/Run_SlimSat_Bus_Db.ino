// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <sc_bus_db.h>


// Construct the S/C Bus Database Object
ScBusDb ScDb;

void setup() {
  // Open a serial port for SlimSat communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F("\n ~ Running SlimSat S/C Bus Database (DB) ..."));
}

void loop() {
	// Exercise the various SlimSat S/C Bus Database methods
	uint8_t state = 0;

	ScDb.print();
	
	Serial.println("");

	Serial.println(ScDb.getScBusMode());
	ScDb.Bus_mode = NOMINAL_ON;
	Serial.println(ScDb.getScBusMode());
	ScDb.setScBusMode(4);
	Serial.println(ScDb.getScBusMode());
	ScDb.setScBusMode(2);
	Serial.println(ScDb.getScBusMode());

	Serial.println("");

	Serial.println(ScDb.getScBusStreamDataState());
	ScDb.setScBusStreamDataState(1);
	Serial.println(ScDb.getScBusStreamDataState());

	Serial.println("");

	Serial.println(ScDb.Sc_Time.getScBusEpochTimeInMs());
	ScDb.Sc_Time.setScBusEpochTimeInMs(12345);
	Serial.println(ScDb.Sc_Time.getScBusEpochTimeInMs());

	Serial.println(F("\n ~ Done."));
  
	delay(30000);
}