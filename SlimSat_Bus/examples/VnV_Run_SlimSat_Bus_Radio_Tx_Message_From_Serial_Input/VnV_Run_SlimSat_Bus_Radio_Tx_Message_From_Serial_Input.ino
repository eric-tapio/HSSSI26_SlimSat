// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// This is a LoRa Radio V&V sketch. Run this TX sketch using one system with a LoRa radio and
// run the RX sketch on another system with a LoRa radio to ensure connectivity.
// Ensure that #define GET_CMD_INPUT_FROM_LORA_RADIO is set to 0 in slimsat_bus.h
// Then type a string input into the Arduino IDE serial terminal and press the Enter key.
// Verify that the message typed is received on by the RX system

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

	Serial.println(F("\n ~ Running SlimSat Bus/LoRa Radio Transmit Message From Serial Input ...\n"));

	// Initialize the SlimSat Bus
	Slimsat.initializeBus();

	Serial.println(F(" ~ Enter a message to transmit: "));

	return;
}


void loop(void) {
	Slimsat.transmitMessageFromSerialTermToLoraRadio();
}