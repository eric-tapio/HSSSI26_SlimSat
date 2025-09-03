// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#include <nmea_lite_msg_handler.h>

NmeaMsgHandler::NmeaMsgHandler(void) {
	// The default constructor, and only constructor, for the NMEA Message Handler class

	initializeMessageBuffes();

	msg_is_valid = 0;
	rxd_valid_msg_count = 0;
	rxd_invalid_msg_count = 0;
	cmd_id = 0;
	cmd_value = 0;

	return;
}

void NmeaMsgHandler::print(void) const {
	// This method prints the NMEA Message Handler class members
	
	Serial.println(F("\n ~ Printing NMEA Msg Data ... "));

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

	Serial.print(F("\n ~ Printing NMEA Msg Data Circular Buffer ... "));
	Sc_cbfr.print();

	return;
}

		
void NmeaMsgHandler::initializeMessageBuffes(void) {
	// This method initializess all buffers used

	initializeMessageBuffer();
	initializeTempMessageBuffer();
	initializeNmeaMsgHeader();

	return;
}



void NmeaMsgHandler::initializeMessageBuffer(void) {
	// This method initializes the S/C message buffer used to output S/C data to the SlimSat

	// Set all array elements of the buffer to 'null'
	for (uint8_t i = 0; i < SC_MSG_BUFFER_LENGTH; i++) {
		sc_output_msg_buffer[i] = '\0';
	}

	return;
}


void NmeaMsgHandler::initializeTempMessageBuffer(void) {
	// This method initializes the temp message buffer used to output S/C data to the SlimSat

	// Set all array elements of the buffer to 'null'
	for (uint8_t i = 0; i < SC_MSG_BUFFER_LENGTH; i++) {
		temp_msg_buffer[i] = '\0';
	}

	return;
}




void NmeaMsgHandler::incrementValidMsgCount(void) {
	// This method increments the valid received message counter
	
	rxd_valid_msg_count++;

	return;
}


void NmeaMsgHandler::incrementInvalidMsgCount(void) {
	// This method increments the invalid received message counter
	
	rxd_invalid_msg_count++;

	return;
}


uint8_t NmeaMsgHandler::nmeaMsgIsValid(char* slimsat_msg) {
	// This method determines whether or not the msg received is a valid message

	char* slimsat_msg_start_adr = NULL;
	char* slimsat_msg_stop_adr = NULL;
	char* comma_token_adr = NULL;
	char* strfunc_result_ptr = NULL;
	char* msg_adr = slimsat_msg;
	char slimsat_msg_checksum[NUM_HEX_CHARS_IN_CHECKSUM + 1] = { '\0' };
	uint8_t valid_start_char = 0;
	uint8_t valid_stop_char = 0;
	uint8_t msg_checksum = 0;
	uint8_t slimsat_msg_stop_index = 0;
	uint8_t checksum_is_valid = 0;
	uint8_t msg_index_offset = 0;
	uint8_t slimsat_id_matches = 0;
	
	const char SLIMSAT_FULL_HEADER_ID[] = SLIMSAT_MSG_TALKER SLIMSAT_ID;

	reinitializeNmeaMsgHandlerMetadata();

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Determining if NMEA msg is valid ..."));
		Serial.print(F("    Input NMEA msg: "));
		Serial.println(slimsat_msg);
	}

	// Parse the message header by finding the end of the NMEA Msg preamble
	comma_token_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_DATA_TOKEN);

	if (comma_token_adr == NULL) {
			Serial.println(F("\n ~ Warning! The NMEA Msg token character ',' was NOT found!"));
	}

	msg_index_offset = getCharIndexOffsetFromAdr(slimsat_msg, comma_token_adr);

	copyMsgHeader(msg_adr, strfunc_result_ptr, msg_index_offset);
					
	// Then check if the cmd is for this SlimSat ID
	if (strncmp(full_msg_header, SLIMSAT_FULL_HEADER_ID, strlen(SLIMSAT_FULL_HEADER_ID)) == STRNCMP_MATCHES_RESULT) {
		slimsat_id_matches = 1;
	}
	else {
		slimsat_id_matches = 0;
	}
	
	if (slimsat_id_matches) {
		// Find the NMEA Msg Start Char, '$'
		slimsat_msg_start_adr = slimsat_msg;
		slimsat_msg_stop_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_START_CHAR);

		if (slimsat_msg_stop_adr == NULL) {
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

			if (slimsat_msg_stop_adr == NULL) {
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

					if (strfunc_result_ptr == NULL) {
						if (VERBOSE_MSG_HANDLER_OUTPUT) {
							Serial.println(F("\n ~ Warning: strncpy returned a null pointer in attempting to copy the NMEA Msg checksum!"));
						}
					}
					else {
						// Convert the checksum to a numeric value
						msg_checksum = (uint8_t)strtol(slimsat_msg_checksum, &strfunc_result_ptr, HEXIDECIMAL_BASE);
					}

					if (strfunc_result_ptr == NULL) {
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
		Serial.println(F("\n ~ Checking Checksum ... "));
	}

	computed_checksum = computeChecksum(slimsat_msg);

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Message Checksum is (hex): "));
		Serial.println(message_checksum, HEX);
		Serial.print(F(" ~ Computed Checksum is (hex): "));
		Serial.println(computed_checksum, HEX);
	}

	if (computed_checksum == message_checksum)
		return 1;
	else
		return 0;
}


