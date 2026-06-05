/**
 * @file nmea_lite_msg_handler.h
 * @brief NMEA Lite Message Handler Header
 * 
 * @details This header file defines the NmeaMsgHandler class which provides
 * NMEA-style message parsing, validation, and command processing for the
 * SlimSat system. It handles command parsing, checksum validation, response
 * generation, and message buffering using a circular buffer system.
 * 
 * This file has been scrubbed, but not the library file
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef NMEA_LITE_MSG_HANDLER_HEADER
#define NMEA_LITE_MSG_HANDLER_HEADER

#include <Arduino.h>
#include <slimsat_cmd_constants.h>
#include <slimsat_id_constants.h>
#include <msg_circular_buffer.h>
#include <bus_data.h>
#include <payload.h>
#include <lora_radio.h>
#include <simple_timer.h>
#include <bus_db.h>
#include <bus_db_register_struct.h>
#include <payload_data.h>
#include <gps_if.h>

#define NUM_HEX_CHARS_IN_CHECKSUM 2

#define SLIMSAT_MSG_CMD_ID_FIELD_INDEX 1
#define SLIMSAT_MSG_CMD_VALUE_FIELD_INDEX 2
#define SLIMSAT_MSG_OPTIONAL_CMD_ARG_1_FIELD_INDEX 3
//#define SLIMSAT_MSG_OPTIONAL_CMD_ARG_2_FIELD_INDEX 4


#define SLIMSAT_MSG_START_CHAR '$'
#define SLIMSAT_MSG_STOP_CHAR '*'
#define SLIMSAT_MSG_DATA_TOKEN ','

#define MIN_SLIMSAT_CMD_ID 1
#define MAX_SLIMSAT_CMD_ID 59

#define SLIMSAT_MSG_MAX_HEADER_LENGTH (5+1)

#define MAX_MSG_HANDLER_MSG_LENGTH 160
#define MSG_HANDLER_BUFFER_LENGTH (MAX_MSG_HANDLER_MSG_LENGTH+1)

#define MAX_SLIMSAT_MSG_LENGTH 32
#define SLIMSAT_MSG_BUFFER_LENGTH (MAX_SLIMSAT_MSG_LENGTH+1)

#define DOUBLE_ELEMENT_MSG_LENGTH 12
#define DOUBLE_ELEMENT_MSG_BUFFER_LENGTH (DOUBLE_ELEMENT_MSG_LENGTH+1)

#define MAX_MSG_PAYLOAD_LENGTH 15
#define MSG_PAYLOAD_BUFFER_LENGTH (33)

#define STR_CMP_MATCHES_VALUE 0
#define STRNCMP_MATCHES_RESULT 0

#define INITIAL_INVALID_VALUE 254
#define HEXIDECIMAL_BASE 16
#define DECIMAL_BASE 10

#define VERBOSE_MSG_HANDLER_OUTPUT 0
#define VERBOSE_MSG_HANDLER_OUTPUT_TO_USER 1

/**
 * @brief NMEA Lite Message Handler Class
 * 
 * @details The NmeaMsgHandler class provides comprehensive NMEA-style message
 * processing capabilities including message parsing, validation, command
 * execution, and response generation. It supports checksum validation,
 * circular buffer management, and integration with various SlimSat subsystems
 * including payload, LoRa radio, GPS, and bus database operations.
 */
class NmeaMsgHandler {
private:
	// Data Members
	char slimsat_msg_header[SLIMSAT_MSG_MAX_HEADER_LENGTH];
	char full_msg_header[SLIMSAT_MSG_MAX_HEADER_LENGTH];
	char bus_output_msg_buffer[MSG_HANDLER_BUFFER_LENGTH];
	char temp_msg_buffer[MSG_HANDLER_BUFFER_LENGTH];
	uint8_t msg_is_valid;
	uint16_t rxd_valid_msg_count;
	uint16_t rxd_invalid_msg_count;
	uint8_t cmd_id;
	int32_t cmd_value;
	int32_t opt_cmd_arg_1;
	//uint32_t opt_cmd_arg_2;
	const char* slimsat_id;

	// Methods
	void initializeDataMembers(void);
	void reinitializeNmeaMsgHandlerMetadata(void);
	void copyMsgHeader(char* msg_adr, char* strfunc_result_ptr, uint8_t msg_index_offset);
	uint8_t getCharIndexOffsetFromAdr(const char* slimsat_msg_start_adr, const char* slimsat_msg_stop_adr);
	uint8_t checksumIsValid(char* slimsat_msg, uint8_t message_checksum);
	uint8_t computeXorChecksumValue(const char* slimsat_msg, uint8_t start_index, uint8_t end_index);
	void respondToNmeaMsg(char* slimsat_msg);
	void pushMsgOntoCircBuffer(char* slimsat_msg);
	uint8_t cmdIdIsValid(uint8_t cmd_id);
	uint8_t parseMsg(char* slimsat_msg);
	void incrementValidMsgCount(BusDb& Bus_Db);
	void incrementInvalidMsgCount(BusDb& Bus_Db);
	void initializeNmeaMsgHeader(void);
	char* findCharInStr(char* start_adr, char chr_to_find);
	void constructResponseMsg(char* msg);
	void initializeTempMessageBuffer(void);
	void initializeMessageBuffers(void);
	
public:
	// Data Member Objects
	MsgCircularBuffer Bus_cbfr;
	BusDataRec bus_data;
	BusDbRegisterValues bus_db_reg_data;
	PlDataRec pl_data_rec;
	
	// Constructors
	NmeaMsgHandler(void);
	
	// Methods
	void print(void) const;
	uint8_t nmeaMsgIsValid(char* slimsat_msg);
	uint8_t computeChecksum(char* slimsat_msg);
	void initializeMessageBuffer(void);
	void handleNmeaMsg(char* slimsat_msg, BusDb& Bus_Db, Payload& Payload, LoRaRadio& LoRa, GpsIf& Gps, SimpleTimer& Bus_Timer, SimpleTimer& Pl_Timer, SimpleTimer& Bcn_Timer);
	void constructAckMsg(int8_t include_cmd_val=0);
	void constructNackMsg(int16_t error_value=0);
	void constructBeaconMsg(char* beacon_msg);
	char* popMsgFromCircBuffer(void);
	void printCircBuffer(void) const;
	uint8_t circBufferIsEmpty(void) const;
	char* getNextMsg(void);
	void constructBusDataResponseMsg(BusDataRec& bus_data);
	void constructBusDbRegisterValueResponseMsg(BusDbRegisterValues& bus_db_reg_data);
	void constructGpsPositionResponseMsg(char* gps_pos_msg);
	void constructGetFlashRegisterValueResponseMsg(uint32_t uint32_value);
	void constructSetFlashRegisterValueResponseMsg(uint32_t uint32_value);
	void handlePayloadCmd(Payload& Payload, uint8_t command_id, int32_t command_value);
	
	void formatMsgFragmentIntoMsgPayload(uint8_t cmd_id, uint8_t is_ack, char* fragment, uint8_t fragment_number);
	uint8_t constructMsgFragments(uint8_t cmd_id, uint8_t is_ack, char* msg);
	uint8_t constructResponseMsgGivenMsgPayload(uint8_t cmd_id, uint8_t is_ack, char* msg);
	void constructPayloadDataResponseMsg(PlDataRec& pl_data);
};

#endif