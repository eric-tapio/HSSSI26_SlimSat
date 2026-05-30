/**
 * @file nmea_lite_msg_handler.cpp
 * @brief NMEA Lite Message Handler Implementation
 * 
 * @details This file implements the NmeaMsgHandler class methods for processing
 * NMEA-style messages within the SlimSat system. Provides comprehensive message
 * validation, parsing, command execution, and response generation capabilities.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <nmea_lite_msg_handler.h>

/**
 * @brief Default constructor for the NMEA Message Handler class
 * @details Initializes the message handler with default configuration including
 * buffer initialization, message counters, and command processing variables.
 */
NmeaMsgHandler::NmeaMsgHandler(void) {
	// The default constructor, and only constructor, for the NMEA Message Handler class

	initializeMessageBuffers();
	initializeDataMembers();

	return;
}


void NmeaMsgHandler::initializeDataMembers(void) {
	// This method initialized the data members
	
	msg_is_valid = 0;
	cmd_id = 0;
	cmd_value = 0;	
	opt_cmd_arg_1 = -1;
	//opt_cmd_arg_2 = 0;
	
	// These two values will be overwritten by the values read from flash before used
	rxd_valid_msg_count = 0;
	rxd_invalid_msg_count = 0;
	
	// Construct the full SlimSat ID Header based on the SlimSat ID Number
	snprintf(slimsat_msg_header, sizeof(slimsat_msg_header), "$GS%02d", SLIMSAT_ID_NUM);
	slimsat_id = slimsat_msg_header;

	return;
}


/**
 * @brief Print message handler information
 * @details Outputs comprehensive handler state including message statistics,
 * current command information, and circular buffer status for debugging
 * and diagnostic purposes.
 */
void NmeaMsgHandler::print(void) const {
	// This method prints the NMEA Message Handler class members
	
	Serial.println(F("\n ~ Printing NMEA Msg Data ..."));
	Serial.print(F("    Full Msg Header is: "));
	Serial.println(full_msg_header);
	Serial.print(F("    Msg is Valid is: "));
	Serial.println(msg_is_valid);
	Serial.print(F("    Rx's Msg Count is: "));
	Serial.println(rxd_valid_msg_count);
	Serial.print(F("    Msg ID is: "));
	Serial.println(cmd_id);
	Serial.print(F("    Msg Value is: "));
	Serial.println(cmd_value);
	Serial.print(F("    Msg Optional Cmd Arg 1 Value is: "));
	Serial.println(opt_cmd_arg_1);
	//Serial.print(F("    Msg Optional Cmd Arg 2 Value is: "));
	//Serial.println(opt_cmd_arg_2);
	
	Serial.println(F("\n ~ Printing NMEA Msg Data Circular Buffer ..."));
	Bus_cbfr.print();

	return;
}


void NmeaMsgHandler::initializeMessageBuffers(void) {
	// This method initializess all buffers used

	initializeMessageBuffer();
	initializeTempMessageBuffer();
	initializeNmeaMsgHeader();

	return;
}


void NmeaMsgHandler::initializeMessageBuffer(void) {
	// This method initializes the message buffer used to output data to the SlimSat

	memset(bus_output_msg_buffer, 0, sizeof(bus_output_msg_buffer));
	
	return;
}


void NmeaMsgHandler::initializeTempMessageBuffer(void) {
	// This method initializes the temp message buffer used to output data to the SlimSat
	
	memset(temp_msg_buffer, 0, sizeof(temp_msg_buffer));
	
	return;
}


void NmeaMsgHandler::incrementValidMsgCount(BusDb& Bus_Db) {
	// This method increments the valid received message counter	
	// Write the value to flash
	rxd_valid_msg_count = Bus_Db.Flash_Memory.getValidMessageCount();	
	rxd_valid_msg_count = Bus_Db.Flash_Memory.incrementAndReturnValidMessageCount();

	return;
}


void NmeaMsgHandler::incrementInvalidMsgCount(BusDb& Bus_Db) {
	// This method increments the invalid received message counter
	
	rxd_invalid_msg_count = Bus_Db.Flash_Memory.incrementAndReturnInvalidMessageCount();

	return;
}


/**
 * @brief Validate NMEA message
 * @details Performs comprehensive message validation including:
 * - SlimSat ID verification
 * - Start/stop character validation
 * - Checksum verification
 * - Message format compliance
 * @param slimsat_msg Pointer to message string to validate
 * @return Validation result (0=invalid, 1=valid)
 */
uint8_t NmeaMsgHandler::nmeaMsgIsValid(char* slimsat_msg) {
	// This method determines whether or not the msg received is a valid message

	char* slimsat_msg_start_adr = nullptr;
	char* slimsat_msg_stop_adr = nullptr;
	char* comma_token_adr = nullptr;
	char* strfunc_result_ptr = nullptr;
	char* msg_adr = slimsat_msg;
	char slimsat_msg_checksum[NUM_HEX_CHARS_IN_CHECKSUM + 1] = { '\0' };
	uint8_t valid_start_char = 0;
	uint8_t valid_stop_char = 0;
	uint8_t msg_checksum = 0;
	uint8_t slimsat_msg_stop_index = 0;
	uint8_t checksum_is_valid = 0;
	uint8_t msg_index_offset = 0;
	uint8_t slimsat_id_matches = 0;

	reinitializeNmeaMsgHandlerMetadata();

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Determining if NMEA msg is valid ..."));
		Serial.print(F("    Input NMEA msg: "));
		Serial.println(slimsat_msg);
	}

	// Parse the message header by finding the end of the NMEA Msg preamble
	comma_token_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_DATA_TOKEN);

	if (comma_token_adr == nullptr) {
			Serial.println(F("\n ~ Warning! The NMEA Msg token character ',' was NOT found!"));
	}

	msg_index_offset = getCharIndexOffsetFromAdr(slimsat_msg, comma_token_adr);

	copyMsgHeader(msg_adr, strfunc_result_ptr, msg_index_offset);
					
	// Then check if the cmd is for this SlimSat ID
	if (strncmp(full_msg_header, slimsat_id, strlen(slimsat_id)) == STRNCMP_MATCHES_RESULT) {
		slimsat_id_matches = 1;
	}
	else {
		slimsat_id_matches = 0;
	}
	
	if (slimsat_id_matches) {
		// Find the NMEA Msg Start Char, '$'
		slimsat_msg_start_adr = slimsat_msg;
		slimsat_msg_stop_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_START_CHAR);

		if (slimsat_msg_stop_adr == nullptr) {
			valid_start_char = 0;

			if (VERBOSE_MSG_HANDLER_OUTPUT) {
				Serial.println(F("\n ~ Warning! The NMEA Msg start character '$' was NOT found"));
				Serial.print(F(" ~ Received Invalid NMEA message: "));
				Serial.println(slimsat_msg);
			}
		}
		else {
			// The NMEA start character was found
			valid_start_char = 1;
			msg_index_offset = getCharIndexOffsetFromAdr(slimsat_msg_start_adr, slimsat_msg_stop_adr);

			slimsat_msg = slimsat_msg_stop_adr;
			slimsat_msg_start_adr = slimsat_msg;

			slimsat_msg_stop_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_STOP_CHAR);

			if (slimsat_msg_stop_adr == nullptr) {
				valid_stop_char = 0;

				if (VERBOSE_MSG_HANDLER_OUTPUT) {
					Serial.println(F("\n ~ Warning! The NMEA Msg stop character '*' was NOT found"));
					Serial.print(F(" ~ Received Invalid NMEA message: "));
					Serial.println(slimsat_msg);
				}
			}
			else {
				// The NMEA stop character was found
				msg_index_offset = getCharIndexOffsetFromAdr(slimsat_msg_start_adr, slimsat_msg_stop_adr);
				slimsat_msg_stop_index = msg_index_offset - 1; // -1 to exclude the '*' char

				// Parse the 2 digit checksum
				msg_index_offset += NUM_HEX_CHARS_IN_CHECKSUM;

				if (msg_index_offset <= strlen(slimsat_msg)) {
					// Extract the checksum
					
					// Advance the stopchar address by one to point to the first checksum digit
					slimsat_msg_stop_adr++;

					// Copy the checksum value
					strfunc_result_ptr = strncpy(slimsat_msg_checksum, slimsat_msg_stop_adr, sizeof(slimsat_msg_checksum) - 1);
					*(slimsat_msg_checksum + (sizeof(slimsat_msg_checksum) - 1)) = '\0'; // to guarnatee that the copied str is a valid string

					if (strfunc_result_ptr == nullptr) {
						if (VERBOSE_MSG_HANDLER_OUTPUT) {
							Serial.println(F("\n ~ Warning: strncpy returned a null pointer in attempting to copy the NMEA Msg checksum!"));
						}
					}
					else {
						// Convert the checksum to a numeric value
						msg_checksum = (uint8_t)strtol(slimsat_msg_checksum, &strfunc_result_ptr, HEXIDECIMAL_BASE);
					}

					if (strfunc_result_ptr == nullptr) {
						if (VERBOSE_MSG_HANDLER_OUTPUT) {
							Serial.println(F("\n ~ Warning: strtol returned a null pointer in attempting to convert the parsed NMEA Msg checksum value!"));
						}
					}
					else
						checksum_is_valid = checksumIsValid(slimsat_msg, msg_checksum);

					if (checksum_is_valid) {
						msg_is_valid = 1;
					}
					else {
						msg_is_valid = 0;
						if (VERBOSE_MSG_HANDLER_OUTPUT) {
							Serial.println(F("\n ~ Warning: Checksum is NOT valid!"));
							Serial.print(F(" ~ Warning: Received Invalid NMEA msg: "));
							Serial.println(slimsat_msg);
						}
					}
				}
				else {
					if (VERBOSE_MSG_HANDLER_OUTPUT) {
						Serial.println(F("\n ~ The NMEA Msg has NO 2-digit Hex Checksum!"));
						Serial.print(F(" ~ Received Invalid NMEA message: "));
						Serial.println(slimsat_msg);
					}
				}
			}
		}
	
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.print(F("\n ~ NMEA msg is valid: "));
			Serial.println(msg_is_valid);
		}
	}
	else {
		msg_is_valid = 0;
		
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ The NMEA Msg SlimSat ID does not match this SlimSat ID"));
		}
	}

	return msg_is_valid;
}


