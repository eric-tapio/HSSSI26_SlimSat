// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <bus_flash_memory_if.h>

// Construct the S/C Flash Memory IF Object
FlashMemoryIf FlashMemIf;

// Declare global variables used
uint32_t write_to_adr = 0;

void setup() {
  // Open a serial port for SlimSat communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);
	}

	Serial.println(F("\n ~ Running SlimSat Flash Memory Interface ...\n"));
	
	FlashMemIf.print();
	Serial.println("");
	
  FlashMemIf.flashEraseBlock(0);

	float float_val = 3.1425;
	write_to_adr = 4;
	FlashMemIf.writeValueToFlashAddress(write_to_adr, float_val);

	uint16_t uint16_val = 65530;
	write_to_adr = 10;
  FlashMemIf.writeValueToFlashAddress(write_to_adr, uint16_val);

	write_to_adr = 20;
  FlashMemIf.writeValueToFlashAddress(write_to_adr, 'A');

	double double_val = 6.2830;
	write_to_adr = 30;
  FlashMemIf.writeValueToFlashAddress(write_to_adr, double_val);

	long long_val = 123456789;
	write_to_adr = 40;
  FlashMemIf.writeValueToFlashAddress(write_to_adr, long_val);
	
	return;
}


void loop() {
}