uint8_t NmeaMsgHandler::computeChecksum(char* slimsat_msg) {
	// This method computes the checksum of the given message
	
	char* SLIMSAT_start_token_adr = NULL;
	char* slimsat_end_token_adr = NULL;
	uint8_t xor_val = 0;
	uint8_t start_index = 0;
	uint8_t end_index = 0;

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Computing Checksum ... "));
	}

	SLIMSAT_start_token_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_START_CHAR);
	slimsat_end_token_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_STOP_CHAR);
	if ((SLIMSAT_start_token_adr != NULL) && (slimsat_end_token_adr != NULL)) {
		start_index = SLIMSAT_start_token_adr + 1 - slimsat_msg; // + 1 to skip over the '$'
		end_index = slimsat_end_token_adr - slimsat_msg; // The < end_index will ensure that the '*' is not included

		xor_val = computeXorChecksumValue(slimsat_msg, start_index, end_index);
	}
	else if (SLIMSAT_start_token_adr == NULL) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ Warning: No NMEA Msg start $ char found in NMEA Msg:"));
			Serial.print(slimsat_msg);
		}
	}
	else if (slimsat_end_token_adr == NULL) {
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

	for (uint8_t i = 0; i < SLIMSAT_MSG_MAX_HEADER_SIZE; i++)
		full_msg_header[i] = '\0';

	return;
}


