// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Adafruit_TinyUSB.h>
#include <payload_data.h>
#include <assert.h>

#define REC_TIME_1 12
#define REC_TIME_2 22
#define REC_NUM_1 20
#define REC_NUM_2 100

// Construct the Payload Data Object
PlDataRec pl_data_rec;
double value = 0.0;
uint16_t rec_num = 0;
uint32_t rec_time = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for serial to initialize

  Serial.println(" ~ Testing Payload Data Record ...\n");

  // Get the current data struct
  pl_data_rec.printRecord();
  Serial.println();

	Serial.println(F(" ~ Testing Default Initialized Array Element ..."));
  for (uint8_t i=0; i<PAYLOAD_DATA_ARY_SIZE; i++) {
    value = pl_data_rec.getArrayElement(i);
    assert(value == 0);
  }

	Serial.println(F(" ~ Testing Set Array Element ..."));

  for (uint8_t i=0; i<PAYLOAD_DATA_ARY_SIZE; i++) {
    pl_data_rec.setArrayElement(i, i*1.111);
  }

  pl_data_rec.printArray();
  Serial.println();

	Serial.println(F(" ~ Testing Get Array Element ..."));

  for (uint8_t i=0; i<PAYLOAD_DATA_ARY_SIZE; i++) {
    value = pl_data_rec.getArrayElement(i);
    assert(value == i*1.111);
  }

	Serial.println(F(" ~ Testing Set Record Time ..."));
  // Set and get the record time
  pl_data_rec.setTime(REC_TIME_1);
  rec_time = pl_data_rec.getTime();
  assert(rec_time == REC_TIME_1);

	Serial.println(F(" ~ Testing Set Record Number ..."));
  pl_data_rec.setRecordNumber(REC_NUM_1);
	rec_num = pl_data_rec.getRecordNumber();
  assert(rec_num == REC_NUM_1);


	Serial.println(F(" ~ Testing Set Record Time ..."));
  pl_data_rec.time = REC_TIME_2;
  assert(pl_data_rec.time == REC_TIME_2);

	Serial.println(F(" ~ Testing Set Record Number ..."));
  pl_data_rec.pl_rec_number = REC_NUM_2;
  assert(pl_data_rec.pl_rec_number == REC_NUM_2);

  pl_data_rec.printRecord();

	Serial.println(F("\n ~ All Payload Data tests: PASS\n"));
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