uint8_t NmeaMsgHandler::checksumIsValid(char* slimsat_msg, uint8_t message_checksum) {
	// This method determines if the message checksum is valid
	
	uint8_t computed_checksum = 0;

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Checking Checksum ..."));
	}

	computed_checksum = computeChecksum(slimsat_msg);

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Message Checksum is (hex): "));
		if (message_checksum < 0x10) {
			// Manually print a leading zero if the value is a single digit
			Serial.print("0"); 
		}
		Serial.println(message_checksum, HEX);
		Serial.print(F(" ~ Computed Checksum is (hex): "));
		if (computed_checksum < 0x10) {
			// Manually print a leading zero if the value is a single digit
			Serial.print("0"); 
		}
		Serial.println(computed_checksum, HEX);
	}

	if (computed_checksum == message_checksum)
		return 1;
	else
		return 0;
}


/**
 * @brief Compute message checksum
 * @details Calculates XOR checksum for NMEA message between start ('$') and
 * stop ('*') characters. Uses standard NMEA checksum algorithm.
 * @param slimsat_msg Pointer to message string
 * @return Computed checksum value (0-255)
 */
uint8_t NmeaMsgHandler::computeChecksum(char* slimsat_msg) {
	// This method computes the checksum of the given message
	
	char* SLIMSAT_start_token_adr = nullptr;
	char* slimsat_end_token_adr = nullptr;
	uint8_t xor_val = 0;
	uint8_t start_index = 0;
	uint8_t end_index = 0;

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Computing Checksum ..."));
	}

	SLIMSAT_start_token_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_START_CHAR);
	slimsat_end_token_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_STOP_CHAR);
	if ((SLIMSAT_start_token_adr != nullptr) && (slimsat_end_token_adr != nullptr)) {
		start_index = SLIMSAT_start_token_adr + 1 - slimsat_msg; // + 1 to skip over the '$'
		end_index = slimsat_end_token_adr - slimsat_msg; // The < end_index will ensure that the '*' is not included

		xor_val = computeXorChecksumValue(slimsat_msg, start_index, end_index);
	}
	else if (SLIMSAT_start_token_adr == nullptr) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ Warning: No NMEA Msg start $ char found in NMEA Msg:"));
			Serial.print(slimsat_msg);
		}
	}
	else if (slimsat_end_token_adr == nullptr) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ Warning: No NMEA Msg end * char found in NMEA Msg:"));
			Serial.print(slimsat_msg);
		}
	}
	else {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ Error: Unhandled case in computeChecksum!"));
		}
	}

	return xor_val;
}


void NmeaMsgHandler::initializeNmeaMsgHeader(void) {
	// This method initializes the NMEA message header buffer

	memset(full_msg_header, 0, sizeof(full_msg_header));
	
	return;
}


void NmeaMsgHandler::reinitializeNmeaMsgHandlerMetadata(void) {
	// This method initializes the NMEA message metadata
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Reinitializing NMEA Msg Metadata ..."));
	}

	initializeNmeaMsgHeader();

	msg_is_valid = 0;

	if (VERBOSE_MSG_HANDLER_OUTPUT){
		print();
	}

	return;
}


uint8_t NmeaMsgHandler::getCharIndexOffsetFromAdr(const char* slimsat_msg_start_adr, const char* slimsat_msg_stop_adr) {
	// This method gets the offset of a given character in a char array
	
	uint8_t indexOffset = INITIAL_INVALID_VALUE;

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Getting Char Index Offset from Adr ..."));
	}

	if ((slimsat_msg_start_adr == nullptr) || (slimsat_msg_stop_adr == nullptr)) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ Warning: Either slimsat_msg_start_adr or slimsat_msg_stop_adr is NULL! ..."));
		}
	}
	else {
		indexOffset = (slimsat_msg_stop_adr - slimsat_msg_start_adr) / sizeof(char);
	}

	return indexOffset;
}


void NmeaMsgHandler::copyMsgHeader(char* msg_adr, char* strfunc_result_ptr, uint8_t msg_index_offset) {
	// This method copies the message header to a char array

	strfunc_result_ptr = strncpy(full_msg_header, msg_adr, msg_index_offset);
	*(full_msg_header + msg_index_offset) = '\0'; // to guarnatee that the copied str is a valid string

	if (strfunc_result_ptr == nullptr) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ Warning: strncpy returned a null pointer in attempting to copy to NMEA Msg Header!"));
		}
	}

	return;
}


uint8_t NmeaMsgHandler::computeXorChecksumValue(const char* slimsat_msg, uint8_t start_index, uint8_t end_index) {
	// This method uses the Exlcusive Or logic to compute the checksum value of the given message
	
	uint8_t xor_val = 0;
	uint8_t msg_index = 0;

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Computing Checksum ..."));
	}
	
	for (msg_index = start_index; msg_index < end_index; msg_index++)
		xor_val = xor_val ^ slimsat_msg[msg_index];

	return xor_val;
}


/**
 * @brief Handle NMEA message
 * @details Main message processing method that validates messages, parses commands,
 * and routes them to appropriate subsystems. Generates ACK/NACK responses and
 * manages command execution for all bus operations.
 * @param slimsat_msg Message string to process
 * @param Bus_Db Spacecraft database reference
 * @param Payload Payload system reference
 * @param LoRa LoRa radio reference
 * @param Pl_Timer Payload timer reference
 * @param Bcn_Timer Beacon timer reference
 */
