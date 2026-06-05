// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Adafruit_TinyUSB.h>
#include <gps_if.h>

// Construct the GPS IF object
GpsIf gps_if;

// Declare global variables used
char* gps_pos_msg = nullptr;


void setup() {
	// Open a serial port to the CPU
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

  Serial.println("\n ~ Running GPS IF, which uses Tiny GPS ...\n");
  
  // Start the GPS Sensor
  gps_if.print();
  gps_if.begin();

  Serial.println("\n ~ Pos Msg Format: GPS Time, Latitude, Longitude, Altitude");

  return;
}


void loop() {
  // Get the latest GPS Position Message
  gps_pos_msg = gps_if.getGpsPositionMessage();
  
  if (1) {
    gps_if.print();
  }
  
  if (gps_pos_msg != nullptr) {
    Serial.println(gps_pos_msg);
  }
  
  delay(2000);
}
