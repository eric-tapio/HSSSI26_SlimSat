// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#ifndef MSG_CIRCULAR_BUFFER_HEADER
#define MSG_CIRCULAR_BUFFER_HEADER

#include <Arduino.h>

#define BUFFER_CAPACITY_NUM_MSGS 10
#define BUFFER_MSG_LENGTH (31+1)

#define VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT 0

// This is FIFO Message Circular Buffer

class MsgCircularBuffer {
private:
	// Data Members
	char circ_buffer[BUFFER_CAPACITY_NUM_MSGS * BUFFER_MSG_LENGTH];
	char* buffer_ptr;
	uint16_t head; // Head and Tail need to be uint16_t since the length of the buffer can easily go beyond 255. See if this is a constraint on Arduino
	uint16_t tail;
	uint8_t num_msg_capacity; //of the buffer
	uint8_t msg_length; //in the buffer
	uint8_t num_msgs_in_buffer;
	uint8_t is_full;
	
	// Methods
	void advanceHeadPointer(void);
	void advanceTailPointer(void);
	void resetCircularBuffer(void); // Moving to public for testing
	
public:
	// Constructors
	MsgCircularBuffer(void);
	
	// Methods
	void print(void) const;
	void printCircularBufferMsgs(void) const;
	uint8_t circularBufferIsEmpty(void) const;
	uint8_t circularBufferIsFull(void) const;
	void circularBufferPushMsg(const char* msg_data_ptr);
	void circularBufferPushMsg(char* msg_data);
	uint8_t getCircularBufferCapacity(void) const;
	uint8_t getCircularBufferNumMsgs(void) const;
	char* circularBufferPopMsg(void);
	char* getCircularBufferNextMsg(void);
	void initEntireBufferAry(void);
	void printEntireBufferAry(void) const;

	// To be removed after unit tested
	uint16_t getHeadPointer(void) const;
	uint16_t getTailPointer(void) const;
};


#endif