void NmeaMsgHandler::handleNmeaMsg(char* slimsat_msg, BusDb& Bus_Db, Payload& Payload, LoRaRadio& LoRa,  GpsIf& Gps, SimpleTimer& Bus_Timer, SimpleTimer& Pl_Timer, SimpleTimer& Bcn_Timer) {
	// This method handles received cmd messages
	
	int32_t int32_val = 0;
	uint32_t uint32_val = 0;
	double double_val = 0.0;
	int16_t radio_status_code = 0;
	uint8_t num_bytes_read = 0;
	uint8_t data_rec_num_is_valid = 0;
	char* gps_pos_msg = nullptr;
	
	// For now, the cmd val will be included in the acknowledgement for set commands, to be left off for get commands since data is returned
	
	// Reinitialize the Cmd ID to 0
	cmd_id = 0;

	if (nmeaMsgIsValid(slimsat_msg)) {
		parseMsg(slimsat_msg);
		incrementValidMsgCount(Bus_Db);

		if ((MIN_SLIMSAT_CMD_ID <= cmd_id) && (cmd_id <= MAX_SLIMSAT_CMD_ID)) {
			switch (cmd_id) {
			case PING_BUS_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Ping Command ..."));
				}
				constructAckMsg();
				if (0) {
					Serial.println(F(" ~ Ping Cmd received"));
				}
				break;
			case REBOOT_COMMAND_ID:
								
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F("\n ~ Handling Rx'd Reboot Command ..."));
				}
				constructAckMsg();
				
				// The will never transimt the acknowledgement
				Bus_Db.Bus_Power_Monitor.rebootBus();
					
				break;
			case GET_PAYLOAD_DATA_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Payload Data Mode Command ..."));
				}
				//constructAckMsg();
				
				// If the cmd_value is zero, then get new, current Payload Data and transmit it
				if (cmd_value == 0) {
					// Get new, current Payload Data
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F("\n ~ Getting new, current Payload Data ..."));
					}
					Payload.performPayloadLoopIteration(pl_data_rec);
					//pl_data_rec.printRecord();
					constructPayloadDataResponseMsg(pl_data_rec);
				
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Payload Data Sent"));
					}
				}
				//else if ((cmd_value != 0) && (opt_cmd_arg_1 == 0)) {
				else if ((cmd_value > 0) && (opt_cmd_arg_1 == 0)) {
					// Note: a cmd value of 0 is invalid, and hence it is protected against here
					// Get the cmd value specified bus data record from flash memory
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F("\n ~ Getting single Payload Data record ..."));
					}
					
					data_rec_num_is_valid = Bus_Db.Flash_Memory.getPayloadDataRecordFromFlash(cmd_value, pl_data_rec);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ data_rec_num_is_valid is: "));
						Serial.println(data_rec_num_is_valid);
					}
				
					if (data_rec_num_is_valid) {
						//pl_data_rec.printRecord();
						constructPayloadDataResponseMsg(pl_data_rec);
					}
					else {
						constructNackMsg();
					}
				}
				//else if ((cmd_value != 0) && (opt_cmd_arg_1 != 0)) {
				else if ((cmd_value > 0) && (opt_cmd_arg_1 != 0)) {
					// Get the range of bus data records starting from cmd value specified value through the opt cmd arg 1 value from flash memory
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F("\n ~ Getting range of Payload Data records..."));
					}
					
					for (uint16_t i = cmd_value; i < (cmd_value + opt_cmd_arg_1); i++) {
						// Get the Payload Data struct
						if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
							Serial.print(F("\n -> Getting Payload Data Record: "));
							Serial.println(i);
						}
						
						data_rec_num_is_valid = Bus_Db.Flash_Memory.getPayloadDataRecordFromFlash(i, pl_data_rec);
					
						if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
							Serial.print(F(" ~ data_rec_num_is_valid is: "));
							Serial.println(data_rec_num_is_valid);
						}
					
						if (data_rec_num_is_valid) {
							//pl_data_rec.printRecord();
							constructPayloadDataResponseMsg(pl_data_rec);
						}
						else {
							constructNackMsg();
						}
					}
				}
				break;
			case GET_BUS_SAFE_MODE_VOLTAGE_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Safe Mode Voltage Command ..."));
				}
				uint32_val = Bus_Db.getBusSafeModeVoltageInMv();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
								
				break;				
			case SET_BUS_SAFE_MODE_VOLTAGE_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Safe Mode Voltage Command ..."));
				}
				Bus_Db.setBusSafeModeVoltageInMv(cmd_value);
				constructAckMsg(1);
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
				Serial.println(F(" ~ Note: Safe Mode Voltage Set"));
				}				
				break;
			case GET_RADIO_FREQUENCY_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Frequency Command ..."));
				}
				uint32_val = LoRa.getFrequency();
				
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Frequency is: "));
					Serial.print(uint32_val);
					Serial.println(F(" Hz"));
				}
				break;
			case SET_RADIO_FREQUENCY_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Radio Frequency Command ..."));
				}
				
				radio_status_code = LoRa.setFrequency(cmd_value);
				
				if (radio_status_code == 0) {
					constructAckMsg(1);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ LoRa Frequency has been set to: "));
						Serial.print(cmd_value);
						Serial.println(F(" Hz"));
					}
				}
				else {
					constructNackMsg(radio_status_code);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: LoRa Frequency was NOT successfully set! "));
					}
				}					
				break;
			case GET_RADIO_BANDWIDTH_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Bandwidth Command ..."));
				}
				uint32_val = LoRa.getBandwidth();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Bandwidth is: "));
					Serial.print(uint32_val);
					Serial.println(F(" Hz"));
				}
				break;
			case SET_RADIO_BANDWIDTH_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Radio Bandwidth Command ..."));
				}
				
				radio_status_code = LoRa.setBandwidth(cmd_value);
				
				if (radio_status_code == 0) {
					constructAckMsg(1);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ LoRa Bandwidth has been set to: "));
						Serial.print(cmd_value);
						Serial.println(F(" Hz"));
					}
				}
				else {
					constructNackMsg(radio_status_code);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: LoRa Bandwidth was NOT successfully set! "));
					}
				}
				break;
			case GET_RADIO_SPREAD_FACTOR_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Spread Factor Command ..."));
				}
				uint32_val = LoRa.getSpreadFactor();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Spread Factor is: "));
					Serial.println(uint32_val);
				}
				break;
			case SET_RADIO_SPREAD_FACTOR_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Radio Spread Factor Command ..."));
				}
				radio_status_code = LoRa.setSpreadFactor(cmd_value);

				if (radio_status_code == 0) {
					constructAckMsg(1);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ LoRa Spread Factor has been set to: "));
						Serial.println(cmd_value);
					}
				}
				else {
					constructNackMsg(radio_status_code);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: LoRa Spread Factor was NOT successfully set! "));
					}
				}
				break;							
			case GET_RADIO_CODING_RATE_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Coding Rate Command ..."));
				}
				uint32_val = LoRa.getCodingRate();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Coding Rate is: "));
					Serial.println(uint32_val);
				}
				break;
			case SET_RADIO_CODING_RATE_COMMAND_ID:			
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Radio Coding Rate Command ..."));
				}
				radio_status_code = LoRa.setCodingRate(cmd_value);
				
				if (radio_status_code == 0) {
					constructAckMsg(1);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ LoRa Coding Rate has been set to: "));
						Serial.println(cmd_value);
					}
				}
				else {
					constructNackMsg(radio_status_code);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: LoRa Coding Rate was NOT successfully set! "));
					}
				}
				break;							
			case GET_RADIO_OUTPUT_POWER_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Output Power Command ..."));
				}
				int32_val = LoRa.getOutputPower();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%ld,%d", int32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Output Power is: "));
					Serial.println(int32_val);
				}
				break;
			case SET_RADIO_OUTPUT_POWER_COMMAND_ID:	
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Radio Output Power Command ..."));
				}
				radio_status_code = LoRa.setOutputPower(cmd_value);
				
				if (radio_status_code == 0) {
					constructAckMsg(1);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ LoRa Output Power has been set to: "));
						Serial.println(cmd_value);
					}
				}
				else {
					constructNackMsg(radio_status_code);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: LoRa Output Power was NOT successfully set! "));
					}
				}
				break;
			case GET_RADIO_CURRENT_LIMIT_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Current Limit Command ..."));
				}
				uint32_val = LoRa.getCurrentLimit();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Current Limit is: "));
					Serial.print(uint32_val);
					Serial.println(F(" mA"));
				}
				break;
			case SET_RADIO_CURRENT_LIMIT_COMMAND_ID:
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Radio Current Limit Command ..."));
				}
				
				radio_status_code = LoRa.setCurrentLimit(cmd_value);
				
				if (radio_status_code == 0) {
					constructAckMsg(1);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ LoRa Current Limit has been set to: "));
						Serial.print(cmd_value);
						Serial.println(F(" mA"));
					}
				}
				else {
					constructNackMsg(radio_status_code);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: LoRa Current Limit was NOT successfully set! "));
					}
				}
				break;
			case SET_RADIO_PREAMBLE_LENGTH_COMMAND_ID:	
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Radio Preamble Length Command ..."));
				}
				constructAckMsg(1);
				LoRa.setPreambleLength(cmd_value);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Preamble Length has been set to: "));
					Serial.println(cmd_value);
				}
				break;
			case GET_RADIO_PREAMBLE_LENGTH_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Preamble Length Command ..."));
				}
				uint32_val = LoRa.getPreambleLength();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Preamble Length is: "));
					Serial.println(uint32_val);
				}
				break;
			case SET_RADIO_GAIN_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Radio Gain Command ..."));
				}
				
				radio_status_code = LoRa.setGain(cmd_value);
				
				if (radio_status_code == 0) {
					constructAckMsg(1);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ LoRa Gain has been set to: "));
						Serial.println(cmd_value);
					}
				}
				else {
					constructNackMsg(radio_status_code);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: LoRa Gain was NOT successfully set! "));
					}
				}						
				break;
			case GET_RADIO_GAIN_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Gain Command ..."));
				}
				uint32_val = LoRa.getGain();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Gain is: "));
					Serial.print(uint32_val);
					Serial.println(" dB");
				}
				break;
			case GET_RADIO_FREQUENCY_ERROR_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio Frequency Error Command ..."));
				}
				double_val = LoRa.getFrequencyError();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%1.2f,%d", double_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa Frequency Error is: "));
					Serial.print(double_val);
					Serial.println(" Hz");
				}
				break;
			case GET_RADIO_SNR_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio SNR Command ..."));
				}
				double_val = LoRa.getSnr();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%1.2f,%d", double_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa SNR is: "));
					Serial.print(double_val);
					Serial.println(" dBm");
				}
				break;
			case GET_RADIO_RSSI_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Radio RSSI Command ..."));
				}
				double_val = LoRa.getRssi();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%1.2f,%d", double_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ LoRa RSSI is: "));
					Serial.print(double_val);
					Serial.println(" dBm");
				}
				break;
			case GET_BEACON_PERIOD_COMMAND_ID:
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Beacon Period Command ..."));
				}
				uint32_val = Bcn_Timer.getIntervalInMs();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Beacon Period is: "));
					Serial.print(uint32_val);
					Serial.println(F(" ms"));
				}
				break;
			case SET_BEACON_PERIOD_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Beacon Period Command ..."));
				}
				constructAckMsg(1);
				uint32_val = Bcn_Timer.setIntervalInMs(cmd_value);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Beacon Period has been set to: "));
					Serial.print(cmd_value);
					Serial.println(F("  ms"));							
				}
				break;
			case GET_BUS_MODE_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Bus Mode Command ..."));
				}
				uint32_val = Bus_Db.getBusMode();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Bus Mode is: "));
					Serial.println(uint32_val);
				}
				break;
			case SET_BUS_MODE_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F("\n ~ Handling Rx'd Set Bus Mode Command ..."));
				}
				constructAckMsg(1);
				Bus_Db.setBusMode((uint8_t)cmd_value);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Bus Mode set to: "));
					Serial.println(cmd_value);
				}
				break;
			case GET_BUS_EPOCH_TIME_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Bus Epoch Time Command ..."));
				}
				uint32_val = Bus_Db.Bus_Time.getBusEpochTimeInMs();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Bus Time is: "));
					Serial.print(uint32_val);
					Serial.println(F("  ms"));
				}
				break;
			case SET_BUS_EPOCH_TIME_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Bus Epoch Time Command ..."));
				}
				constructAckMsg(1);
				Bus_Db.Bus_Time.setBusEpochTimeInMs(cmd_value);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Bus Epoch Time set to: "));
					Serial.print(uint32_val);
					Serial.println(F("  ms"));	
				}
				break;
			case GET_BUS_TIME_COMMAND_ID:			
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Bus Time Command ..."));
				}
				uint32_val = Bus_Db.Bus_Time.getBusTimeInMs();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Bus Time is: "));
					Serial.print(uint32_val);
					Serial.println(F("  ms"));
				}
				break;
			case GET_BUS_DATA_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Bus Data Command ..."));
				}
				// If no arguments, then collect the current data. Otherwise retreive and send

				// If the cmd_value is zero, then get new, current Bus Data and transmit it
				if (cmd_value == 0) {
					// Get new, current Bus Data
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Getting new, current Bus Data ..."));
					}
					
					Bus_Db.Bus_Power_Monitor.collectBusData(bus_data);
					constructBusDataResponseMsg(bus_data);		
				}
				//else if ((cmd_value != 0) && (opt_cmd_arg_1 == 0)) {
				else if ((cmd_value > 0) && (opt_cmd_arg_1 == 0)) {
					// Note: a cmd value of 0 is invalid, and hence it is protected against here
					// Get the cmd value specified bus data record from flash memory
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Getting single Bus Data record ..."));
					}
					data_rec_num_is_valid = Bus_Db.Flash_Memory.getBusDataRecordFromFlash(cmd_value, bus_data);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ data_rec_num_is_valid is: "));
						Serial.println(data_rec_num_is_valid);
					}
				
					if (data_rec_num_is_valid) {
						//bus_data.printRecord();
						constructBusDataResponseMsg(bus_data);
					}
					else {
						constructNackMsg();
					}
				}
				//else if ((cmd_value != 0) && (opt_cmd_arg_1 != 0)) {
				else if ((cmd_value > 0) && (opt_cmd_arg_1 != 0)) {
					// Get the range of bus data records starting from cmd value specified value through the opt cmd arg 1 value from flash memory
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Getting range of Bus Data records..."));
					}
					
					Serial.println(" ~ About to enter loop ...");
					
					Serial.print(F(" ~ cmd_value is: "));
					Serial.println(cmd_value);
					
					Serial.print(F(" ~ ropt_cmd_arg_1 is: "));
					Serial.println(opt_cmd_arg_1);
					
					for (uint16_t i = cmd_value; i < (cmd_value + opt_cmd_arg_1); i++) {
						// Get the Bus Data struct
						
						if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
							Serial.print(F(" ~ retreiving Bus Data Rec: "));
							Serial.println(i);
						}
						
						data_rec_num_is_valid = Bus_Db.Flash_Memory.getBusDataRecordFromFlash(i, bus_data);
					
						if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
							Serial.print(F(" ~ data_rec_num_is_valid is: "));
							Serial.println(data_rec_num_is_valid);
						}
					
						if (data_rec_num_is_valid) {
							//bus_data.printRecord();
							constructBusDataResponseMsg(bus_data);
						}
						else {
							constructNackMsg();
						}
					}
					
					Serial.println(" ~ Done with loop ...");
				}
				//else { do nothing}
				// opt_cmd_arg_1
				// If the cmd value is not zero, and the optional value 1 is zero then it represents the Bus Data record. Retrieve the record and transmit
				// If the cmd value is not zero and the optional value 1 is also not zero, then it represents the range of Bus data records to transmit.
				
				break;
			case GET_BUS_DB_REGISTER_VALUES_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Bus Database Register Values Command ..."));
				}
				Bus_Db.Flash_Memory.getBusDatabaseRegisterValues(bus_db_reg_data);
				constructBusDbRegisterValueResponseMsg(bus_db_reg_data);
				break;
			case GET_PAYLOAD_OP_PERIOD_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Payload Op Period Command ..."));
				}
				uint32_val = Pl_Timer.getIntervalInMs();
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Payload Op Period is: "));
					Serial.print(uint32_val);
					Serial.println(F(" ms"));
				}
				break;
			case SET_PAYLOAD_OP_PERIOD_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Payload Op Period Command ..."));
				}
				Pl_Timer.setIntervalInMs(cmd_value);
				constructAckMsg(1);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Payload Op Period has been set to: "));
					Serial.print(cmd_value);
					Serial.println(F(" ms"));
				}
				break;			
			case GET_BUS_DATA_RECORD_PERIOD_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Bus Record Period Command ..."));
				}
				uint32_val = Bus_Timer.getIntervalInMs();
				
				snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_val, rxd_valid_msg_count);
				constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Bus Data Record Period is: "));
					Serial.print(uint32_val);
					Serial.println(F(" ms"));
				}
				break;
			case SET_BUS_DATA_RECORD_PERIOD_COMMAND_ID:				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Bus Record Period Command ..."));
				}
				Bus_Timer.setIntervalInMs(cmd_value);
				constructAckMsg(1);
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~  Bus Data Record Period has been set to: "));
					Serial.print(cmd_value);
					Serial.println(F(" ms"));
				}
				break;
			case GET_FLASH_REGISTER_VALUE_COMMAND_ID:
				// Need to put validation code here - Still TBD
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get Flash Register Command ..."));
				}
				
				num_bytes_read = Bus_Db.Flash_Memory.readValueFromFlashAddress(cmd_value, &uint32_val);
				
				if (num_bytes_read > 0) {
					constructGetFlashRegisterValueResponseMsg(uint32_val);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ Value read from address "));
						Serial.print(cmd_value);
						Serial.print(F(": "));
						Serial.println(uint32_val);
					}
				}
				else {
					constructNackMsg(num_bytes_read);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: Value not read from Flash Memory! "));
					}
				}
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Register Value is: "));
					Serial.println(uint32_val);
				}
				break;
			case SET_FLASH_REGISTER_VALUE_COMMAND_ID:
				// Need to put validation code here - TBD
				// This has to be a Sector value, you have to erase it before you can write it
				// This has not yet been implemented
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Set Flash Register Command ..."));
				}
				num_bytes_read = Bus_Db.Flash_Memory.writeValueToFlashAddress(cmd_value, opt_cmd_arg_1);
				
				if (num_bytes_read > 0) {
					constructSetFlashRegisterValueResponseMsg(opt_cmd_arg_1);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.print(F(" ~ Value wrote to address "));
						Serial.print(cmd_value);
						Serial.print(F(": "));
						Serial.println(opt_cmd_arg_1);
					}
				}
				else {
					constructNackMsg(num_bytes_read);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: Value not written to Flash Memory! "));
					}
				}
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F(" ~ Register Value: "));
					Serial.println(opt_cmd_arg_1);
				}
				break;
			case ERASE_FLASH_CHIP_MEMORY_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Erase Flash Chip Command ..."));
				}	
				num_bytes_read = Bus_Db.Flash_Memory.flashEraseChip();
				
				if (num_bytes_read > 0) {
					snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%d",  rxd_valid_msg_count);
					constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Flash Chip Memory Erased"));
					}
				}
				else {
					constructNackMsg(num_bytes_read);
					
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: Chip Flash Memory Not Erased! "));
					}
				}
				break;
			case ERASE_FLASH_BLOCK_MEMORY_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Erase Flash Block Command ..."));
				}
				// Validate Input. Cmd val needs to be greater than 0
				if (cmd_value >= 0) {
					num_bytes_read = Bus_Db.Flash_Memory.flashEraseBlock(cmd_value);
					if (num_bytes_read > 0) {
						snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%d", rxd_valid_msg_count);
						constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
						
						if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
							Serial.print(F(" ~ Flash Block: "));
							Serial.print(cmd_value);
							Serial.println(F(" Memory Erased"));
						}
					}
					else {
						constructNackMsg(num_bytes_read);
						
						if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
							Serial.println(F(" ~ Error: Flash Memory Block Not Erased!"));
						}
					}
				}
				else {
					constructNackMsg(num_bytes_read);
						
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: Ignoring invalid Block Number! "));
					}
				}
				break;
			case ERASE_FLASH_SECTOR_MEMORY_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Erase Flash Sector Command ..."));
				}
				// Validate Input. Cmd val needs to be greater than 0
				if (cmd_value >= 0) {
					num_bytes_read = Bus_Db.Flash_Memory.flashEraseSector(cmd_value);
					if (num_bytes_read > 0) {
						snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%d", rxd_valid_msg_count);
						constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
						
						if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
							Serial.print(F(" ~ Flash Sector: "));
							Serial.print(cmd_value);
							Serial.println(F(" Memory Erased"));
						}
					}
					else {
						constructNackMsg(num_bytes_read);
						
						if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
							Serial.println(F(" ~ Error: Flash Memory Sector Not Erased!"));
						}
					}
				}
				else {
					constructNackMsg(num_bytes_read);
						
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Error: Ignoring invalid Sector Number! "));
					}
				}
				break;
			case GET_GPS_POSITION_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Get GPS Position Command ..."));
				}
				
				gps_pos_msg = Gps.getGpsPositionMessage();
  
				if (gps_pos_msg != nullptr) {
					constructGpsPositionResponseMsg(gps_pos_msg);
				}
				else {
					if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
						Serial.println(F(" ~ Returned GPS Msg is Nullptr ..."));
					}				
				}
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F(" ~ GPS Position Msg Sent."));
				}
				break;
			case ERASE_BUS_DATA_FLASH_BLOCKS_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Erase Bus Data Flash Sectors Command ..."));
				}
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F(" ~  Bus Data Flash Sectors Erased - TBV"));
				}
				
				constructAckMsg();
				Bus_Db.Flash_Memory.flashEraseAllBusDataRecordBlocks();
	
				break;
			case ERASE_PAYLOAD_DATA_FLASH_BLOCKS_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd  Erase Payload Data Flash Sectors Command ..."));
				}
				constructAckMsg();
				
				Bus_Db.Flash_Memory.flashEraseAllPayloadDataRecordBlocks();
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F(" ~ Payload Flash Sectors Erased - TBV"));
				}
				break;
			case CUTDOWN_BURN_WIRE_COMMAND_ID:
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F("\n ~ Handling Rx'd Cutdown / Burn wire Command ..."));
				}
				constructAckMsg();
				Bus_Db.Bus_Power_Monitor.initiateCutdown();
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.println(F(" ~ Cutdown initiated ..."));
				}
				break;
			case SLIMSAT_CMD_47:
			case SLIMSAT_CMD_48:
			case SLIMSAT_CMD_49:
				constructNackMsg();
				break;
			// case PAYLOAD_CMD_1:
			// case PAYLOAD_CMD_2:
			// case PAYLOAD_CMD_3:
			// case PAYLOAD_CMD_4:
			// case PAYLOAD_CMD_5:
			// case PAYLOAD_CMD_6:
			// case PAYLOAD_CMD_7:
			// case PAYLOAD_CMD_8:
			// case PAYLOAD_CMD_9:
			// case PAYLOAD_CMD_10:
				// handlePayloadCmd(Payload, cmd_id, cmd_value);
				// break;
			default:
				
				if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
					Serial.print(F("\n ~ Encountered Unhandled SlimSat Command ID: "));
					Serial.print(cmd_id);
					Serial.println(F(". Ignoring!"));
				}
				break;
			}
		}
		else {
			if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
				Serial.print(F("\n ~ Encountered Invalid Bus Command ID: "));
				Serial.print(cmd_id);
				Serial.println(F(". Ignoring!"));
			}
			constructNackMsg();
			incrementInvalidMsgCount(Bus_Db);
		}
	}
	else {
		if (VERBOSE_MSG_HANDLER_OUTPUT_TO_USER) {
			Serial.print(F("\n ~ Encountered Invalid SlimSat Msg ID: "));
			Serial.print(cmd_id);
			Serial.println(F(". Ignoring!"));
		}
		// Then do nothing
		// Do not respond to message, nor increment anything
		constructNackMsg(-1);
		incrementInvalidMsgCount(Bus_Db);
	}

	return;
}


