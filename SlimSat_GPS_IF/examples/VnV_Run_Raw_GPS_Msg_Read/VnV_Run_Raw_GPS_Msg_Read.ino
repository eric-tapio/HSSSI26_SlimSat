// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Adafruit_TinyUSB.h>

void setup() {
	// Open a serial port to the CPU
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	// Open a serial port to the GPS Device
  Serial1.begin(9600);
    while (!Serial) {
		delay(10);   
	}

  Serial.println("\n  ~ GPS Sensor Started ...");
  Serial.println(" ~ Reading Raw GPS Messages ...\n");

  return;
}


void loop() {
  if (Serial1.available() > 0) { // Check if data is available
    char received_char = Serial1.read();  // Read one character
    Serial.print(received_char); // Print the character to the serial monitor
  }

  return;
}