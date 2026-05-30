/**
 * @file gps_if.cpp
 * @brief GPS Interface Implementation
 * 
 * @details This file implements the GpsIf class methods for managing
 * GPS communication and data processing. Provides GPS initialization,
 * message parsing, position tracking, and time synchronization
 * capabilities for the SlimSat navigation system.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <gps_if.h>

/**
 * @brief Default constructor for the GPS Interface class
 * @details Initializes the GPS interface by calling the data member
 * initialization method to set up default values and clear buffers.
 */
GpsIf::GpsIf(void) {
	initializeDataMembers();
}

/**
 * @brief Initialize GPS interface data members
 * @details Sets all GPS data members to default values including time validity,
 * position data, and message buffers. Prepares the GPS interface for operation.
 */
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
	buffer_index = 0;

	return;
}


/**
 * @brief Initialize GPS message buffer
 * @details Clears the GPS message buffer by setting all bytes to zero.
 * Ensures clean buffer state for GPS message processing and output.
 */
void GpsIf::initializeGpsMessageBuffer(void) {
	// This method initializes the GPS message buffer used to output Payload data to the Bus

	memset(gps_msg_buffer, 0, sizeof(gps_msg_buffer));

	return;
}


/**
 * @brief Begin GPS operation
 * @details Initializes the GPS device by starting serial communication
 * at the specified baud rate. Provides optional verbose output showing
 * library version information when debugging is enabled.
 */
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


char* GpsIf::getGpsMessage(void) {
    // This function (attempts to) get the next GPGGA GPS message from the GPS sensor
    // If a GPGGA message is received, the message is encoded and a 3D position sentence is returned
    // Otherwise a default empty sentence is returned
    uint8_t number_tries = 0;
    uint8_t gpgga_msg_found = 0;
    char* gps_msg_ptr = nullptr;
    char* msg_ptr = nullptr;

    do {
        gps_msg_ptr = readGpsMessage();
 
        if (gps_msg_ptr != nullptr) {
            // Then a gga message was found
            //Serial.println(my_ptr);
            gpgga_msg_found = 1;
        }
        else {
            // Increment the try counter
            number_tries++;
        }

    } while ((gpgga_msg_found == 0) || (number_tries < MAX_NUM_GPS_MSG_READ_TRIES));

	if (VERBOSE_GPS_OUTPUT) {
		Serial.println(" ~ Out of Do While Loop ...");
	}	

    if (gpgga_msg_found) {
        // Then encode the message and return the formatted partial message
        msg_ptr = encodeGpsMessage(gps_msg_ptr);
    }
    else {
        // Return the default empty string
		msg_ptr = getEmptyGpsPositionMessage();
    }

    return msg_ptr;
}


char* GpsIf::readGpsMessage(void) {
    // This function reads a GPS message from the GPS sensor
    // This funciton returns a nullptr until a complete message is received, at which point the function 
    // returns a pointer to the received message
    char* msg_ptr = nullptr;
    
    while (Serial1.available()) {
        // Read one character at a time
        char rc = (char)Serial1.read();

        // Detect start of a new NMEA message
        if (rc == '$') {
            buffer_index = 0;
        }

        // Store character if space remains
        if (buffer_index < GPS_BUFFER_SIZE) {
            gps_msg_buffer[buffer_index++] = rc;

            // Null terminate the string
            gps_msg_buffer[buffer_index] = '\0';
        }
        else {
            // Prevent buffer overflow
            buffer_index = 0;
        }

        // NMEA messages end with newline
        if (rc == '\n') {
            if (strncmp(gps_msg_buffer, GPGGA_MSG_HEADER, strlen(GPGGA_MSG_HEADER)) == 0) { 
                msg_ptr = gps_msg_buffer;
                break;
            }

            // Reset for next message
            buffer_index = 0;
        }
    }

    return msg_ptr;
}


char* GpsIf::encodeGpsMessage(char* msg_ptr) {
	// This method encodes the received GPS Messages
	
    for (uint8_t i=0; i<strlen(msg_ptr); i++) {
        gps.encode(gps_msg_buffer[i]);
    }
				
	time_is_valid = gps.time.isValid();
	update_time_hour = gps.time.hour();
	update_time_min = gps.time.minute();
	update_time_sec = gps.time.second();
	position_is_valid = gps.location.isValid();
	lat_pos = gps.location.lat();
	long_pos = gps.location.lng();
	alt_pos = gps.altitude.meters();
	
	if (VERBOSE_GPS_OUTPUT) {	
		print();
	}
				
	return getGpsPositionMessagePayload();
}


char* GpsIf::getGpsPositionMessage(void) {
	// This method gets input from the GPS serial source

	char* return_ptr = nullptr;
	
	return_ptr = getGpsMessage();

	return return_ptr;
}


char* GpsIf::getGpsPositionMessagePayload(void) {
	// This method populates and returns the GPS Position Message 'Payload'

	// Time from TinyGPS can be 0 and still valid. As a result, guard against it 
	uint8_t update_time_sum = update_time_hour + update_time_min + update_time_sec;
	
	if (0) {
		Serial.println(gps_msg_buffer);
		Serial.println();
	}
	
	if (time_is_valid && (update_time_sum > 0)) {

		if (VERBOSE_GPS_OUTPUT) {
			Serial.println(F(" ~ Time is Valid."));
		}
		
		// Update the last update time
		last_update_time_sec = update_time_sec;
					
		// Construct the output message
		if (position_is_valid) {
			snprintf(gps_msg_buffer, sizeof(gps_msg_buffer), "%02d:%02d:%02d,%1.6f,%1.6f,%1.1f", update_time_hour, update_time_min, update_time_sec, lat_pos, long_pos, alt_pos);
		}
		else {
			snprintf(gps_msg_buffer, sizeof(gps_msg_buffer), "%02d:%02d:%02d,,,", update_time_hour, update_time_min, update_time_sec);
		}

		return gps_msg_buffer;
	}		
	else {
		if (VERBOSE_GPS_OUTPUT) {
			Serial.println(F(" ~ Returning Null Ptr as the GPS message."));
		}
		
		return getEmptyGpsPositionMessage();
	}
}


char* GpsIf::getEmptyGpsPositionMessage(void)  {
	// This method returns an Empty GPS Position Message, which is returned if the current GPS position fix is not valid
	snprintf(gps_msg_buffer, sizeof(gps_msg_buffer), DEFAULT_EMPTY_GPS_MSG);
	return gps_msg_buffer;
}


void GpsIf::print(void) const {
	// This method prints the GPS Interface Data Members
	Serial.println(F("\n ~ Printing GPS Sensor Interface Data Members ..."));

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
	
	Serial.println();
	
    return;
}