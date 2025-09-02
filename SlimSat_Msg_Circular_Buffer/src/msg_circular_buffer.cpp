// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#include <msg_circular_buffer.h>

MsgCircularBuffer::MsgCircularBuffer(void) {
	// The default constructor, and only constructor, for the message circular buffer class

	buffer_ptr = circ_buffer;
	head = 0; // Head points to the address/index to be written to next
	tail = 0; // Tail points to the address/index be read from next
	num_msg_capacity = (uint8_t)BUFFER_CAPACITY_NUM_MSGS;
	msg_length = (uint8_t)BUFFER_MSG_LENGTH;
	num_msgs_in_buffer = 0;
	is_full = 0;

	initEntireBufferAry();

	return;
}


void MsgCircularBuffer::print(void) const {
	// This method prints the message circular buffer data members
	
	Serial.println(F("\n ~  Printing Circular Buffer ..."));
	Serial.print(F("      Circ Buffer Ptr: "));
	Serial.println(buffer_ptr);
	Serial.print(F("      Head Ptr: "));
	Serial.println(head);
	Serial.print(F("      Tail Ptr: "));
	Serial.println(tail);
	Serial.print(F("      Row Size: "));
	Serial.println(num_msg_capacity);
	Serial.print(F("      Col Size: "));
	Serial.println(msg_length);
	Serial.print(F("      Num of Mgs in Buffer: "));
	Serial.println(num_msgs_in_buffer);
	Serial.print(F("      Is is_full: "));
	Serial.println(is_full);
	Serial.print(F("      Is Empty: "));
	Serial.println(circularBufferIsEmpty());

	printCircularBufferMsgs();

	Serial.println("");

	return;
}


void MsgCircularBuffer::printCircularBufferMsgs(void) const {
	//  This method prints the messages in the circular buffer
	
	uint8_t msg_index = 0;
	uint16_t start_index = 0;
	char* msg_data_ptr = NULL;

	Serial.println(F("\n ~  Printing Circular Buffer Msgs ..."));

	for (msg_index = 0; msg_index < num_msg_capacity; msg_index++) {
		start_index = msg_index * msg_length;
		msg_data_ptr = &(buffer_ptr[start_index]);

		if (1) {
			Serial.print(F("\n      start_index["));
			Serial.print(msg_index);
			Serial.print(F("] = "));
			Serial.println(start_index);
			Serial.print(F("      Buffer Data Msg["));
			Serial.print(msg_index);
			Serial.print(F("] = "));
			Serial.println(msg_data_ptr);
		}
	}

	Serial.println(F("\n ~ Done Printing Circular Buffer Msgs ..."));

	return;
}


void MsgCircularBuffer::advanceHeadPointer(void) {
	// This method advances the circular buffer head pointer

	// If the buffer is full, then the head is not to advance. Otherwise, advance the head
	// Advancing has nothing to do with the tail pointer!
	// That is, this circular buffer will NOT let the user to overfill it!

	if (!is_full) {
		head = (head + msg_length) % (num_msg_capacity * msg_length);

		num_msgs_in_buffer++;

		// Mark is_full. Tail will be advanced on the next iteration
		is_full = (head == tail);
	}

	return;
}


void MsgCircularBuffer::advanceTailPointer(void) {
	// This method advances the circular buffer tail pointer
	
	is_full = 0;
	tail = (tail + msg_length) % (num_msg_capacity * msg_length);
	num_msgs_in_buffer--;

	return;
}


void MsgCircularBuffer::resetCircularBuffer(void) {
	// This method resets the circular buffer

	head = 0;
	tail = 0;
	is_full = 0;
	num_msgs_in_buffer = 0;

	return;
}


uint8_t MsgCircularBuffer::getCircularBufferNumMsgs(void) const {
	// This method returns the number of messages in the circular buffer
	return num_msgs_in_buffer;
}


uint8_t MsgCircularBuffer::getCircularBufferCapacity(void) const {
	// This method returns the maximum number of messages that the circular buffer can hold
	return num_msg_capacity;
}


