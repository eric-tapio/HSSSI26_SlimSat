//
//  Payload Data Record
//
// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <payload_data.h>


PlDataRec::PlDataRec(void) {
	// Default Constructor
	time = 0;
	pl_rec_number = 0;
	pl_data_array;
	pl_data_ptr = &(pl_data_array[0]);
	
	initializeArray();
}


void PlDataRec::initializeArray(void) {
	// This method initializes the temp message buffer used to output data to the SlimSat
	memset(pl_data_array, 0.0, sizeof(pl_data_array));
	return;
}


void PlDataRec::printRecord(void) {

	Serial.println(F("\n ~ Printing the Payload Data Record ..."));
	
	Serial.print(F("    Number Data Payload Data Array Elements is: "));
	Serial.println(PAYLOAD_DATA_ARY_SIZE);
	Serial.print(F("    Payload Data Array Size in Bytes is: "));
	Serial.println(getRecordSize());	
	Serial.print(F("    Time is: "));
	Serial.println(time);
	
	Serial.print(F("    Rec Number is: "));
	Serial.println(pl_rec_number);

	printArray();
	
	return;
}


void PlDataRec::printArray(void) {

	Serial.println(F("\n ~ Printing the Payload Data Array ..."));

	for (uint8_t i=0 ; i<PAYLOAD_DATA_ARY_SIZE; i++) {
		Serial.print(F(" data["));
		Serial.print(i);
		Serial.print(F("] = "));
		Serial.println(pl_data_array[i]);
	}
	
	return;
}


uint8_t PlDataRec::getRecordSize(void) {
	// This method determines the size of a Payload Data Record
	return (PAYLOAD_DATA_ARY_SIZE) * sizeof(double) + sizeof(uint32_t) + sizeof(uint16_t);
}


void PlDataRec::setArrayElement(uint8_t index, double value) {

	if ((0 <= index) && (index < PAYLOAD_DATA_ARY_SIZE)) {
		pl_data_array[index] = value;
	}
	
	return;
}


double PlDataRec::getArrayElement(uint8_t index) {

	if ((0 <= index) && (index < PAYLOAD_DATA_ARY_SIZE)) {
		return pl_data_array[index];
	}
	else {
		return -1.0;
	}
}


uint32_t PlDataRec::getTime(void) {
	return time;
}


void PlDataRec::setTime(uint32_t time_val) {
	time = time_val;
	return;
}


uint16_t PlDataRec::getRecordNumber(void) {
	return pl_rec_number;
}


void PlDataRec::setRecordNumber(uint16_t rec_number) {
	pl_rec_number = rec_number;
	return;
}