// void NmeaMsgHandler::handlePayloadCmd(Payload& Payload, uint8_t command_id, int32_t command_value) {
	// Serial.print(F("\n ~ Handling Rx'd Payload Command ..."));
	// char* pl_msg = nullptr;
		
	// pl_msg = Payload.handlePayloadCommand(command_id, command_value, pl_data_rec);	
	// pl_msg = Payload.getPayloadDataStr();
		
	// constructResponseMsgGivenMsgPayload(command_id, 1, pl_msg);

	// return;
// }


//void NmeaMsgHandler::constructAckMsg(uint8_t include_cmd_val) {
void NmeaMsgHandler::constructAckMsg(int8_t include_cmd_val) {
	// This method constructs an Acknowledged message to the received command message
	// SlimSat Acknowledgement Msg
	// $GS01,A,cmd_id*hh<CR><LF>
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Constructing SlimSat ACK Msg ..."));
		Serial.print(F("   - cmd_id is: "));
		Serial.println(cmd_id);
		Serial.print(F("   - rxd_valid_msg_count is: "));
		Serial.println(rxd_valid_msg_count);
	}
	
	if (include_cmd_val) {
		//snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", cmd_value, rxd_valid_msg_count);
		snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%ld,%d", cmd_value, rxd_valid_msg_count);
	}
	else {
		snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%d", rxd_valid_msg_count);
	}
	constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);

	return;
}


