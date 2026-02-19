// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef GPS_IF_HEADER
#define GPS_IF_HEADER

#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include <TinyGPSPlus.h>

#define GPS_BAUD 9600
#define MAX_GPS_MSG_LENGTH 60
#define MSG_GPS_BUFFER_LENGTH (MAX_GPS_MSG_LENGTH+1)
#define MAX_NUM_GPS_MSG_READ_TRIES 10

#define VERBOSE_GPS_OUTPUT 0


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
	char* getGpsPositionMessagePayload(void);
	void initializeDataMembers(void);
	char* getEmptyGpsPositionMessage(void);

public:
	TinyGPSPlus gps;
	char gps_msg_buffer[MSG_GPS_BUFFER_LENGTH];
	GpsIf(void);
	void begin(void);
	void initializeGpsMessageBuffer(void);
	char* getGpsPositionMessage(void);
	char* encodeGpsMessage(void);
	void print(void) const;  
};

#endif


