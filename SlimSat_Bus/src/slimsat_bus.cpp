// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <slimsat_bus.h>


SlimSatBus::SlimSatBus(void) {
	// Default constructor for the SlimSatBus
	
	Payload_timer.enableAndStartTimer();
	Beacon_timer.enableAndStartTimer();
	
	dataIndex = 0;
	newCmdRxd = 0;
	newCmdToProcess = 0;
	
	return;
}


void SlimSatBus::print(void) const {
	// This method prints the data members for the SlimSatBus object. This function is provided for diagnostic purposes only
	
	Serial.println(F("\n ~ Printing SlimSat Bus Object ..."));
	Beacon_timer.print();
	Serial.print(F(""));
	Serial.print(F("\n ~ Printing the Payload Timer ... "));
	Payload_timer.print();
	Serial.print(F(""));
	Serial.print(F("\n ~ Printing the Beacon Timer ... "));
	Beacon_timer.print();
	Serial.print(F(""));

	return;
}


void SlimSatBus::performScBusOperationIteration(void) {
	// This method performs the SlimSat bus actions that should be performed every iteration
	if (VERBOSE_SC_BUS_OUTPUT) {
		Serial.println(F("\n ~ Performing Sc Bus Opeation Iteration ..."));
	}
	// The idea is to do this continuously:
	//  1) Check radio for a command
	//  2) If receoved a command, then handle the command
	//  3) Transmit a response or data message
	
	getScBusCmd();

	if (newCmdToProcess) {
		newCmdToProcess = 0;
		handleScBusCmd();
	}
	
	
	if (Sc_database.getScBusStreamDataState() == 1) {
		// Then stream the data
		
	}
	
	return;
}


void SlimSatBus::initializeserialTermReceivedChars(void) {
	// This method initializes the serial terminal receive character buffer

	for (uint8_t i = 0; i < MAX_CHARS; i++)
		serialTermReceivedChars[i] = '\0';

	return;
}


void SlimSatBus::initializeScBus(void) {
	// This method performs the actions that are requied to initialize the S/C Bus 
	if (VERBOSE_SC_BUS_OUTPUT) {
		Serial.println(F("\n ~ Initializing Sc Bus Opeation Iteration ..."));
	}

	initializeserialTermReceivedChars();
	
	return;
}


void SlimSatBus::transmitBeaconMessage(void) {
	// This method transmits the S/C Bus beacon message
	
	if (VERBOSE_SC_BUS_OUTPUT) {
		Serial.println(F("\n ~ Transmitting Beacon Message ..."));
	}
	
	Serial.println(F("$S01G,BEACON,MSG,1*AB"));
  return;
}


void SlimSatBus::getScBusCmd(void) {
	// This method checks and gets S/C Bus commands (from the serial terminal or LoRa)
	
	getSerialTermInput();
	displayNewSerialTermData();

	return;
}


void SlimSatBus::handleScBusCmd(void) {
	// This method handles S/C Bus Commands
	
	if (VERBOSE_SC_BUS_OUTPUT) {
		Serial.println(F("\n ~ Handling SC Bus Command ..."));
	}

	char* sc_cmd_msg = &(serialTermReceivedChars[0]);
	char* sc_response_msg = NULL;

	if (0) {
		Serial.print(F("\n ~ serialTermReceivedChars is: "));
		Serial.println((unsigned int)&serialTermReceivedChars);
		Serial.println(serialTermReceivedChars);

		Serial.print(F("\n ~ sc_cmd_msg is: "));
		Serial.println((unsigned int)*sc_cmd_msg);
		Serial.println(sc_cmd_msg);
	}
	
	Msg_handler.handleNmeaMsg(sc_cmd_msg, Sc_database, Payload1, LoRa, Payload_timer, Beacon_timer);
	
	if (1) {
		Serial.print(F("\n ~ Received response from S/C: "));
	}
	
	uint8_t num_bfr_msgs = Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_response_msg = Msg_handler.Sc_cbfr.circularBufferPopMsg();

	return;
}


void SlimSatBus::getSerialTermInput(void) {
	// This method gets input from the serial terminal
	
	while (Serial.available() > 0 && newCmdRxd == 0) {
		char rc = Serial.read(); // Read one character from the serial buffer

		// Store character in array if not newline and within bounds
		if (rc != '\n' && dataIndex < MAX_CHARS - 1) {
			serialTermReceivedChars[dataIndex] = rc;
			dataIndex++;
		} else {
			// If newline or buffer full, terminate string and set flag
			serialTermReceivedChars[dataIndex] = '\0'; // Null-terminate the string
			newCmdRxd = 1;
			newCmdToProcess = 1;
			dataIndex = 0; // Reset index for next message
		}
	}

	return;
}


void SlimSatBus::displayNewSerialTermData(void) {
	// This method displays new data that has been entered into the serial terminal
	
	if (newCmdRxd == 1) {
		if (1) {
			Serial.print("Received: ");
			Serial.println(serialTermReceivedChars);	
		}
		
		newCmdRxd = 0; // Reset flag for next message
	}

	return;
}