// void NmeaMsgHandler::constructReceivedInvalidMsgNackMsg(void) {
	// // This method constructs an Not Acknowledged message to the received command message
	// // SlimSat Acknowledgement Msg
	// // $GS01,N,-1*hh<CR><LF>

	// if (VERBOSE_MSG_HANDLER_OUTPUT) {
		// Serial.print(F("\n ~ Constructing Received Invalid Msg SlimSat NACK Msg ..."));
	// }
	
	// snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%ld", 255);
	// constructResponseMsgGivenMsgPayload(cmd_id, 0, temp_msg_buffer);

	// return;
// }


void NmeaMsgHandler::constructNackMsg(int16_t error_value) {
	// This method constructs an Not Acknowledged message to the received command message
	// SlimSat Acknowledgement Msg
	// $GS01,N,cmd_id*hh<CR><LF>

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Constructing SlimSat NACK Msg ..."));
	}
	
	//snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu", cmd_value);
	snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%ld", cmd_value);
	constructResponseMsgGivenMsgPayload(cmd_id, 0, temp_msg_buffer);

	return;
}


void NmeaMsgHandler::constructResponseMsg(char* msg) {
	// This method constructs a formatted response message, given a message "payload" 

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Constructing SlimSat Response Msg ..."));
	}
	
	uint8_t msg_int_checksum = 0;
	char checksum_bfr[3] = {'\0'};

	// Construct the reply
	//snprintf(bus_output_msg_buffer, sizeof(bus_output_msg_buffer), "$%sG,%s*", SLIMSAT_ID, msg);
	snprintf(bus_output_msg_buffer, sizeof(bus_output_msg_buffer), "$S%02dG,%s*", SLIMSAT_ID_NUM, msg);
	msg_int_checksum = computeChecksum(bus_output_msg_buffer);
	snprintf(checksum_bfr, sizeof(checksum_bfr), "%02X", msg_int_checksum);
	strcat(bus_output_msg_buffer, checksum_bfr);

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Full SlimSat Response msg is: "));
		Serial.println(bus_output_msg_buffer);
		Serial.print(F(" ~ SlimSat Response msg strlen is: "));
		Serial.println(strlen(bus_output_msg_buffer));
	}

	if (strlen(bus_output_msg_buffer) > MAX_SLIMSAT_MSG_LENGTH) {
		Serial.println(F("\n ~ Algorithm Error! strlen(bus_output_msg_buffer) exceeds MAX_SLIMSAT_MSG_LENGTH."));
	}
	
	// Place the message on the circular buffer
	Bus_cbfr.circularBufferPushMsg(bus_output_msg_buffer);

	return;
}


