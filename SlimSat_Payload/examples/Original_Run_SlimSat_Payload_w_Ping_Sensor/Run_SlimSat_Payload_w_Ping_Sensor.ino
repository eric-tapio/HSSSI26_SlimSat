// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <payload.h>
#include <payload_cmd_constants.h>

#define FOR_LOOP_COUNT_START 0
#define FOR_LOOP_COUNT_STOP 45


// Construct the payload
Payload pl;

void setup() {
  // Open a serial port for SlimSat communication
  Serial.begin(115200);
  while (!Serial) {
    // Wait for Serial
		delay(10);
  }

	Serial.print(" ~ Running SlimSat Payload with Ping Sensor (Class Version) ... ");

	// Print the payload
	pl.print();
}

void loop() {
	// Operate the payload via the payload public interface
	// Operations is performed by sending the payload commands

	// Test the Ping Payload command (Ping as in, send a message expecting a response, which is different from Ping, the name of the sensor that is being used. Expect an Acknowledgement ("A") back
	Serial.print("\n ~ Sending payload command: ");
	Serial.println(PING_PAYLOAD_CMD_ID);
	pl.handlePayloadCommand(PING_PAYLOAD_CMD_ID, 0);
	Serial.print(" ~ Received response from payload: ");
	Serial.println(pl.getPayloadDataStr());

	// Test the get Payload State command. Expect an Acknowledgement ("A") and that the Payload is Initialized (1)
	Serial.print("\n ~ Sending payload command: ");
	Serial.println(GET_PAYLOAD_STATE_CMD_ID);
	pl.handlePayloadCommand(GET_PAYLOAD_STATE_CMD_ID, 0);
	Serial.print(" ~ Received response from payload: ");
	Serial.println(pl.getPayloadDataStr());


	// Test the Take Measurement command. Expect there to be only one measurement when printed
	Serial.print("\n ~ Sending payload command: ");
	Serial.println(TAKE_MEASUREMENTS_CMD_ID);
	pl.handlePayloadCommand(TAKE_MEASUREMENTS_CMD_ID, 0);

	// Test the Print Measurement command
	Serial.print("\n ~ Sending payload command: ");
	Serial.print(PRINT_MEASUREMENTS_CMD_ID);
	pl.handlePayloadCommand(PRINT_MEASUREMENTS_CMD_ID, 0);
	
	// Test the Get Processed Measurements command
	Serial.print("\n ~ Sending payload command: ");
	Serial.println(GET_PROCESSED_MEASUREMENT_DATA_CMD_ID);
	pl.handlePayloadCommand(GET_PROCESSED_MEASUREMENT_DATA_CMD_ID, 0);
	Serial.print(" ~ Received response from payload: ");
	Serial.println(pl.getPayloadDataStr());


	// Now set the new number of consecutive measurements to take in each round
	Serial.print("\n ~ Sending payload command: ");
	Serial.println(SET_NUMBER_MEASUREMENTS_TO_TAKE_CMD_ID);
	pl.handlePayloadCommand(SET_NUMBER_MEASUREMENTS_TO_TAKE_CMD_ID, 16);

	// Test the Take Measurement command. Expect there to be only one measurement when printed
	Serial.print("\n ~ Sending payload command: ");
	Serial.println(TAKE_MEASUREMENTS_CMD_ID);
	pl.handlePayloadCommand(TAKE_MEASUREMENTS_CMD_ID, 0);

	// Test the Print Measurement command
	Serial.print("\n ~ Sending payload command: ");
	Serial.print(PRINT_MEASUREMENTS_CMD_ID);
	pl.handlePayloadCommand(PRINT_MEASUREMENTS_CMD_ID, 0);

	// Test the Get Processed Measurements command
	Serial.print(" ~ Sending payload conmmand: ");
	Serial.print(GET_PROCESSED_MEASUREMENT_DATA_CMD_ID);
	pl.handlePayloadCommand(GET_PROCESSED_MEASUREMENT_DATA_CMD_ID, 0);
	Serial.print(" ~ Received response from payload: ");
	Serial.println(pl.getPayloadDataStr());

	Serial.print(" ~ Done running SlimSat Payload Cmds ...");

  delay(30000);
}