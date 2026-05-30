/**
 * @file payload.cpp
 * @brief SlimSat Payload System Implementation
 * 
 * @details This file implements the Payload class methods for managing
 * the payload subsystem including sensor operations, measurement collection,
 * data processing, and command handling for SlimSat payload instruments.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <simple_payload.h>


/**
 * @brief Default constructor for the payload class
 * @details Initializes the payload system by setting up data members
 * and performing system initialization. This is the only constructor
 * available for the payload class.
 */
Payload::Payload(void) {
	// The default constructor, and only constructor, for the payload class
	// No data members in this example
}


/**
 * @brief Initialize payload system
 * @details Performs complete payload initialization including hardware
 * configuration, pin setup, and system state initialization. Sets
 * payload to STOPPED state for safe startup.
 */
void Payload::initializePayload(void) {
	// This method initializes the payload, which can include initializing data members and hardware pin states
	// This method is intended to be called only once during setup
	
	// Nothing to Initialize in this example
	
	return;
}


void Payload::performPayloadLoopIteration(PlDataRec& pl_data) {
	// This method performs the operations desired each time a payload operation is performed
	
	double measurement = 0.0;
	
	for (uint8_t i=0; i<PAYLOAD_DATA_ARY_SIZE; i++) {
		measurement = (double)(Ping.getRangeMeasurementInCm());
		pl_data.setArrayElement(i, measurement);
	}

	pl_data.setArrayElement(SENSOR_2_MEASUREMENT_ARY_INDEX, getSensor2Measurement());

	return;
}


double Payload::getSensor2Measurement(void) const {
	// This method emulates a second sensor value
	return 23.45;
}