void NmeaMsgHandler::constructBusDbRegisterValueResponseMsg(BusDbRegisterValues& bus_db_reg_data) {
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Constructing SlimSat Bus DB Register Values Response Msg ..."));
	}
		
	initializeTempMessageBuffer();
	//snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%lu,%lu,%lu,%lu,%lu,%lu", bus_db_reg_data.boot_count, bus_db_reg_data.num_valid_msgs, bus_db_reg_data.num_invalid_msgs, bus_db_reg_data.num_bus_data_recs, bus_db_reg_data.next_bus_data_write_to_adr, bus_db_reg_data.num_payload_data_recs, bus_db_reg_data.next_payload_data_write_to_adr);
	snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%lu,%lu,%lu,%lu,%lu,%lu", bus_db_reg_data.boot_count, bus_db_reg_data.num_valid_msgs, bus_db_reg_data.num_invalid_msgs, bus_db_reg_data.num_bus_data_recs, bus_db_reg_data.num_payload_data_recs, bus_db_reg_data.next_bus_data_write_to_adr, bus_db_reg_data.next_payload_data_write_to_adr);
	constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
	
	return;
}


void NmeaMsgHandler::constructBusDataResponseMsg(BusDataRec& bus_data) {
	// This method constructs a formatted Bus Data response message, given a message "payload" 

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Constructing SlimSat Bus Data Response Msg ..."));
	}
	
	initializeTempMessageBuffer();
	snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d,%1.1f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.1f,%1.1f", bus_data.time, bus_data.bus_rec_number, bus_data.temp_C, bus_data.thermistor_V, bus_data.bus_voltage_V_1, bus_data.bus_voltage_V_2, bus_data.bus_voltage_V_3, bus_data.current_mA_1, bus_data.current_mA_2, bus_data.current_mA_3);

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Initial SlimSat Bus Data Response msg is: "));
		Serial.println(temp_msg_buffer);
		Serial.print(F(" ~ SlimSat Response msg strlen is: "));
		Serial.println(strlen(temp_msg_buffer));
	}
	
	// Construct the response message and push it onto the circ buffer
	constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);

	return;
}


void NmeaMsgHandler::constructPayloadDataResponseMsg(PlDataRec& pl_data) {
	// This method constructs a formatted Payload Data response message, given a message "payload" 

	char double_val_buffer[DOUBLE_ELEMENT_MSG_BUFFER_LENGTH];
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Constructing SlimSat Payload Data Response Msg ..."));
	}
	
	initializeTempMessageBuffer();
	snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", pl_data.time, pl_data.pl_rec_number);
	
	for (uint8_t i=0; i<PAYLOAD_DATA_ARY_SIZE; i++) {
		snprintf(double_val_buffer, sizeof(double_val_buffer), ",%1.1f", pl_data.getArrayElement(i));
		strcat(temp_msg_buffer, double_val_buffer);
	}		
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Command ID is: "));
		Serial.println(cmd_id);
		Serial.print(F("\n ~ SlimSat Payload Data array message is: "));
		Serial.println(temp_msg_buffer);
		Serial.print(F(" ~ SlimSat Payload Data array message strlen is: "));
		Serial.println(strlen(temp_msg_buffer));
	}
	
	// Construct the response message and push it onto the circ buffer
	constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);

	return;
}


