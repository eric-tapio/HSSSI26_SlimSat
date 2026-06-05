/**
 * @file slimsat_bus.cpp
 * @brief SlimSat Bus System Main Controller Implementation
 * 
 * @details This file implements the SlimSatBus class methods for coordinating
 * all bus operations including command processing, payload management,
 * communication handling, and system timing operations.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <slimsat_bus.h>

/**
 * @brief Default constructor for the SlimSatBus
 * @details Initializes all subsystem timers and command processing flags.
 * Sets up the payload and beacon timers for automatic operation and
 * prepares the command reception system for incoming messages.
 */
SlimSatBus::SlimSatBus(void) {
	// Default constructor for the SlimSatBus
	
	Payload_timer.enableAndStartTimer();
	Beacon_timer.enableAndStartTimer();
	Bus_timer.enableAndStartTimer();
	
	serial_msg_data_index = 0;
	new_serial_msg_rxd = 0;
	new_serial_msg_to_process = 0;
	radio_status_code = 0;
	//pl_data_ptr = nullptr;
	
	return;
}


/**
 * @brief Print bus object information
 * @details Outputs diagnostic information about the bus state
 * and subsystem status including timer states for debugging purposes.
 * This function is provided for diagnostic purposes only.
 */
void SlimSatBus::print(void) const {
	// This method prints the data members for the SlimSatBus object. This function is provided for diagnostic purposes only
	
	Serial.println(F("\n ~ Printing SlimSat Bus Object ..."));
	Serial.print(F("    Radio Status Code is: "));
	Serial.println(radio_status_code);
	Serial.print(F(""));
	Serial.print(F("\n ~ Printing the Payload Timer ..."));
	Payload_timer.print();
	Serial.print(F(""));
	Serial.print(F("\n ~ Printing the Beacon Timer ..."));
	Beacon_timer.print();	
	Serial.print(F("\n ~ Printing the Bus Data Record Timer ..."));
	Bus_timer.print();
	Serial.print(F(""));
	
	return;
}


/**
 * @brief Perform one iteration of bus operations
 * @details Executes the main operational loop including:
 * 1. Check radio for incoming commands
 * 2. Process any received commands
 * 3. Handle command execution and response generation
 * This method should be called continuously in the main program loop.
 */
void SlimSatBus::performBusOpLoopIteration(void) {
	// This method performs the SlimSat bus actions that should be performed every iteration
	if (0) {
		Serial.println(F("\n ~ Performing Sc Bus Opeation Iteration ..."));
	}
	// The idea is to do this continuously:
	//  1) Check radio for a command
	//  2) If receoved a command, then handle the command
	//  3) Transmit a response or data message
	
	char* bus_cmd_msg = nullptr;
	
	if (GET_CMD_INPUT_FROM_LORA_RADIO) {
		bus_cmd_msg = getReceivedMsgFromLoraRadio();
	}
	else {
		bus_cmd_msg = getBusCmdFromSerialTerm();
	}
	
	if (bus_cmd_msg != nullptr) {
		handleBusCmd(bus_cmd_msg);
		transmitResponseMessages();
	}
	
	return;
}


/**
 * @brief Initialize serial terminal received characters buffer
 * @details Clears all characters in the serial terminal receive buffer
 * by setting each element to null terminator. This ensures a clean
 * buffer state for receiving new command data.
 */
void SlimSatBus::initializeSerialTermReceivedChars(void) {
	// This method initializes the serial terminal receive character buffer

	for (uint8_t i=0; i<MAX_SERIAL_TERM_BUFFER_CHARS; i++)
		serial_term_received_chars[i] = '\0';

	return;
}


/**
 * @brief Initialize the bus system
 * @details Performs initialization of all bus subsystems including:
 * - Serial terminal character buffer initialization
 * - Subsystem setup and configuration
 * Prepares the system for normal operational mode.
 */
void SlimSatBus::initializeBus(void) {
	// This method performs the actions that are requied to initialize the Bus 
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Initializing Sc Bus ..."));
	}

	// Initialize HW pins
	initializePinStates();
	
	// Start I2C Devices
	// Initialize the Power Monitor
	Bus_database.startPowerMonitor();
	
	// Start the Radio
	startLoRa();
	
	// Start the GPS
	startGps();
	
	// Initialize the Payload
	Payload1.initializePayload();
	
	// Initialize the Serial Terminal (only for dev unit)
	initializeSerialTermReceivedChars();
	
	// Increment the boot counter
	Bus_database.Flash_Memory.incrementFlashBootCount();

	return;
}


