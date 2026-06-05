// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// This is a LoRa Radio V&V sketch. Run this TX sketch using one system with a LoRa radio and
// run the RX counterpart sketch on another system with a LoRa radio to ensure connectivity

#include <Adafruit_TinyUSB.h> // for Serial
#include <slimsat_bus.h>

#define MSG_LENGTH 100
#define TEST_MSG "Hello SlimSat World-"

// Construct the SlimSat Bus
SlimSatBus Slimsat;

// Declare global variables
char msg_buffer[MSG_LENGTH];

void initializeMessageBuffer(void) {	
	memset(msg_buffer, 0, sizeof(msg_buffer));
	
	return;
}


void setup(void) {
	// Open a serial port for SlimSat SW Simulator communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F("\n ~ Running SlimSat Bus/LoRa Radio Transmit Test Message ...\n"));

	// Initialize the SlimSat Bus
	Slimsat.initializeBus();

	return;
}


void loop(void) {
	Serial.println(F(" ~ Transmiting Test Messages ..."));

	for (uint8_t i=0; i<100; i++) { 
		snprintf(msg_buffer, sizeof(msg_buffer), "%s%d\n", TEST_MSG, i);
		Serial.println(msg_buffer);
		Slimsat.transmitMessageToLoraRadio(msg_buffer);
		delay(1000);
	}

	Serial.println(F(" ~ Done Transmiting Test Messages ..."));

	delay(10000);
}