void NmeaMsgHandler::constructGetFlashRegisterValueResponseMsg(uint32_t uint32_value) {
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Constructing SlimSat Get Flash Register Value Response Msg ..."));
	}
	
	// Note, using temp_msg_buffer here because will be passing the temp buffer to construct response, which will build on it using bus buffer 
	initializeTempMessageBuffer();
	snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_value, rxd_valid_msg_count);
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ SlimSat Get Flash Register Value Response msg is: "));
		Serial.println(temp_msg_buffer);
		Serial.print(F(" ~ SlimSat Response msg strlen is: "));
		Serial.println(strlen(temp_msg_buffer));
	}
	
	// Construct the response message and push it onto the circ buffer
	constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
	
	return;
}


void NmeaMsgHandler::constructGpsPositionResponseMsg(char* gps_pos_msg) {
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Constructing SlimSat Get Flash Register Value Response Msg ..."));
	}
	
	if (0) {
		Serial.print(F("\n ~ Before constructing response message, GPS Position msg is: "));
		Serial.println(gps_pos_msg);
	}
		
	// Note, using temp_msg_buffer here because will be passing the temp buffer to construct response, which will build on it using bus buffer 
	initializeTempMessageBuffer();
	
	snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%s", gps_pos_msg);
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ GPS Position msg is: "));
		Serial.println(gps_pos_msg);
		Serial.print(F(" ~ SlimSat Response msg strlen is: "));
		Serial.println(strlen(temp_msg_buffer));
	}
	
	// Construct the response message and push it onto the circ buffer
	constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
	
	return;
}


void NmeaMsgHandler::constructSetFlashRegisterValueResponseMsg(uint32_t uint32_value)  {
	
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Constructing SlimSat Set Flash Register Value Response Msg ..."));
	}
	
	// Note, using temp_msg_buffer here because will be passing the temp buffer to construct response, which will build on it using bus buffer 
	initializeTempMessageBuffer();
	snprintf(temp_msg_buffer, sizeof(temp_msg_buffer), "%lu,%d", uint32_value, rxd_valid_msg_count);
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ SlimSat Set Flash Register Value Response msg is: "));
		Serial.println(temp_msg_buffer);
		Serial.print(F(" ~ SlimSat Response msg strlen is: "));
		Serial.println(strlen(temp_msg_buffer));
	}
	
	// Construct the response message and push it onto the circ buffer
	constructResponseMsgGivenMsgPayload(cmd_id, 1, temp_msg_buffer);
	
	return;
}
	

char* NmeaMsgHandler::popMsgFromCircBuffer(void) {
	// This method pops the next message off of the circular buffer

	if (VERBOSE_MSG_HANDLER_OUTPUT)
		Serial.println(F("\n ~ Popping the top Msg from Circular Buffer ..."));

	return Bus_cbfr.circularBufferPopMsg();
}


void NmeaMsgHandler::pushMsgOntoCircBuffer(char* slimsat_msg) {
	// This method pushes a message onto the circular buffer

	if (VERBOSE_MSG_HANDLER_OUTPUT)
		Serial.println(F("\n ~ Pushing Msg onto Circular Buffer ..."));

	Bus_cbfr.circularBufferPushMsg(slimsat_msg);

	return;
}


uint8_t NmeaMsgHandler::parseMsg(char* slimsat_msg) {
	// This method parses a received message

	char* slimsat_token_adr = nullptr;
	char* slimsat_end_token_adr = nullptr;
	char* strfunc_result_ptr = nullptr;
	uint8_t field_index = 0;
	uint8_t msg_cmd_id = 0; // Note: a cmd_id of 0 is invalid
	uint8_t msg_cmd_id_is_valid = 0;

	// Reinitialize cmd_value so to avoid stale data
	cmd_value = 0;
	opt_cmd_arg_1 = 0;
	// opt_cmd_arg_2 = 0;

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ SlimSat Message ..."));
		Serial.print(F(" ~ SlimSat Msg: "));
		Serial.println(slimsat_msg);
	}

	slimsat_token_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_DATA_TOKEN);

	// Advance the pointer by one to skip over the ','
	slimsat_token_adr++;

	// Note, the first field of msg type is fundamentally skipped, as it does not need to be parsed.
	while (slimsat_token_adr != nullptr) {
		field_index++;

		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.print(F(" ~ Field Index: "));
			Serial.print(field_index);
			Serial.print(F(" slimsat_token_adr: "));
			Serial.println(slimsat_token_adr);

			if (slimsat_token_adr != nullptr) {
				if (VERBOSE_MSG_HANDLER_OUTPUT) {
					Serial.print(F("\n  str is: "));
					Serial.println(slimsat_token_adr);
				}
			}
		}
		switch (field_index) {
		case SLIMSAT_MSG_CMD_ID_FIELD_INDEX:
			msg_cmd_id = atoi(slimsat_token_adr);
			msg_cmd_id_is_valid = cmdIdIsValid(msg_cmd_id);

			// If cmd_id is valid, then assign it cmd_id
			if (msg_cmd_id_is_valid) {
				cmd_id = msg_cmd_id;
			}
			break;
		case SLIMSAT_MSG_CMD_VALUE_FIELD_INDEX:
			if (msg_cmd_id_is_valid) {
				// Note, there is no validation performed here on the cmd value
				// Convert the cmd value to a numeric value
				//cmd_value = (uint32_t)strtol(slimsat_token_adr, &strfunc_result_ptr, DECIMAL_BASE);
				cmd_value = (int32_t)strtol(slimsat_token_adr, &strfunc_result_ptr, DECIMAL_BASE);

				if (strfunc_result_ptr == nullptr) {
					if (VERBOSE_MSG_HANDLER_OUTPUT) {
						Serial.println(F("\n ~ Warning: strtol returned a null pointer in attempting to convert the parsed NMEA Msg Cmd value!"));
					}
				}
					
				if (VERBOSE_MSG_HANDLER_OUTPUT) {
					Serial.print(F("\n ~ msg value is: "));
					Serial.println(cmd_value);
				}
			}
			break;
		case SLIMSAT_MSG_OPTIONAL_CMD_ARG_1_FIELD_INDEX:
			if (msg_cmd_id_is_valid) {
				// Note, there is no validation performed here on the cmd value
				// Convert the cmd value to a numeric value
				opt_cmd_arg_1 = (uint32_t)strtol(slimsat_token_adr, &strfunc_result_ptr, DECIMAL_BASE);

				if (strfunc_result_ptr == nullptr) {
					if (VERBOSE_MSG_HANDLER_OUTPUT) {
						Serial.println(F("\n ~ Warning: strtol returned a null pointer in attempting to convert the parsed NMEA Msg Cmd optional argument 1!"));
					}
				}
					
				if (VERBOSE_MSG_HANDLER_OUTPUT) {
					Serial.print(F("\n ~ msg optional argument 1 is: "));
					Serial.println(opt_cmd_arg_1);
				}
			}
			break;
		// case SLIMSAT_MSG_OPTIONAL_CMD_ARG_2_FIELD_INDEX:
			// if (msg_cmd_id_is_valid) {
				// // Note, there is no validation performed here on the cmd value
				// // Convert the cmd value to a numeric value
				// opt_cmd_arg_2 = (uint32_t)strtol(slimsat_token_adr, &strfunc_result_ptr, DECIMAL_BASE);

				// if (strfunc_result_ptr == nullptr) {
					// if (VERBOSE_MSG_HANDLER_OUTPUT) {
						// Serial.println(F("\n ~ Warning: strtol returned a null pointer in attempting to convert the parsed NMEA Msg Cmd optional argument 2!"));
					// }
				// }
					
				// if (VERBOSE_MSG_HANDLER_OUTPUT) {
					// Serial.print(F("\n ~ msg optional argument 2 is: "));
					// Serial.println(opt_cmd_arg_2);
				// }
			// }
			// break;
		default:
			if (VERBOSE_MSG_HANDLER_OUTPUT) {
				Serial.print(F("\n ~ Error!  Unhandled SlimSat Msg Field Number value "));
				Serial.print(field_index);
				Serial.println(F(" encountered"));
			}
		}

		slimsat_token_adr = findCharInStr(slimsat_token_adr, SLIMSAT_MSG_DATA_TOKEN);

		if (slimsat_token_adr != nullptr) {
			slimsat_token_adr++;
		}
	}

	return msg_cmd_id_is_valid;
}