void MsgCircularBuffer::circularBufferPushMsg(const char* msg_data_ptr) {
	// This method pushes a const char* messages onto the circular buffer

	char* strfunc_result_ptr = NULL;
	char* head_ptr = &(buffer_ptr[head]);

	if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
		Serial.print(F("\n ~ msg_data_ptr is: "));
		Serial.println(msg_data_ptr);
	}

	strfunc_result_ptr = strncpy(head_ptr, msg_data_ptr, (msg_length));
	buffer_ptr[head + msg_length - 1] = '\0';

	if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
		Serial.print(F("\n ~ msg_length is: "));
		Serial.println(msg_length);
		Serial.print(F(" ~ strfunc_result_ptr is: "));
		Serial.println(strfunc_result_ptr);
	}
	
	if (strfunc_result_ptr == NULL) {
		if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
			Serial.println(F("\n ~ Warning: strncpy returned a null pointer in attempting add the msg to the buffer!"));
		}
	}
	else {
		advanceHeadPointer();
	}

	return;
}


void MsgCircularBuffer::circularBufferPushMsg(char* msg_data_ptr) {
	// This method pushes a char* messages onto the circular buffer
	
	char* strfunc_result_ptr = NULL;
	char* head_ptr = &(buffer_ptr[head]);

	if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
		Serial.print(F("\n ~ msg_data_ptr is: "));
		Serial.println(msg_data_ptr);
		Serial.print(F(" ~ msg_data_ptr is: "));
		Serial.println(msg_data_ptr);
		Serial.print(F(" ~ strncpy Destination Address is: "));
		Serial.println(head_ptr);
	}

	strfunc_result_ptr = strncpy(head_ptr, msg_data_ptr, (msg_length));
	buffer_ptr[head + msg_length - 1] = '\0';

	if (strfunc_result_ptr == NULL) {
		if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
			Serial.println(F("\n ~ Warning: strncpy returned a null pointer in attempting add the msg to the buffer!"));
		}
	}
	else {
		advanceHeadPointer();
	}

	return;
}


char* MsgCircularBuffer::circularBufferPopMsg(void) {
	// This method pops a messages off of the circular buffer

	char* tail_ptr = nullptr;

	if (!circularBufferIsEmpty()) {
		tail_ptr = &(buffer_ptr[tail]);

		if (tail_ptr == NULL) {
			if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
				Serial.println(F(" ~ Warning, Tail Ptr is NULL! "));
			}
		}

		if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
			Serial.println(F("\n ~  In circularBufferPopMsg, Getting message from buffer."));
			Serial.print(F(" ~  tail_ptr is: "));
			Serial.println(tail_ptr);
			Serial.print(F(" ~  Msg retreived from the buffer is: "));
			Serial.println(tail_ptr);
			Serial.print(F("    tail_ptr Adr is: "));
			Serial.println(tail_ptr);
		}

		advanceTailPointer();
	}

	return tail_ptr;
}


char* MsgCircularBuffer::getCircularBufferNextMsg(void) {
	// This method returns the next messages from the circular buffer
	return circularBufferPopMsg();
}


uint8_t MsgCircularBuffer::circularBufferIsEmpty(void) const {
	// This method returns whether or not the circular buffer is empty
	return (!is_full && (head == tail));
}


uint8_t MsgCircularBuffer::circularBufferIsFull(void) const {
	// This method returns whether or not the circular buffer is full
	return is_full;
}


uint16_t MsgCircularBuffer::getHeadPointer(void) const {
	// This method returns the circular buffer head and is used for diagnostic purposes only
	return head;
}


uint16_t MsgCircularBuffer::getTailPointer(void) const {
	// This method returns the circular buffer tail and is used for diagnostic purposes only
	return tail;
}


void MsgCircularBuffer::initEntireBufferAry(void) {
	uint16_t msg_index = 0;

	//Serial.println(F(" ~  Initializing Entire Circular Buffer Array ..."));

	for (msg_index = 0; msg_index < (msg_length * num_msg_capacity); msg_index++) {
		circ_buffer[msg_index] = '\0';
	}

	//Serial.println(F("\n ~ Done Initializing Cicrular Buffer"));
	//Serial.println(F(" ~ Now Reading back the Circular Buffer ..."));

	//printEntireBufferAry();

	return;
}

// This method prints the entire buffer char by char
void MsgCircularBuffer::printEntireBufferAry(void) const {
	uint16_t msg_index = 0;

	Serial.println(F(" ~  Printing Entire Circular Buffer Data - Raw ..."));

	for (msg_index = 0; msg_index < (msg_length * num_msg_capacity); msg_index++) {
		Serial.println(circ_buffer[msg_index]);
	}
	
	Serial.println(F(" ~ Done Printing Entire Circular Buffer ..."));

	return;
}