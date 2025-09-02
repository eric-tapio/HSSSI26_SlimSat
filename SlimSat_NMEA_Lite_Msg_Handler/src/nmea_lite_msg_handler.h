// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#ifndef NMEA_LITE_MSG_HANDLER_HEADER
#define NMEA_LITE_MSG_HANDLER_HEADER

#include <Arduino.h>
#include <slimsat_cmd_constants.h>
#include <slimsat_id_constants.h>
#include <msg_circular_buffer.h>
#include <sc_bus_db.h>
#include <payload.h>
#include <lora_radio.h>
#include <simple_timer.h>

#define NUM_HEX_CHARS_IN_CHECKSUM 2

#define SLIMSAT_MSG_CMD_ID_FIELD_INDEX 1
#define SLIMSAT_MSG_CMD_VALUE_FIELD_INDEX 2


#define SLIMSAT_MSG_TALKER "$G"

#define SLIMSAT_MSG_START_CHAR '$'
#define SLIMSAT_MSG_STOP_CHAR '*'
#define SLIMSAT_MSG_DATA_TOKEN ','

#define MIN_SLIMSAT_MSG_LENGTH 1
#define MAX_SLIMSAT_MSG_LENGTH 40
#define SLIMSAT_MSG_MAX_HEADER_SIZE (5+1)
#define SC_MSG_BUFFER_LENGTH (32+1)
#define MAX_SLIMSAT_MSG_SIZE (21+1)
#define STR_CMP_MATCHES_VALUE 0
#define STRNCMP_MATCHES_RESULT 0

#define INITIAL_INVALID_VALUE 254
#define HEXIDECIMAL_BASE 16
#define DECIMAL_BASE 10

#define VERBOSE_MSG_HANDLER_OUTPUT 0


class NmeaMsgHandler {
private:
	// Data Members
	char full_msg_header[SLIMSAT_MSG_MAX_HEADER_SIZE];
	char sc_output_msg_buffer[(uint8_t)SC_MSG_BUFFER_LENGTH];
	char temp_msg_buffer[(uint8_t)SC_MSG_BUFFER_LENGTH];
	uint8_t msg_is_valid;
	uint8_t rxd_valid_msg_count;
	uint8_t rxd_invalid_msg_count;
	uint8_t cmd_id;
	uint32_t cmd_value;

	// Methods
	void reinitializeNmeaMsgHandlerMetadata(void);
	void copyMsgHeader(char* msg_adr, char* strfunc_result_ptr, uint8_t msg_index_offset);
	uint8_t getCharIndexOffsetFromAdr(const char* slimsat_msg_start_adr, const char* slimsat_msg_stop_adr);
	uint8_t checksumIsValid(char* slimsat_msg, uint8_t message_checksum);
	uint8_t computeXorChecksumValue(const char* slimsat_msg, uint8_t start_index, uint8_t end_index);
	void respondToNmeaMsg(char* slimsat_msg);
	void pushMsgOntoCircBuffer(char* slimsat_msg);
	void reinitializeNmeaMsg(char* slimsat_msg);
	uint8_t cmdIdIsValid(uint8_t cmd_id);
	uint8_t parseMsg(char* slimsat_msg);
	void incrementValidMsgCount(void);
	void incrementInvalidMsgCount(void);
	void initializeNmeaMsgHeader(void);
	char* findCharInStr(char* start_adr, char chr_to_find);
	void constructResponseMsg(char* msg);
	void initializeTempMessageBuffer(void);
	void initializeMessageBuffes(void);

public:
	// Data Member Objects
	MsgCircularBuffer Sc_cbfr;
	
	// Constructors
	NmeaMsgHandler(void);
	
	// Methods
	void print(void) const;
	uint8_t nmeaMsgIsValid(char* slimsat_msg);
	uint8_t computeChecksum(char* slimsat_msg);
	void initializeMessageBuffer(void);
	void handleNmeaMsg(char* slimsat_msg, ScBusDb& Sc_Db, Payload& Payload, LoRaRadio& LoRa, SimpleTimer& Pl_Timer, SimpleTimer& Bcn_Timer);
	void constructAckMsg(void);
	void constructNackMsg(void);
	void constructBeaconMsg(char* beacon_msg);
	char* popMsgFromCircBuffer(void);
	void printCircBuffer(void) const;
	uint8_t circBufferIsEmpty(void) const;
	
	// New
	void handlePayloadCmd(Payload& Payload, uint8_t command_id, uint32_t command_value);
};


#endif
