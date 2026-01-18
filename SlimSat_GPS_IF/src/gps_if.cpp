// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <gps_if.h>


// Constructors
GpsIf::GpsIf(void) {
	initializeDataMembers();
}


void GpsIf::initializeDataMembers(void) {
	// This function initializes object data members
	initializeGpsMessageBuffer();
	
	time_is_valid = false;
	update_time_hour = 0;
	update_time_min = 0;
	update_time_sec = 0;
	position_is_valid = false;
	lat_pos = 0.0;
	long_pos = 0.0;
	alt_pos = 0.0;
		
	last_update_time_sec = 0;

	return;
}


void GpsIf::initializeGpsMessageBuffer(void) {
	// This method initializes the GPS message buffer used to output Payload data to the Bus

	memset(gps_msg_buffer, 0, sizeof(gps_msg_buffer));

	return;
}


void GpsIf::begin(void) {
	// This method starts the GPS device
	if (VERBOSE_GPS_OUTPUT) {
		Serial.println(F(" ~ Starting GPS ...")); 
		Serial.print(F("   Using TinyGPSPlus library v. ")); 
		Serial.println(TinyGPSPlus::libraryVersion());
	}
	
	Serial1.begin(GPS_BAUD);
	
	return;
}


char* GpsIf::encodeGpsMessage(void) {


	while (Serial1.available() > 0) {
		if (gps.encode(Serial1.read())) {
			
			time_is_valid = gps.time.isValid();
			update_time_hour = gps.time.hour();
			update_time_min = gps.time.minute();
			update_time_sec = gps.time.second();
			position_is_valid = gps.location.isValid();
			lat_pos = gps.location.lat();
			long_pos = gps.location.lng();
			alt_pos = gps.altitude.meters();
			
			return getGpsPositionMessagePayload();
		}
	}

	return nullptr;
}


char* GpsIf::getGpsPositionMessage(void) {
	// This method gets input from the GPS serial source
	
	char* return_ptr = nullptr;
	
	do {
		return_ptr = encodeGpsMessage();
	} while (return_ptr == nullptr);
		
	return return_ptr;
}


char* GpsIf::getGpsPositionMessagePayload(void) {
	// This method populates and returns the GPS Position Message 'Payload'
	
	if (time_is_valid) {
			
		// Only update if the time has changed	
		if (update_time_sec != last_update_time_sec) {

			// Update the last update time
			last_update_time_sec = update_time_sec;
			
			// Construct the output message
			if (position_is_valid) {
				snprintf(gps_msg_buffer, sizeof(gps_msg_buffer), "%02d:%02d:%02d,%1.6f,%1.6f,%1.1f", update_time_hour, update_time_min, update_time_sec, lat_pos, long_pos, alt_pos);
			}
			else {
				snprintf(gps_msg_buffer, sizeof(gps_msg_buffer), "%02d:%02d:%02d,,,", update_time_hour, update_time_min, update_time_sec);
			}

			if (0) {
				Serial.println(gps_msg_buffer);
				Serial.println();
			}
			
			return gps_msg_buffer;
		}
	}
	
	return nullptr;
}


void GpsIf::print(void) const {
	Serial.println(F(" ~ Printing GPS Sensor Interface Data Members ..."));

	Serial.print(F("     Update Time is Valid: "));
	Serial.println(time_is_valid);
	Serial.print(F("     Update Time Hour Value: "));
	Serial.println(update_time_hour);
	Serial.print(F("     Update Time Min Value: "));
	Serial.println(update_time_min);
	Serial.print(F("     Update Time Sec Value: "));
	Serial.println(update_time_sec);
	Serial.print(F("     Update Position is Valid: "));
	Serial.println(position_is_valid);
	Serial.print(F("     Update Position Lat Value: "));
	Serial.println(lat_pos);
	Serial.print(F("     Update Position Long Value: "));
	Serial.println(long_pos);
	Serial.print(F("     Update Position Alt Value: "));
	Serial.println(alt_pos);	
	Serial.print(F("     Last Update Time Sec Value: "));
	Serial.println(last_update_time_sec);
	Serial.print(F("     GPS Message Buffer: "));
	Serial.println(gps_msg_buffer);	
	
	Serial.println(F(""));
	
    return;
}