void NmeaMsgHandler::reinitializeNmeaMsgHandlerMetadata(void) {
	// This method initializes the NMEA message metadata
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Reinitializing NMEA Msg Metadata ..."));
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
		Serial.print(F("\n ~ Getting Char Index Offset from Adr ..."));
	}

	if ((slimsat_msg_start_adr == NULL) || (slimsat_msg_stop_adr == NULL)) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.print(F("\n ~ Warning: Either slimsat_msg_start_adr or slimsat_msg_stop_adr is NULL! ..."));
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

	if (strfunc_result_ptr == NULL) {
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


void NmeaMsgHandler::handleNmeaMsg(char* slimsat_msg, ScBusDb& Sc_Db, Payload& Payload, LoRaRadio& LoRa, SimpleTimer& Pl_Timer, SimpleTimer& Bcn_Timer) {
	// This method handles received cmd messages
	
	uint32_t uint32_val = 0;
	
	// Reinitialize the Cmd ID to 0
	cmd_id = 0;

	if (nmeaMsgIsValid(slimsat_msg)) {
		parseMsg(slimsat_msg);
		incrementValidMsgCount();

		if ((MIN_SLIMSAT_CMD_ID <= cmd_id) && (cmd_id <= MAX_SLIMSAT_CMD_ID)) {
			switch (cmd_id) {
			case PING_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Ping Command ..."));
				constructAckMsg();
				Serial.println(F("\n ~ S/C Ping Cmd received"));
				break;
			case REBOOT_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Reboot Command ..."));
				constructAckMsg();
				Serial.println(F("\n ~ Note: Remote reboot is not yet supported on this development unit"));	
				break;
			case GET_REGISTER_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Register Command ..."));
				uint32_val = LoRa.getRegister(cmd_value);
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ Register Value is: "));
				Serial.println(uint32_val);
				break;
			case SET_REGISTER_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set Register Command ..."));
				constructAckMsg();
				LoRa.setRegister(1000, cmd_value); // Still need an additional argument for register address - TBD
				Serial.print(F("\n ~ Register has been set to Value: "));
				Serial.println(cmd_value);
				break;
			case GET_RADIO_FREQUENCY_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Radio Frequency Command ..."));
				uint32_val = LoRa.getFrequency();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ Lora Frequency is: "));
				Serial.print(uint32_val);
				Serial.println(F(" Hz"));
				break;
			case SET_RADIO_FREQUENCY_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set Radio Frequency Command ..."));
				constructAckMsg();
				LoRa.setFrequency(cmd_value);
				Serial.print(F("\n ~ Lora Frequency has been set to: "));
				Serial.print(cmd_value);
				Serial.println(F(" Hz"));
				break;
			case GET_RADIO_BANDWIDTH_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Radio Bandwidth Command ..."));
				uint32_val = LoRa.getBandwidth();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ Lora Bandwidth is: "));
				Serial.print(uint32_val);
				Serial.println(F(" Hz"));
				break;
			case SET_RADIO_BANDWIDTH_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set Radio Bandwidth Command ..."));
				constructAckMsg();
				LoRa.setBandwidth(cmd_value);
				Serial.print(F("\n ~ Lora Bandwidth has been set to: "));
				Serial.println(cmd_value);
				break;
			case GET_RADIO_SPREAD_FACTOR_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Radio Spread Factor Command ..."));
				uint32_val = LoRa.getSpreadFactor();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ Lora Spread Factor is: "));
				Serial.println(uint32_val);
				break;
			case SET_RADIO_SPREAD_FACTOR_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set Radio Spread Factor Command ..."));
				constructAckMsg();
				LoRa.setSpreadFactor(cmd_value);
				Serial.print(F("\n ~ Lora Spread Factor has been set to: "));
				Serial.println(cmd_value);
				break;
			case GET_RADIO_RSSI_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Radio RSSI Command ..."));
				uint32_val = LoRa.getRssi();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ Lora RSSI is: "));
				Serial.println(uint32_val);
				break;
			case GET_BEACON_PERIOD_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Beacon Period Command ..."));
				uint32_val = Bcn_Timer.getIntervalInMs();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ Beacon Period is: "));
				Serial.print(uint32_val);
				Serial.println(F(" ms"));
				break;
			case SET_BEACON_PERIOD_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set Beacon Period Command ..."));
				constructAckMsg();
				uint32_val = Bcn_Timer.setIntervalInMs(cmd_value);
				Serial.print(F("\n ~ Beacon Period has been set to: "));
				Serial.print(cmd_value);
				Serial.println(F("  ms"));							
				break;
			case GET_SC_BUS_MODE_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get S/C Bus Mode Command ..."));
				uint32_val = Sc_Db.getScBusMode();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ S/C Bus Mode is: "));
				Serial.println(uint32_val);
				break;
			case SET_SC_BUS_MODE_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set S/C Bus Mode Command ..."));
				constructAckMsg();
				Sc_Db.setScBusMode((uint8_t)cmd_value);
				Serial.print(F("\n ~ S/C Bus Mode set to: "));
				Serial.println(cmd_value);
				break;
			case GET_SC_BUS_EPOCH_TIME_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get S/C Bus Epoch Time Command ..."));
				uint32_val = Sc_Db.Sc_Time.getScBusEpochTimeInMs();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ S/C Bus Time is: "));
				Serial.print(uint32_val);
				Serial.println(F("  ms"));	
				break;
			case SET_SC_BUS_EPOCH_TIME_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set S/C Bus Epoch Time Command ..."));
				constructAckMsg();
				Sc_Db.Sc_Time.setScBusEpochTimeInMs(cmd_value);
				Serial.print(F("\n ~ S/C Bus Epoch Time set to: "));
				Serial.print(uint32_val);
				Serial.println(F("  ms"));	
				break;
			case GET_SC_BUS_TIME_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get S/C Bus Time Command ..."));
				uint32_val = Sc_Db.Sc_Time.getScBusTimeInMs();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ S/C Bus Time is: "));
				Serial.print(uint32_val);
				Serial.println(F("  ms"));
				break;
			case GET_SC_BUS_DATA_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get S/C Bus Data Command ..."));
				constructAckMsg();
				Serial.println(F("\n ~ Set S/C Data is not yet supported"));
				break;
			case GET_DATA_STREAM_MODE_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get S/C Bus Stream Data Mode Command ..."));
				uint32_val = Sc_Db.getScBusStreamDataState();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ S/C Bus Stream Data Mode is: "));
				Serial.println(uint32_val);
				break;
			case SET_DATA_STREAM_MODE_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set S/C Bus Stream Data Mode Command ..."));
				constructAckMsg();
				Sc_Db.setScBusStreamDataState(cmd_value);
				Serial.print(F("\n ~ S/C Bus Stream Data Mode has been set to: "));
				Serial.println(cmd_value);
				break;				
			case GET_PAYLOAD_OP_PERIOD_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Payload Op Period Command ..."));
				uint32_val = Pl_Timer.getIntervalInMs();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ Payload Op Period is: "));
				Serial.print(uint32_val);
				Serial.println(F(" ms"));
				break;
			case SET_PAYLOAD_OP_PERIOD_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Set Payload Op Period Command ..."));
				constructAckMsg();
				Pl_Timer.setIntervalInMs(cmd_value);
				Serial.print(F("\n ~ Payload Op Period has been set to: "));
				Serial.print(cmd_value);
				Serial.println(F(" ms"));
				break;
			case GET_PAYLOAD_MODE_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Payload Op Mode Command ..."));
				uint32_val = Payload.getPayloadState();
				sprintf(temp_msg_buffer, "A,%d,%lu,%d", cmd_id, uint32_val, rxd_valid_msg_count);
				constructResponseMsg(temp_msg_buffer);
				Serial.print(F("\n ~ Payload Mode is: "));
				Serial.println(uint32_val);
				break;
			case GET_PAYLOAD_DATA_COMMAND_ID:
				Serial.print(F("\n ~ Handling Rx'd Get Payload Data Mode Command ..."));
				constructAckMsg();
				Serial.println(F("\n ~ Get Payload Data is not yet supported."));
				break;
			case PAYLOAD_CMD_1:
			case PAYLOAD_CMD_2:
			case PAYLOAD_CMD_3:
			case PAYLOAD_CMD_4:
			case PAYLOAD_CMD_5:
			case PAYLOAD_CMD_6:
			case PAYLOAD_CMD_7:
			case PAYLOAD_CMD_8:
			case PAYLOAD_CMD_9:
			case PAYLOAD_CMD_10:
				handlePayloadCmd(Payload, cmd_id, cmd_value);
				break;
			default:
				Serial.print(F("\n ~ Encountered Unhandled SlimSat Command ID: "));
				Serial.print(cmd_id);
				Serial.println(F(". Ignoring!"));
				break;
			}
		}
		else {
			Serial.print(F("\n ~ Encountered Invalid S/C Bus Command ID: "));
			Serial.print(cmd_id);
			Serial.println(F(". Ignoring!"));
			constructNackMsg();
			incrementInvalidMsgCount();
		}
	}
	else {
		Serial.print(F("\n ~ Encountered Invalid SlimSat Msg ID: "));
		Serial.print(cmd_id);
		Serial.println(F(". Ignoring!"));
		constructNackMsg();
		incrementInvalidMsgCount();
	}

	return;
}


void NmeaMsgHandler::handlePayloadCmd(Payload& Payload, uint8_t command_id, uint32_t command_value) {
	Serial.print(F("\n ~ Handling Rx'd Payload Command ..."));
	char* pl_msg = NULL;
	
	uint8_t num_pl_msgs = Payload.handlePayloadCommand(command_id, command_value);			
	uint8_t num_msgs = Payload.getNumPayloadDataMsgs();
				
	for (uint8_t i=0; i<num_msgs; i++) {
		pl_msg = Payload.getNextPayloadDataMsg();
		
		constructResponseMsg(pl_msg);
		
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.print(F(" ~ Payload Msg["));
			Serial.print(i);
			Serial.print(F("] is: "));
			Serial.println(pl_msg);
		}
	}

	return;
}