void SlimSatBus::initializePinStates(void) {
	// This method initializes the bus pin states

	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Initializing Pin States ..."));
	}
	
	#if (USING_SLIMSAT_MODULE_CONFIG == 1)
		if (VERBOSE_BUS_OUTPUT) {
			Serial.println(F("\n ~ Using SlimSat Module Beta Configuration ..."));
		}
		// Initialize Pins
		// Note: Arduino analog input pins do not need to be initialized if using them for analog readings
		// A0 -> analog input for Thermistor, no initialization needed
		// A1-A4, D2, D5, D9, D11, D12  -> Initialized in common initialization section
		// Set A5 to a DO
		pinMode(BUS_PL_IF_D7_PIN, OUTPUT);
		digitalWrite(BUS_PL_IF_D7_PIN, LOW);

		pinMode(BUS_WAG_WDT_PIN, OUTPUT);
		digitalWrite(BUS_WAG_WDT_PIN, LOW);

		//pinMode(BUS_PL_IF_A5_PIN, OUTPUT);
		//digitalWrite(BUS_PL_IF_A5_PIN, LOW);
	#else
		if (VERBOSE_BUS_OUTPUT) {
			Serial.println(F("\n ~ Using SW Dev Board Configuration ..."));
		}
		// Initialize Pins
		// A0 -> Initialize as a DO
		pinMode(SW_DEV_BUS_RESET_PIN, OUTPUT);
		digitalWrite(SW_DEV_BUS_RESET_PIN, HIGH); // Initialize HIGH and pull LOW to reset the bus
		
		// A1-A4, D2, D5, D9, D11, D12  -> Initialized in common initialization section
		// A5 is uninitialized, so defaults to an analog input
		pinMode(BUS_PL_IF_D12_PIN, OUTPUT);
		digitalWrite(BUS_PL_IF_D12_PIN, LOW);
		
		pinMode(BUS_PL_IF_D13_PIN, OUTPUT);
		digitalWrite(BUS_PL_IF_D13_PIN, LOW);
	#endif

	// Common configuration pin definitions
	pinMode(LORA_NSS_CS_PIN, OUTPUT);
	digitalWrite(LORA_NSS_CS_PIN, LOW); // Initialize LOW since a chip select (Note, for SSM, may be an active low)
	
	pinMode(LORA_INT_PIN, OUTPUT);
	digitalWrite(LORA_INT_PIN, LOW); // Initialize LOW

	pinMode(LORA_RST_PIN, OUTPUT);
	digitalWrite(LORA_RST_PIN, LOW); // Initialize LOW

	pinMode(LORA_DIO0_G0_PIN, OUTPUT);
	digitalWrite(LORA_DIO0_G0_PIN, LOW); // Initialize LOW
	
	pinMode(CUTDOWN_DO_PIN, OUTPUT);
	digitalWrite(CUTDOWN_DO_PIN, LOW); // Initialize LOW and pull HIGH to initiate burn
		
	// The PL Analog pins will all be configured and set to DO with a state of LOW until otherwise directed in the payload code
	// Initialize A1 - A5
	pinMode(BUS_PL_IF_A1_PIN, OUTPUT);
	pinMode(BUS_PL_IF_A2_PIN, OUTPUT);
	pinMode(BUS_PL_IF_A3_PIN, OUTPUT);
	pinMode(BUS_PL_IF_A4_PIN, OUTPUT);
	//pinMode(BUS_PL_IF_A5_PIN, OUTPUT);
	
	digitalWrite(BUS_PL_IF_A1_PIN, LOW);
	digitalWrite(BUS_PL_IF_A2_PIN, LOW);
	digitalWrite(BUS_PL_IF_A3_PIN, LOW);
	digitalWrite(BUS_PL_IF_A4_PIN, LOW);
	//digitalWrite(BUS_PL_IF_A5_PIN, LOW);
			
	// Initialize D5
	pinMode(BUS_PL_IF_D5_PIN, OUTPUT);
	digitalWrite(BUS_PL_IF_D5_PIN, LOW);
	
	// This is the original code
	// pinMode(SW_DEV_BUS_RESET_PIN, OUTPUT);
	// pinMode(BUS_PL_IF_A1_PIN, OUTPUT);
	// pinMode(BUS_PL_IF_A2_PIN, OUTPUT);
	// //pinMode(BUS_PL_IF_A3_PIN, OUTPUT);  // Currently being used as the Payload Ping input signal
	// //pinMode(SW_DEV_BUS_RESET_PIN, INPUT);  // Initialization not needed
	// pinMode(BUS_PL_IF_A5_PIN, OUTPUT);
	// pinMode(LORA_INT_PIN, OUTPUT);
	// pinMode(BUS_WAG_WDT_PIN, OUTPUT); // Initialization needed?
	
	// //pinMode(LORA_RST_PIN, OUTPUT);  // Initialization not needed
	// //pinMode(LORA_NSS_CS_PIN, OUTPUT);  // Initialization not needed
	// //pinMode(LORA_DIO0_G0_PIN, OUTPUT);  // Initialization not needed
	// //pinMode(LORA_DIO1_PIN, OUTPUT);  // Initialization not needed
	// pinMode(CUTDOWN_DO_PIN, OUTPUT);
	// pinMode(SW_DEV_D12_WD_MR_BAR_PIN, OUTPUT); // Initialization needed?
	// pinMode(SW_DEV_D13_WD_RST_BAR_PIN, OUTPUT); // Initialization needed?

	// digitalWrite(SW_DEV_BUS_RESET_PIN, HIGH); // Setting this pin low would reset the ItsyBitsy

	// digitalWrite(CUTDOWN_DO_PIN, LOW);
	// //digitalWrite(BUS_PL_IF_A3_PIN, LOW);
	// //digitalWrite(BUS_PL_IF_A5_PIN, LOW); // Now letting LoRa use this pin as necessary. Not sure if needed yet
	// digitalWrite(LORA_INT_PIN, LOW);
	// digitalWrite(SW_DEV_D12_WD_MR_BAR_PIN, LOW);
	// digitalWrite(SW_DEV_D13_WD_RST_BAR_PIN, LOW);

	return;
}


