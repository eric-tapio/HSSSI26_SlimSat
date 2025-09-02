// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#ifndef SLIMSAT_PAYLOAD_HEADER
#define SLIMSAT_PAYLOAD_HEADER

#include <Arduino.h>
#include <payload_cmd_constants.h>
#include <ping_sensor.h>
#include <msg_circular_buffer.h>

#define PAYLOAD_MSG_BUFFER_LENGTH 22+1
#define TEMP_BUFFER_LENGTH 22+1
#define TEMP_DATA_ELEMENT_BUFFER_LENGTH 10+1

#define RANGE_MEASUREMENT_ARY_SIZE 20
#define NUM_MILLISEC_PER_SEC 1000
#define MIN_NUMBER_MEASUREMENTS 1
#define MAX_NUMBER_MEASUREMENTS 20
#define BUFFER_CAPACITY_NUM_MSGS 10
#define BUFFER_MSG_LENGTH (31+1)
#define VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT 0
#define STR_CMP_MATCHES_VALUE 0

#define VERBOSE_PAYLOAD_OUTPUT 0

enum Payload_State
{
	STOPPED,    // Default value is 0
	RUNNING,    // Default value is 1
};

// The Payload class, which can be expanded and added to by the Payload Team, as needed
class Payload {
private:
	// The private portion of the class can be changed to whatever is needed to tailor 
    // the interface to support the sensor(s) used by the SlimSat Team
	
	// Data Members
	Payload_State pl_state;
	
	uint16_t ranges_in_cm_ary[RANGE_MEASUREMENT_ARY_SIZE]; // Create an array to hold range measurements
	char pl_output_msg_buffer[(uint8_t)PAYLOAD_MSG_BUFFER_LENGTH]; // Create a char array to hold the payload data message.
	uint16_t max_measurement_value;
	uint16_t min_measurement_value;
	uint16_t mean_measurement_value;
	uint8_t num_measurements_to_take;
	uint8_t input_is_valid;
	uint8_t sensor_ID;
	uint32_t measurement_time_in_sec;

	// Methods (funcitons)
	void initializeDataMembers(void);
	void setMeasurementTime(void);
	uint8_t getNextPayloadDataMsg(uint8_t start_at_index, uint8_t message_num, uint8_t total_num_messages);
	uint8_t formatPayloadDataMsgs(uint8_t total_num_messages);

	// All of the following are example private methods
	void initializeRangeMeasAry(void);
	void setNumMeasurementsToTake(uint8_t num_meas);
	void performRoundofMeasurements(void);
	void processRangeMeasurements(void);
	void findMaxRangeMeasurement(void); 
	void findMinRangeMeasurement(void);
	void findMeanRangeMeasurement(void);


public:
	// **************************************************
	// The following public interface shall not be modified.
	// However, new methods may be added, and requires approval by the U of L SW team
	
	// Constructors
	Payload(void);

	// Object Data Members
	Ping_Sensor Ping;
	MsgCircularBuffer Payload_cbfr;

	// Methods (funcitons)
	void print(void) const;
	void initializePayload(void);
	void initializePayloadMessageBuffer(void);
	uint8_t handlePayloadCommand(uint8_t cmd_id, uint32_t cmd_val);
	uint8_t getPayloadState(void) const;
	char* getPayloadDataStr(void);
	uint8_t getPayloadDataMsgs(void); // New for circ buffer extraction
	uint8_t getNumPayloadDataMsgs(void);
	char* getNextPayloadDataMsg(void);
	uint8_t performPayloadOperationIteration(void);
	// ************************************************
	
	uint32_t getScBusUpTime(void);  // For now until the S/C is included
	// ********************

	// New public methods to support development and debug only
	void printRangeAry(void) const;
};

#endif