void NmeaMsgHandler::constructAckMsg(void) {
	// This method constructs an Acknowledged message to the received command message
	// SlimSat Acknowledgement Msg
	// $GS01,A,cmd_id*hh<CR><LF>
	
	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Constructing SlimSat ACK Msg ..."));
	}
	
	uint8_t msg_int_checksum = 0;
	char checksum_bfr[3] = {'\0'};

	sprintf(sc_output_msg_buffer, "$%sG,A,%d,%lu,%d*", SLIMSAT_ID, cmd_id, cmd_value, rxd_valid_msg_count);
	msg_int_checksum = computeChecksum(sc_output_msg_buffer);
	sprintf(checksum_bfr, "%02X", msg_int_checksum);
	strcat(sc_output_msg_buffer, checksum_bfr);

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Full SlimSat ACK msg is: "));
		Serial.println(sc_output_msg_buffer);
		Serial.print(F(" ~ SlimSat ACK msg strlen is: "));
		Serial.println(strlen(sc_output_msg_buffer));
	}

	// Place the message on the circular buffer
	Sc_cbfr.circularBufferPushMsg(sc_output_msg_buffer);

	return;
}


void NmeaMsgHandler::constructNackMsg(void) {
	// This method constructs an Not Acknowledged message to the received command message
	// SlimSat Acknowledgement Msg
	// $GS01,N,cmd_id*hh<CR><LF>

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Constructing SlimSat NACK Msg ..."));
	}
	
	//This function outputs a SlimSat ACK Acknowledgement message
	uint8_t msg_int_checksum = 0;
	char checksum_bfr[3] = {'\0'};
	
	sprintf(sc_output_msg_buffer, "$%sG,N,%d,%lu*", SLIMSAT_ID, cmd_id, cmd_value);
	msg_int_checksum = computeChecksum(sc_output_msg_buffer);
	sprintf(checksum_bfr, "%02X", msg_int_checksum);
  
	// Then concatenate the data element to the temp bfr
	strcat(sc_output_msg_buffer, checksum_bfr);

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Full SlimSat NACK msg is: "));
		Serial.println(sc_output_msg_buffer);
		Serial.print(F(" ~ SlimSat NACK msg strlen is: "));
		Serial.println(strlen(sc_output_msg_buffer));
	}

	// Place the message on the circular buffer
	Sc_cbfr.circularBufferPushMsg(sc_output_msg_buffer);

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
	sprintf(sc_output_msg_buffer, "$S01G,%s*", msg);
	msg_int_checksum = computeChecksum(sc_output_msg_buffer);
	sprintf(checksum_bfr, "%02X", msg_int_checksum);
	strcat(sc_output_msg_buffer, checksum_bfr);

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Full SlimSat Response msg is: "));
		Serial.println(sc_output_msg_buffer);
		Serial.print(F(" ~ SlimSat Response msg strlen is: "));
		Serial.println(strlen(sc_output_msg_buffer));
	}

	// Place the message on the circular buffer
	Sc_cbfr.circularBufferPushMsg(sc_output_msg_buffer);

	return;
}