void SlimSatBus::startGps(void) {
	// Start the GPS Sensor
	
	if (USE_GPS_SENSOR) {
		if (VERBOSE_BUS_OUTPUT) {
			Serial.println(" ~ Starting GPS ...");
		}
		Gps.begin();
	}
	else {
		if (VERBOSE_BUS_OUTPUT) {
			Serial.println(" ~ Note: Not using GPS Sensor ...");
		}
	}	
	
	return;	
}


int16_t SlimSatBus::startLoRa(void) {
	// Start the Radio
	
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Starting the LoRa Radio ..."));
	}
	
	int16_t radio_status_code = LoRa.beginUsingStandardDefaultValues();
	//int16_t radio_status_code = LoRa.begin();
	
	if (radio_status_code == 0) {
		if (VERBOSE_BUS_OUTPUT) {
			Serial.println(" ~ Lora Radio Successfully Started!");
		}
	}
	else {
		if (VERBOSE_BUS_OUTPUT) {
			Serial.println(" ~ ERROR: Lora Radio FAILED to Start!");
		}
	}
		
	return radio_status_code;
}


/**
 * @brief Transmit beacon message
 * @details Sends the beacon message containing system status
 * and identification information. The beacon message follows NMEA format
 * and includes identification and status data.
 */
void SlimSatBus::transmitBeaconMessage(void) {
	// This method transmits the Bus beacon message
	
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Transmitting Beacon Message ..."));
	}
	
	// Transmit the Beacon Message to both the Serial Port and to the radio for debug/aliveness 
	Serial.println(F("$S01G,BEACON,MSG,1*AB"));
	transmitMessageToLoraRadio("$S01G,BEACON,MSG,1*AB");
	
	return;
}


/**
 * @brief Get bus commands
 * @details Checks for incoming commands from multiple sources including:
 * - Serial terminal input
 * - LoRa radio communications
 * Processes and buffers received commands for handling.
 */
