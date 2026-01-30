/**
 * @file payload.h
 * @brief SlimSat Payload System Header
 * 
 * @details This header file defines the Payload class which manages the
 * payload subsystem for the SlimSat spacecraft. It handles sensor operations,
 * measurement collection, data processing, and command execution for the
 * payload instruments including ultrasonic range sensors.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef SLIMSAT_PAYLOAD_HEADER
#define SLIMSAT_PAYLOAD_HEADER

#include <Arduino.h>
#include <payload_cmd_constants.h>
#include <ping_sensor.h>
#include <payload_data.h>

#define PAYLOAD_MSG_BUFFER_LENGTH 22+1

// In this example, the first 20 elements of the payload data array, 0 thru 19, are allocated for raw PING))) Range Measurements
// The next 3 elements of the payload data array, 21 thru 23, are allocated for processed range measurements (min, max, & mean)
// The last element is allocated for a second (simulated) sensor which outputs a single measurement value

//#define MIN_MEASUREMENT_ARY_INDEX 20
//#define MAX_MEASUREMENT_ARY_INDEX 21
//#define MEAN_MEASUREMENT_ARY_INDEX 22
//#define SENSOR_2_MEASUREMENT_ARY_INDEX 23

#define MIN_MEASUREMENT_ARY_INDEX 6
#define MAX_MEASUREMENT_ARY_INDEX 7
#define MEAN_MEASUREMENT_ARY_INDEX 8
#define SENSOR_2_MEASUREMENT_ARY_INDEX 9

#define MIN_NUMBER_MEASUREMENTS 1
#define MAX_NUMBER_MEASUREMENTS 20

#define VERBOSE_PAYLOAD_OUTPUT 0


/**
 * @brief SlimSat Payload System Class
 * 
 * @details The Payload class manages the complete payload subsystem including
 * sensor operations, measurement collection, data processing, and command handling.
 * It can be expanded and customized by the Payload Team as needed for specific
 * mission requirements. The class integrates ultrasonic sensors and provides
 * comprehensive measurement and data management capabilities.
 */
class Payload {
private:
	// The private portion of the class can be changed to whatever is needed to tailor 
    // the interface to support the sensor(s) used by the SlimSat Team
	
	// Data Members
	char pl_output_msg_buffer[PAYLOAD_MSG_BUFFER_LENGTH]; // Buffer for payload data messages
	
	uint8_t num_measurements_to_take; // Number of measurements to collect
	uint8_t input_is_valid; // Input validation flag


	// Methods (functions)
	/**
	 * @brief Initialize payload data members
	 * @details Sets all data members to default/initial values
	 */
	void initializeDataMembers(void);

	/**
	 * @brief Set measurement timestamp
	 * @details Records the current system time for measurements
	 */
	void setMeasurementTime(void);

	/**
	 * @brief Get next payload data message
	 * @details Formats the next data message in a series
	 * @param start_at_index Starting index for data elements
	 * @param message_num Current message number
	 * @param total_num_messages Total number of messages
	 * @return Index where formatting stopped
	 */
	uint8_t getNextPayloadDataMsg(uint8_t start_at_index, uint8_t message_num, uint8_t total_num_messages);

	/**
	 * @brief Format payload data messages
	 * @details Formats measurement data into multiple messages
	 * @param total_num_messages Total number of messages to create
	 * @return Number of messages created
	 */
	uint8_t formatPayloadDataMsgs(uint8_t total_num_messages);

	// All of the following are example private methods
	/**
	 * @brief Initialize range measurement array
	 * @details Clears all values in the measurement array
	 */
	void initializePayloadDataAry(void);

	/**
	 * @brief Set number of measurements to take
	 * @details Configures how many measurements to collect
	 * @param num_meas Number of measurements to take
	 */
	void setNumMeasurementsToTake(uint8_t num_meas);

	/**
	 * @brief Perform round of measurements
	 * @details Executes a complete measurement sequence
	 */
	void performRoundofSensorMeasurements(PlDataRec& pl_data_rec);

	/**
	 * @brief Find maximum range measurement
	 * @details Determines the largest value in measurement array
	 */
	double getMaxRangeMeasurement(PlDataRec& pl_data_rec);

	/**
	 * @brief Find minimum range measurement
	 * @details Determines the smallest value in measurement array
	 */
	double getMinRangeMeasurement(PlDataRec& pl_data_rec);

	/**
	 * @brief Find mean range measurement
	 * @details Calculates the average of all measurements
	 */
	double getMeanRangeMeasurement(PlDataRec& pl_data_rec);


public:
	// **************************************************
	// The following public interface shall not be modified.
	// However, new methods may be added, and requires approval by the U of L SW team
	
	// Constructors
	/**
	 * @brief Default constructor for Payload
	 * @details Initializes payload with default configuration
	 */
	Payload(void);

	// Object Data Members
	Ping_Sensor Ping; // Ultrasonic ping sensor object

	
	// Methods (functions)
	/**
	 * @brief Print payload information
	 * @details Outputs payload state and configuration for debugging
	 */
	void print(void) const;

	/**
	 * @brief Initialize payload system
	 * @details Sets up payload for operation including hardware configuration
	 */
	void initializePayload(void);

	/**
	 * @brief Initialize payload message buffer
	 * @details Clears the payload output message buffer
	 */
	void initializePayloadMessageBuffer(void);

	/**
	 * @brief Handle payload command
	 * @details Processes received payload commands and generates responses
	 * @param cmd_id Command identifier
	 * @param cmd_val Command value/parameter
	 * @return Number of response messages generated
	 */
	//char* handlePayloadCommand(uint8_t cmd_id, uint32_t cmd_val, PlDataRec& pl_data_rec);
	char* handlePayloadCommand(uint8_t cmd_id, int32_t cmd_val, PlDataRec& pl_data_rec);

	/**
	 * @brief Get payload data string
	 * @details Returns pointer to payload output message buffer
	 * @return Pointer to payload data string
	 */
	char* getPayloadDataStr(void);

	void performPayloadOperationIteration(PlDataRec& pl_data_rec);

	double getSensor2Measurement(void) const;
};

#endif