char* NmeaMsgHandler::popMsgFromCircBuffer(void) {
	// This method pops the next message off of the circular buffer

	if (VERBOSE_MSG_HANDLER_OUTPUT)
		Serial.println(F("\n ~ Popping the top Msg from Circular Buffer ..."));

	return Sc_cbfr.circularBufferPopMsg();
}


void NmeaMsgHandler::pushMsgOntoCircBuffer(char* slimsat_msg) {
	// This method pushes a message onto the circular buffer

	if (VERBOSE_MSG_HANDLER_OUTPUT)
		Serial.println(F("\n ~ Pushing Msg onto Circular Buffer ..."));

	Sc_cbfr.circularBufferPushMsg(slimsat_msg);

	return;
}


void NmeaMsgHandler::reinitializeNmeaMsg(char* slimsat_msg) {
	// This method reinitialized the NMEA message buffer

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ Reinitializing NMEA Msg ..."));
	}
	
	for (uint8_t msg_index = 0; msg_index < MAX_SLIMSAT_MSG_SIZE; msg_index++)
		*(slimsat_msg + msg_index) = '\0';

	return;
}


uint8_t NmeaMsgHandler::parseMsg(char* slimsat_msg) {
	// This method parses a received message

	char* slimsat_token_adr = NULL;
	char* slimsat_end_token_adr = NULL;
	char* strfunc_result_ptr = NULL;
	uint8_t field_index = 0;
	uint8_t msg_cmd_id = 0; // Note: a cmd_id of 0 is invalid
	uint8_t msg_cmd_id_is_valid = 0;


	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.println(F("\n ~ SlimSat Message ... "));
		Serial.print(F(" ~ SlimSat Msg: "));
		Serial.println(slimsat_msg);
	}

	slimsat_token_adr = findCharInStr(slimsat_msg, SLIMSAT_MSG_DATA_TOKEN);

	// Advance the pointer by one to skip over the ','
	slimsat_token_adr++;

	// Note, the first field of msg type is fundamentally skipped, as it does not need to be parsed.
	while (slimsat_token_adr != NULL) {
		field_index++;

		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.print(F(" ~ Field Index: "));
			Serial.print(field_index);
			Serial.print(F(" slimsat_token_adr: "));
			Serial.println(slimsat_token_adr);

			if (slimsat_token_adr != NULL) {
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
				cmd_value = (uint32_t)strtol(slimsat_token_adr, &strfunc_result_ptr, DECIMAL_BASE);

				if (strfunc_result_ptr == NULL) {
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
		default:
			if (VERBOSE_MSG_HANDLER_OUTPUT) {
				Serial.print(F("\n ~ Error!  Unhandled SlimSat Msg Field Number value "));
				Serial.print(field_index);
				Serial.println(F(" encountered"));
			}
		}

		slimsat_token_adr = findCharInStr(slimsat_token_adr, SLIMSAT_MSG_DATA_TOKEN);

		if (slimsat_token_adr != NULL) {
			slimsat_token_adr++;
		}
	}

	return msg_cmd_id_is_valid;
}


