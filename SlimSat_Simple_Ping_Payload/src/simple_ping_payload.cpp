/**
 * @file payload.cpp
 * @brief SlimSat Payload System 
 * 
 * @details 
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <simple_ping_payload.h>

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
	// This method initializes the payload, which can include initializing data members
	// and hardware pin states. This method is intended to be called only once during setup
	
	// Nothing to Initialize in this example
	
	return;
}

void Payload::performPayloadLoopIteration(PlDataRec& pl_data) {
	// This method performs the operations desired each time a payload operation is performed
	
	double measurement = 0.0;
	  
	for (uint8_t i=0; i<PAYLOAD_DATA_ARY_SIZE; i++) {
		measurement = getRangeMeasurementInCm();
		pl_data.setArrayElement(i, measurement);
	}

	return;
}

double Payload::getRangeMeasurementInCm(void) {
	// This method performs a Ping Sensor range measurement and returns the measured range in cm
	
	long duration = 0;
	double range_in_cm = 0.0;

	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(PING_PIN, OUTPUT);
	digitalWrite(PING_PIN, LOW);
	delay(3);
	digitalWrite(PING_PIN, HIGH);
	delay(5);
	digitalWrite(PING_PIN, LOW);

	// The same pin is used to read the signal from the PING))): a HIGH pulse
	// whose duration is the time (in microseconds) from the sending of the ping
	// to the reception of its echo off of an object.
	
	pinMode(PING_PIN, INPUT);
	duration = pulseIn(PING_PIN, HIGH);

	// Convert the time into a distance
	range_in_cm = microsecondsToCentimeters(duration);
	
	return range_in_cm;
}

long Payload::microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
	return microseconds / 29 / 2;
}