void SlimSatBus::getInputFromSerialTerm(void) {
	// This method checks and gets Bus commands from the serial terminal
	
	getSerialTermInput();
	displayNewSerialTermData();

	return;
}


uint8_t SlimSatBus::CmdMsgIsForGs(char* msg) {
	// This method determines whether or not the message is for the GS
	
	uint8_t gs_id_matches = 0;
	
	// Check if the cmd is for the GS
	if (strncmp(msg, GS_FULL_HEADER_ID, strlen(GS_FULL_HEADER_ID)) == STRNCMP_MATCHES_RESULT) {
		gs_id_matches = 1;
	}

	//Serial.print(F("\n ~ Msg is for GS is: "));
	//Serial.println(gs_id_matches);
		
	return gs_id_matches;
}
	

void SlimSatBus::performGsOpLoopIteration(void) {
	// This method performs the GS actions that should be performed every iteration
	
	if (0) {
		Serial.println(F("\n ~ Performing GS Opeation Iteration ..."));
	}
	// The idea is to do this continuously:
	//  1) Check radio for a command
	//  2) If receoved a command, then handle the command
	//  3) Transmit a response or data message
	
	char* cmd_msg = nullptr;
	char* response_msg = nullptr;
	
	uint8_t is_gs_msg = 0;
	
	cmd_msg = getBusCmdFromSerialTerm();
	
	if (cmd_msg != nullptr) {
		// For enhanced clarity add a new blank line to the output to distinguish current cmd responses from previous one(s)
		Serial.println();
		
		is_gs_msg = CmdMsgIsForGs(cmd_msg);
		
		if (is_gs_msg) {
		// Then handle the message
			handleBusCmd(cmd_msg);
			transmitResponseMessages();
		}
		else {
			// The message is for a SlimSat. As a result, send the message straight to the radio
			transmitMessageToLoraRadio(cmd_msg);
		}
	}
	
	// Then check the radio for response messages
	response_msg = getReceivedMsgFromLoraRadio();
	
	if (response_msg != nullptr) {
		// Then output the response message to the serial termminal
		if (0) {
			Serial.print(F("\n ~ Received Response Msg: "));
		}
		Serial.println(response_msg);
	}

	return;
}


char* SlimSatBus::getBusCmdFromSerialTerm(void) {
	// This method performs the SlimSat bus actions that should be performed every iteration
	if (0) {
		Serial.println(F("\n ~ Performing Sc Bus Opeation Iteration ..."));
	}
	
	getInputFromSerialTerm();

	if (new_serial_msg_to_process) {
		new_serial_msg_to_process = 0;

		// There is a guard here above to ensure that the ptr is not null
		char* rxd_message = &(serial_term_received_chars[0]);
		
		//Serial.print(F("\n ~ Rx'd Message from Serial Term: "));
		//Serial.println(rxd_message);
		
		return rxd_message;
	}
	else {
		return nullptr;
	}
}


char* SlimSatBus::getReceivedMsgFromLoraRadio(void) {
	// This method checks and gets Bus commands from the LoRa
	char* rxd_message_ptr = nullptr;
	
	if (0) {
		Serial.println(F("\n ~ Getting Command from LoRa Radio ..."));
	}

	rxd_message_ptr = LoRa.receiveUsingInterrupt();
	
	if (rxd_message_ptr != nullptr) {
		if (0) {
			Serial.println(F("\n ~ Message received from Lora Radio."));
			Serial.println(rxd_message_ptr);
		}
	}
	//else {
	//	Serial.println(F("\n ~ No Message received from Lora."));
	//}

	return rxd_message_ptr;
}


void SlimSatBus::getReceivedMsgFromLoraRadioUsingBlocking(void) {
	// This method checks and gets Bus commands from the LoRa
	
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Getting Command from LoRa Radio (Using Blocking) ..."));
	}
	
	LoRa.receiveUsingBlocking();
	
	return;
}