uint8_t NmeaMsgHandler::cmdIdIsValid(uint8_t cmd_ID) {
	// This method determines whether or not the given command ID is valid

	// Validate the cmd_ID
	if ((MIN_SLIMSAT_MSG_LENGTH <= cmd_ID) && (cmd_ID <= MAX_SLIMSAT_MSG_LENGTH)) {
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
	// This method prints the S/C Bus circular buffer
	Sc_cbfr.print();
	return;
}


uint8_t NmeaMsgHandler::circBufferIsEmpty(void) const {
	// This method returns whether or not the S/C Bus circular buffer is empty
	return Sc_cbfr.circularBufferIsEmpty();
}


void NmeaMsgHandler::constructBeaconMsg(char* beacon_msg) {
	// This method will be used to construct the Beacon message once its format has been defined - TBD
	return;
}


char* NmeaMsgHandler::findCharInStr(char* start_adr, char chr_to_find) {
	// This method finds a given char within a char array
	
	char* char_ptr = NULL;

	if (VERBOSE_MSG_HANDLER_OUTPUT) {
		Serial.print(F("\n ~ Finding char in Msg: "));
		Serial.println(chr_to_find);
	}

	char_ptr = strchr(start_adr, chr_to_find);
	
	if (strchr(start_adr, chr_to_find) == NULL) {
		if (VERBOSE_MSG_HANDLER_OUTPUT) {
			Serial.println(F("\n ~ Warning: In Find char in str Function, char_ptr is NULL! ..."));
			Serial.print(F(" ~ The following character was NOT found in the msg: "));
			Serial.println(chr_to_find);
		}
	}

	return char_ptr;
}


// uint8_t NmeaMsgHandler::msgsToTransmitAvailable(void) const {
	// // This method returns whether or not there are messages in the circ buffer to transmit
	
	// uint8_t num_msgs = Sc_cbfr.getCircularBufferNumMsgs();
	
	// if (num_msgs > 0) {
		// return 1;
	// }
	// else {
		// return 0;
	// }
// }


char* NmeaMsgHandler::transmitNextMsg(void) {
	// This method transmits the next message in the circ buffer
	
	char* xmit_msg = Sc_cbfr.circularBufferPopMsg();
	
	if (1) {
		//Serial.print(F("\n ~ Transmitting Msg: "));
		Serial.println(xmit_msg);
	}
	
	return xmit_msg;
}