// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <Arduino.h>
#include <msg_circular_buffer.h>
#include <assert.h>

uint8_t testMsgCircularBuffer(void) {

	Serial.println(F(" ~ Running SlimSat Msg Circular Buffer Tests ..."));

	char msg_data[BUFFER_MSG_LENGTH];
	msg_data[0] = '\0';

	char* msg_ptr = msg_data;

	const char* msg_1 = "This is just a test";
	const char* msg_2 = "of the SlimSat System";
	const char* msg_3 = "This could be a very long test";
	const char* msg_4 = "Or a short 1";
	const char* msg_5 = "It's really up to you";
	const char* msg_6 = "I'm-trying!a#variety$of%symbols";
	const char* msg_7 = "Anything'but'double'quotes'please";
	const char* msg_8 = "What_if_I_use_Underscores";
	const char* msg_9 = "12345567890";
	const char* msg_10 = "Though^ther&is*more(that)can+be=used";
	const char* msg_11 = "$GS01,A,999,6*AA";

	uint8_t num_msgs = 0;
	uint8_t buffer_capacity = 0;
	uint8_t is_empty = 0;
	uint8_t is_full = 0;
	uint16_t head_ptr = 0;
	uint16_t tail_ptr = 0;

	static MsgCircularBuffer cbfr;

	Serial.println(F(" ~ Printing Initialized Circular Buffer ..."));
	cbfr.print();

	// Test Initialization
	Serial.println(F(" ~ Testing Initialized Circular Buffer ..."));

	num_msgs = cbfr.getCircularBufferNumMsgs();
	buffer_capacity = cbfr.getCircularBufferCapacity();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has the correct capacity
	assert(buffer_capacity == (uint8_t)BUFFER_CAPACITY_NUM_MSGS);

	// Verify head and tail pointers are correct
	assert(head_ptr == 0);
	assert(tail_ptr == 0);

	// Verify circular buffer has no messages in it
	assert(num_msgs == 0);
	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Initialized Circular Buffer Tests: PASS"));

	Serial.println(F(" ~ Pushing and Popping first message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_1);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	buffer_capacity = cbfr.getCircularBufferCapacity();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 1 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 1 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping second message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_2);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();
	int strcmp_result = 0;

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 2 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 2 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);

	
	Serial.println(F(" ~ Pushing and Popping third message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_3);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 3 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 3 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping fourth message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_4);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 4 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 4 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping fifth message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_5);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 5 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 5 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping sixth message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_6);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 6 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 6 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping seventh message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_7);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 7 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 7 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping eighth message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_8);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 8 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 8 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping ninth message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_9);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 9 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 9 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping tenth message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_10);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 0 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 0 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);


	Serial.println(F(" ~ Pushing and Popping eleventh message onto Circular Buffer ..."));
	cbfr.circularBufferPushMsg(msg_11);
	cbfr.circularBufferPopMsg();
	cbfr.print();

	num_msgs = cbfr.getCircularBufferNumMsgs();
	is_empty = cbfr.circularBufferIsEmpty();
	is_full = cbfr.circularBufferIsFull();
	head_ptr = cbfr.getHeadPointer();
	tail_ptr = cbfr.getTailPointer();

	// Verify circular buffer has one message in it
	assert(num_msgs == 0);

	// Verify head and tail pointers are correct
	assert(head_ptr == 1 * (uint8_t)BUFFER_MSG_LENGTH);
	assert(tail_ptr == 1 * (uint8_t)BUFFER_MSG_LENGTH);

	// Verify circular buffer is empty
	assert(is_empty == 1);

	// Verify circular buffer is not full
	assert(is_full == 0);

	Serial.println(F("\n ~ Congrats, all SlimSat Message Circular Buffer Tests: PASS!\n"));

	return 0;
}


void setup() {
  // Open a serial port for SlimSat communication
  Serial.begin(115200);
	while (!Serial) {
		// Wait for Serial
		delay(10);
	}

	Serial.println(F("\n ~ Running Message Circular Buffer Tests ... "));

	testMsgCircularBuffer();
}


void loop() {
	Serial.print(".");
	delay(1000);
}
