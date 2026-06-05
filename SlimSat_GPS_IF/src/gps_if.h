// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef GPS_IF_HEADER
#define GPS_IF_HEADER

#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include <TinyGPSPlus.h>
#include <bus_pin_definitions.h>

#define GPS_BAUD 9600
#define GPS_BUFFER_SIZE 100
#define MSG_GPS_BUFFER_SIZE (GPS_BUFFER_SIZE+1)
#define MAX_NUM_GPS_MSG_READ_TRIES 10

#define VERBOSE_GPS_OUTPUT 0
#define GPGGA_MSG_HEADER "$GPGGA"

const char DEFAULT_EMPTY_GPS_MSG[] = ",,,,";

class GpsIf {
private:
	bool time_is_valid;
	uint8_t update_time_hour;
	uint8_t update_time_min;
	uint8_t update_time_sec;
	bool position_is_valid;
	double lat_pos;
	double long_pos;
	double alt_pos;
	uint8_t last_update_time_sec;
	uint8_t buffer_index;
	char gps_msg_buffer[MSG_GPS_BUFFER_SIZE];
	
	char* getGpsPositionMessagePayload(void);
	void initializeDataMembers(void);
	void initializeGpsMessageBuffer(void);
	char* getEmptyGpsPositionMessage(void);
	char* readGpsMessage(void); 
	char* getGpsMessage(void);
	char* encodeGpsMessage(char* msg_ptr);

public:
	TinyGPSPlus gps;
	GpsIf(void);
	void begin(void);
	char* getGpsPositionMessage(void);
	void print(void) const;
	uint8_t testGpsDeviceConnectivity(void) const;
};

#endif