uint8_t NmeaMsgHandler::cmdIdIsValid(uint8_t cmd_ID) {
	// This method determines whether or not the given command ID is valid
			
	// Validate the cmd_ID
	if ((MIN_SLIMSAT_CMD_ID <= cmd_ID) && (cmd_ID <= MAX_SLIMSAT_CMD_ID)) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.print(F("\n ~ The Cmd ID is: "));
			Serial.print(cmd_ID);
			Serial.println(F("\n ~ The Cmd ID is valid!"));
		}
		
		return 1;
	}
	else {
		if (VERBOSE_MSG_HANDLER_OUTPUT)	{
			Serial.print(F("\n ~ The Cmd ID is: "));
			Serial.print(cmd_ID);
			Serial.println(F("\n ~ The Cmd ID is NOT valid!"));
		}
		
		return 0;
	}
}


void NmeaMsgHandler::printCircBuffer(void) const {
	// This method prints the Bus circular buffer
	Bus_cbfr.print();
	return;
}


uint8_t NmeaMsgHandler::circBufferIsEmpty(void) const {
	// This method returns whether or not the Bus circular buffer is empty
	return Bus_cbfr.circularBufferIsEmpty();
}


void NmeaMsgHandler::constructBeaconMsg(char* beacon_msg) {
	// This method will be used to construct the Beacon message once its format has been defined - TBD
	return;
}


char* NmeaMsgHandler::findCharInStr(char* start_adr, char chr_to_find) {
	// This method finds a given char within a char array
	
	char* char_ptr = nullptr;

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Finding char in Msg: "));
		Serial.println(chr_to_find);
	}

	char_ptr = strchr(start_adr, chr_to_find);
	
	if (strchr(start_adr, chr_to_find) == nullptr) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ Warning: In Find char in str Function, char_ptr is NULL! ..."));
			Serial.print(F(" ~ The following character was NOT found in the msg: "));
			Serial.println(chr_to_find);
		}
	}

	return char_ptr;
}


char* NmeaMsgHandler::getNextMsg(void) {
	// This method gets the next message in the circ buffer to transmit
	
	char* xmit_msg = Bus_cbfr.circularBufferPopMsg();
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F(" ~ Getting Next Circular Buffer Msg ..."));
	}
	
	//Serial.println(xmit_msg);
	
	return xmit_msg;
}


void NmeaMsgHandler::formatMsgFragmentIntoMsgPayload(uint8_t cmd_id, uint8_t is_ack, char* fragment, uint8_t fragment_number) {
	// This method completes the message fragment by formatting it
	char formatted_msg_fragment[SLIMSAT_MSG_BUFFER_LENGTH];
	char ack_char = 'A';
	memset(formatted_msg_fragment, 0, sizeof(formatted_msg_fragment));
	
	if (0) {
		Serial.print(F(" ~ Fragment "));
		Serial.print(fragment_number);
		Serial.print(F(" Length: "));
		Serial.print(strlen(fragment));
		Serial.print(F(" Content: "));
		Serial.println(fragment);
	}
	
	if (fragment_number == 1) {
		if (!is_ack) {
			ack_char = 'N';
		}
		snprintf(formatted_msg_fragment, sizeof(formatted_msg_fragment), "%c,%d,%d,%s", ack_char, cmd_id, fragment_number, fragment);
	}
	else {
		snprintf(formatted_msg_fragment, sizeof(formatted_msg_fragment), "%d,%d,%s", cmd_id, fragment_number, fragment);
	}
	
	if (0) {
		Serial.print(F(" ~ Formatted msg fragment is: "));
		Serial.println(formatted_msg_fragment);
		Serial.println(F(""));
	}
	
	// Now finish off the message
	constructResponseMsg(formatted_msg_fragment);
	
	return;
}


uint8_t NmeaMsgHandler::constructMsgFragments(uint8_t cmd_id, uint8_t is_ack, char* msg_payload) {
	// This method fragments the long message buffer into smaller messages
	char fragment_msg_buffer[MSG_PAYLOAD_BUFFER_LENGTH];
	uint8_t number_of_fragments = 1;
	uint8_t start_index = 0;
	uint8_t last_found_index = 0;
	uint8_t next_fragment_index_limit = MAX_MSG_PAYLOAD_LENGTH;
	size_t msg_payload_length = strlen(msg_payload);

	if (0) {
		Serial.println(F(" ~ Comma positions: "));
	}
	
	for (uint8_t i=0; i<msg_payload_length; i++) {
		
		if (0) {
			Serial.print(msg_payload[i]);
			Serial.print(F(" is "));
			Serial.println(i);
		}
		
		if (msg_payload[i] == SLIMSAT_MSG_DATA_TOKEN) {
			if (0) {
				Serial.print(i);
			}
			
			if (i < next_fragment_index_limit) {
				// Then the max fragment size has not been reached. So buffer where the comma was found for when it is reached
				last_found_index = i;
			}
			else {
				// The fragment buffer length has been exceeded, so it is time to fragment using the last found comma
				// But, a new comma has been found, so update the last found index at the end
				if (0) {
					Serial.print(F(" -- Fragment at last found index position: ")); 
					Serial.println(last_found_index);
				}
				
				strncpy(fragment_msg_buffer, &msg_payload[start_index], last_found_index - start_index);
				fragment_msg_buffer[last_found_index - start_index] = '\0'; // Null-terminate the fragment
				
				// Now format the message
				formatMsgFragmentIntoMsgPayload(cmd_id, is_ack, fragment_msg_buffer, number_of_fragments);
				start_index = last_found_index + 1; // To skip the comma
				
				if (0) {
					Serial.print(F(" ~ start_index is now set to: "));
					Serial.println(start_index);
				}
				
				next_fragment_index_limit = last_found_index + MAX_MSG_PAYLOAD_LENGTH;
				
				if (0) {
					Serial.print(F(" ~ next_fragment_index_limit is now set to: "));
					Serial.println(next_fragment_index_limit);
 				}
				
				number_of_fragments++;
				// Update the last found comma index for the next iteration since it was here
				last_found_index = i;
			}
		}
	}

	// Handle the last fragment
	strncpy(fragment_msg_buffer, &msg_payload[start_index], msg_payload_length - start_index);
	fragment_msg_buffer[msg_payload_length - start_index] = '\0'; // Null-terminate the fragment

	formatMsgFragmentIntoMsgPayload(cmd_id, is_ack, fragment_msg_buffer, number_of_fragments);

	return number_of_fragments;
}


uint8_t NmeaMsgHandler::constructResponseMsgGivenMsgPayload(uint8_t cmd_id, uint8_t is_ack, char* msg_payload) {
	// This method breaks a long message into multiple smaller fragments, if the output buffer length is exceeded
	// The input to this method is a char array will be completed with message header and footer
	
	uint8_t number_of_fragments = constructMsgFragments(cmd_id, is_ack, msg_payload);
	
	return number_of_fragments;
}