void SlimSatBus::transmitResponseMessages(void) {
	// This method transmits the given message using the LoRa Radio
	
	// Transmit any responses
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Transmitting all response messages ..."));
		Serial.print(F(" ~ Received response from Bus: "));
	}
	
	uint8_t num_msgs = Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	char* msg = nullptr;
	
	if (VERBOSE_BUS_OUTPUT) {
		Serial.print(F("\n ~ num_msgs is: "));
		Serial.println(num_msgs);
	}
	
	for (uint8_t i=0; i<num_msgs; i++) {
		msg = Msg_handler.getNextMsg();
		
		if (VERBOSE_BUS_OUTPUT) {
			Serial.print(F(" ~ msg is: "));
			Serial.println(msg);
		}
	
		if (TRANSMIT_RESPONSES_USING_LORA_RADIO) {
			transmitMessageToLoraRadio(msg);
		}
		else {
			transmitMessageUsingSerialTerm(msg);
		}
		
		if (num_msgs > 1) {
		// Delay a short delay between transmission of multi-messages to give radio opportunity to handle
		// 500 works, 250 is a bit too short, 300 seems to work!
			delay(MULTI_MESSAGE_INTER_TRANSMISSION_DELAY_IN_MS);
		}
	}

	return;
}


void SlimSatBus::transmitMessageToLoraRadio(char* msg) {
	// This method transmits the given message using the LoRa Radio
	
	//Serial.println(F("\n ~ Transmitting Response Msg from LoRa Radio."));
	//Serial.println(msg);
		
	radio_status_code = LoRa.transmitUsingInterrupt(msg);
	
	if (VERBOSE_BUS_OUTPUT) {
		Serial.print(F(" ~ After Transmit, radio_status_code is: "));
		Serial.println(radio_status_code);
	}
	
	return;
}


void SlimSatBus::transmitMessageToLoraRadio(const char* msg) {
	// This method transmits the given message using the LoRa Radio
	
	radio_status_code = LoRa.transmitUsingInterrupt(msg);
	
	return;
}


void SlimSatBus::transmitMessageUsingSerialTerm(char* msg) {
	// This method transmits the given message using the Serial Terminal

	Serial.println(msg);
	
	return;
}
	

void SlimSatBus::transmitMessageFromSerialTermToLoraRadio(void) {
	// This method transmits the message received by Serial Terminal to LoRa Radio

	char* msg = getBusCmdFromSerialTerm();
	
	if (msg != nullptr) {
		transmitMessageToLoraRadio(msg);
	}
	
	return;
}


void SlimSatBus::transmitResponseMessageFromLoraRadioToSerialTerm(void) {
	// This method transmits the message received by LoRa Radio to the Serial Terminal

	char* msg = getReceivedMsgFromLoraRadio();
	transmitMessageUsingSerialTerm(msg);
		
	return;
}


void SlimSatBus::transmitLoraRadioTestMessage(void) {
	// This method transmits a test message by the LoRa Radio

	transmitMessageToLoraRadio(TEST_LORA_TRASMIT_MESSAGE);
		
	return;
}


void SlimSatBus::getReceivedRadioMessage(void) {
	// This method gets the received message from the LoRa Radio

	char* msg_ptr = getReceivedMsgFromLoraRadio();
	
	if (msg_ptr != nullptr) {
		//Serial.print(F("\n ~ Received LoRa Test Message is: "));
		Serial.print(F(" ~ Rx'd LoRa Msg: "));
		Serial.println(msg_ptr);
	}
	
	return;
}


/**
 * @brief Handle bus commands
 * @details Processes received commands using the NMEA message handler.
 * Validates commands, executes appropriate actions, and generates
 * response messages. Handles command routing to appropriate subsystems
 * and manages response transmission through the circular buffer.
 */
//void SlimSatBus::handleBusCmd(void) {
void SlimSatBus::handleBusCmd(char* bus_cmd_msg) {
	// This method handles Bus Commands
	
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Handling Bus Command ..."));
	}

	// Need to check that the ptr is not null, either here or at the next higher level
	//char* bus_cmd_msg = &(serial_term_received_chars[0]);
	//char* bus_response_msg = nullptr;

	if (0) {
		Serial.print(F("\n ~ serial_term_received_chars is: "));
		Serial.println((unsigned int)&serial_term_received_chars);
		Serial.println(serial_term_received_chars);

		Serial.print(F("\n ~ bus_cmd_msg is: "));
		Serial.println((unsigned int)*bus_cmd_msg);
		Serial.println(bus_cmd_msg);
	}
	
	Msg_handler.handleNmeaMsg(bus_cmd_msg, Bus_database, Payload1, LoRa, Gps, Bus_timer, Payload_timer, Beacon_timer);

	return;
}


