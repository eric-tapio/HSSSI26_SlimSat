/**
 * @file simple_payload.h
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

#ifndef SLIMSAT_SIMPLE_PAYLOAD_HEADER
#define SLIMSAT_SIMPLE_PAYLOAD_HEADER

#include <Arduino.h>
#include <ping_sensor.h>
#include <payload_data.h>

#define SENSOR_2_MEASUREMENT_ARY_INDEX 23
#define VERBOSE_PAYLOAD_OUTPUT 1


/**
 * @brief SlimSat Payload System Class
 * 
 * @details The Payload class manages the complete payload subsystem including
 * sensor operations, measurement collection, data processing, and optionally any command handling.
 * It can be expanded and customized by the Payload Team as needed for specific
 * mission requirements.
 */
 
class Payload {
private:
	// The private portion of the class can be changed to whatever is needed to tailor 
    // the interface to support the sensor(s) used by the SlimSat Team
	

public:
	Ping_Sensor Ping;
	double getSensor2Measurement(void) const;

	// **************************************************
	// The following public interface section shall not be modified.
	// Constructors
	Payload(void);

	void initializePayload(void);
	void performPayloadOperationIteration(PlDataRec& pl_data_rec);

	//
	// **************************************************
};

#endif