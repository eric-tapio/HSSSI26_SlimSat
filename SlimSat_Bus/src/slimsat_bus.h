// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SLIMSAT_BUS_HEADER
#define SLIMSAT_BUS_HEADER

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
//#include <Adafruit_TinyUSB.h> // for Serial
#include <payload.h>
#include <msg_circular_buffer.h>
#include <nmea_lite_msg_handler.h>
#include <simple_timer.h>
#include <lora_radio.h>
#include <sc_bus_db.h>

#define VERBOSE_SC_BUS_OUTPUT 0

// //Serial Terminal Input variables
#define MAX_CHARS 31+1


class SlimSatBus {
private:
	// Data Members
	uint8_t stream_data_mode;

	// Serial Terminal Input data members
	char serialTermReceivedChars[MAX_CHARS];
	uint8_t dataIndex = 0;
	uint8_t newCmdRxd = 0;
	uint8_t newCmdToProcess = 0;
	
	// Methods
	void initializeserialTermReceivedChars(void);

public:
	// Data Member Objects
	ScBusDb Sc_database;
	Payload Payload1;
	NmeaMsgHandler Msg_handler;
	SimpleTimer Payload_timer;
	SimpleTimer Beacon_timer;
	LoRaRadio LoRa;

	// Constructors
	SlimSatBus(void);

	// Methods
	void performScBusOperationIteration(void);
	void initializeScBus(void);
	void transmitBeaconMessage(void);
	void getScBusCmd(void);
	void handleScBusCmd(void);
	void print(void) const;
	void performPayloadOp(void);

	// Serial Terminal Input methods
	void getSerialTermInput(void);
	void displayNewSerialTermData(void);
};

#endif