/**
 * @brief Get input from serial terminal
 * @details Reads characters from the serial port and buffers them
 * until a complete command is received. Handles character-by-character
 * input processing with proper buffer management and newline detection.
 */
void SlimSatBus::getSerialTermInput(void) {
	// This method gets input from the serial terminal
	
	while (Serial.available() > 0 && new_serial_msg_rxd == 0) {
		char rc = Serial.read(); // Read one character from the serial buffer

		// Store character in array if not newline and within bounds
		if (rc != '\n' && serial_msg_data_index < MAX_SERIAL_TERM_BUFFER_CHARS - 1) {
			serial_term_received_chars[serial_msg_data_index] = rc;
			serial_msg_data_index++;
		} else {
			// If newline or buffer full, terminate string and set flag
			serial_term_received_chars[serial_msg_data_index] = '\0'; // Null-terminate the string
			new_serial_msg_rxd = 1;
			new_serial_msg_to_process = 1;
			serial_msg_data_index = 0; // Reset index for next message
		}
	}

	return;
}


/**
 * @brief Display new serial terminal data
 * @details Shows newly received serial terminal data for debugging
 * and confirmation purposes. Outputs received commands to the
 * serial console and resets the command received flag.
 */
void SlimSatBus::displayNewSerialTermData(void) {
	// This method displays new data that has been entered into the serial terminal
	
	if (new_serial_msg_rxd == 1) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.print("Received: ");
			Serial.println(serial_term_received_chars);	
		}
		
		new_serial_msg_rxd = 0; // Reset flag for next message
	}

	return;
}


/**
 * @brief Perform payload operations
 * @details Executes payload-specific operations based on current system mode.
 * In streaming mode, sends scripted payload commands for data collection.
 * In normal mode, performs standard payload operation iterations.
 * The commands can be scripted and potentially set by remote command.
 */
void SlimSatBus::performPayloadOpLoopIteration(void) {
	// This method performs a routine payload operation, getting payload data, and then writing it to flash memory

	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Performing Payload Op ..."));
	}
	
	// Perform the Payload Operation Iteration and return the Payload Data
	Payload1.performPayloadLoopIteration(Pl_data);
	
	// Write the Payload Data Rec to Memory
	Bus_database.Flash_Memory.writePayloadDataRecordToFlash(Pl_data);
	
	return;
}


void SlimSatBus::performPartialPayloadOp(void) {
	// This method performs a routine payload operation, getting payload data, and then writing it to flash memory

	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Performing Partial Payload Op ..."));
	}
	
	// Perform the Payload Operation Iteration and return the Payload Data
	Payload1.performPayloadLoopIteration(Pl_data);
	
	// Write the Payload Data Rec to Memory
	Bus_database.Flash_Memory.writePartialPayloadDataRecordToFlash(Pl_data);
	
	return;
}


uint8_t SlimSatBus::recordBusData(void) {
	// This method records bus data to flash memory
			
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Recording Bus Data to Flash Memory ..."));
	}
	
	// Collect the the bus data
	Bus_database.Bus_Power_Monitor.collectBusData(Bus_data);
	
	// Write the bus data to flash memory
	Bus_database.Flash_Memory.writeBusDataRecordToFlash(Bus_data);
	
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F(" ~ Recording Bus Data to Flash Memory Complete ..."));
	}
	
	return 1;
}


uint8_t SlimSatBus::recordPartialBusData(void) {
	// This method records bus data to flash memory
			
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F("\n ~ Recording Partial Bus Data to Flash Memory ..."));
	}
	
	// Collect the the bus data
	Bus_database.Bus_Power_Monitor.collectBusData(Bus_data);
	
	// Write the bus data to flash memory
	Bus_database.Flash_Memory.writePartialBusDataRecordToFlash(Bus_data);
	
	if (VERBOSE_BUS_OUTPUT) {
		Serial.println(F(" ~ Recording Partial Bus Data to Flash Memory Complete ..."));
	}
	
	return 1;
}