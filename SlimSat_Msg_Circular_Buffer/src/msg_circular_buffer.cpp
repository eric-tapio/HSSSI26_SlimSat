/**
 * @file msg_circular_buffer.cpp
 * @brief Message Circular Buffer Implementation
 * 
 * @details This file implements the MsgCircularBuffer class methods for managing
 * a FIFO circular buffer system. Provides message storage, retrieval, and buffer
 * management with automatic wraparound and overflow protection.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <msg_circular_buffer.h>

/**
 * @brief Default constructor for the message circular buffer class
 * @details Initializes the circular buffer with default configuration including
 * buffer pointers, capacity settings, and clears all buffer memory.
 */
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


/**
 * @brief Print circular buffer information
 * @details Outputs comprehensive buffer state including pointers, capacity,
 * message count, and buffer status. Also prints all stored messages.
 */
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


/**
 * @brief Print all messages in circular buffer
 * @details Iterates through all buffer positions and outputs each message
 * with its index and memory address for debugging purposes.
 */
void MsgCircularBuffer::printCircularBufferMsgs(void) const {
	//  This method prints the messages in the circular buffer
	
	uint8_t msg_index = 0;
	uint16_t start_index = 0;
	char* msg_data_ptr = nullptr;

	Serial.println(F("\n ~  Printing Circular Buffer Msgs ..."));

	for (msg_index = 0; msg_index < num_msg_capacity; msg_index++) {
		start_index = msg_index * msg_length;
		msg_data_ptr = &(buffer_ptr[start_index]);

		Serial.print(F("\n      start_index["));
		Serial.print(msg_index);
		Serial.print(F("] = "));
		Serial.println(start_index);
		Serial.print(F("      Buffer Data Msg["));
		Serial.print(msg_index);
		Serial.print(F("] = "));
		Serial.println(msg_data_ptr);
	}

	Serial.println(F("\n ~ Done Printing Circular Buffer Msgs ..."));

	return;
}


/**
 * @brief Advance the head pointer
 * @details Moves the head pointer to the next write position with wraparound.
 * Prevents buffer overflow by not advancing when buffer is full. Updates
 * message count and full status accordingly.
 */
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


/**
 * @brief Advance the tail pointer
 * @details Moves the tail pointer to the next read position with wraparound.
 * Clears the full flag and decrements the message count.
 */
void MsgCircularBuffer::advanceTailPointer(void) {
	// This method advances the circular buffer tail pointer
	
	is_full = 0;
	tail = (tail + msg_length) % (num_msg_capacity * msg_length);
	num_msgs_in_buffer--;

	return;
}


/**
 * @brief Reset circular buffer
 * @details Resets all buffer pointers and counters to initial state.
 * Clears head, tail, full flag, and message count.
 */
void MsgCircularBuffer::resetCircularBuffer(void) {
	// This method resets the circular buffer

	head = 0;
	tail = 0;
	is_full = 0;
	num_msgs_in_buffer = 0;

	return;
}


/**
 * @brief Get number of messages in circular buffer
 * @details Returns the current count of messages stored in the buffer.
 * @return Current number of messages in buffer
 */
uint8_t MsgCircularBuffer::getCircularBufferNumMsgs(void) const {
	// This method returns the number of messages in the circular buffer
	return num_msgs_in_buffer;
}


/**
 * @brief Get circular buffer capacity
 * @details Returns the maximum number of messages the buffer can hold.
 * @return Buffer capacity in number of messages
 */
uint8_t MsgCircularBuffer::getCircularBufferCapacity(void) const {
	// This method returns the maximum number of messages that the circular buffer can hold
	return num_msg_capacity;
}


/**
 * @brief Push message to circular buffer (const char*)
 * @details Adds a message to the buffer from a const char pointer.
 * Uses strncpy for safe copying and advances head pointer on success.
 * @param msg_data_ptr Pointer to message data to store
 */
void MsgCircularBuffer::circularBufferPushMsg(const char* msg_data_ptr) {
	// This method pushes a const char* messages onto the circular buffer

	char* strfunc_result_ptr = nullptr;
	char* head_ptr = &(buffer_ptr[head]);

	if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
		Serial.print(F("\n ~ msg_data_ptr is: "));
		Serial.println(msg_data_ptr);
	}

	if (msg_data_ptr != nullptr) {
		strfunc_result_ptr = strncpy(head_ptr, msg_data_ptr, (msg_length));
		buffer_ptr[head + msg_length - 1] = '\0';

		if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
			Serial.print(F("\n ~ msg_length is: "));
			Serial.println(msg_length);
			Serial.print(F(" ~ strfunc_result_ptr is: "));
			Serial.println(strfunc_result_ptr);
		}
		
		if (strfunc_result_ptr == nullptr) {
			if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
				Serial.println(F("\n ~ Warning: strncpy returned a null pointer in attempting add the msg to the buffer!"));
			}
		}
		else {
			advanceHeadPointer();
		}
	}
	else {
		if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
			Serial.print(F("\n ~ Ignoring nullptr or empty nessage. Not Pushing ..."));
		}
	}
	
	return;
}


/**
 * @brief Push message to circular buffer (char*)
 * @details Adds a message to the buffer from a char pointer.
 * Uses strncpy for safe copying and advances head pointer on success.
 * @param msg_data_ptr Pointer to message data to store
 */
void MsgCircularBuffer::circularBufferPushMsg(char* msg_data_ptr) {
	// This method pushes a char* messages onto the circular buffer
	
	char* strfunc_result_ptr = nullptr;
	char* head_ptr = &(buffer_ptr[head]);

	if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
		Serial.print(F("\n ~ msg_data_ptr is: "));
		Serial.println(msg_data_ptr);
		Serial.print(F(" ~ msg_data_ptr is: "));
		Serial.println(msg_data_ptr);
		Serial.print(F(" ~ strncpy Destination Address is: "));
		Serial.println(head_ptr);
	}

	if (msg_data_ptr != nullptr) {
		strfunc_result_ptr = strncpy(head_ptr, msg_data_ptr, (msg_length));
		buffer_ptr[head + msg_length - 1] = '\0';

		if (strfunc_result_ptr == nullptr) {
			if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
				Serial.println(F("\n ~ Warning: strncpy returned a null pointer in attempting add the msg to the buffer!"));
			}
		}
		else {
			advanceHeadPointer();
		}
	}
	else {
		if (VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT) {
			Serial.print(F("\n ~ Ignoring nullptr or empty nessage. Not Pushing ..."));
		}
	}
	
	return;
}


/**
 * @brief Pop message from circular buffer
 * @details Removes and returns the oldest message from the buffer.
 * Advances tail pointer and returns pointer to the message data.
 * @return Pointer to the popped message, or nullptr if empty
 */
char* MsgCircularBuffer::circularBufferPopMsg(void) {
	// This method pops a messages off of the circular buffer

	char* tail_ptr = nullptr;

	if (!circularBufferIsEmpty()) {
		tail_ptr = &(buffer_ptr[tail]);

		if (tail_ptr == nullptr) {
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


/**
 * @brief Get next message from circular buffer
 * @details Alias for circularBufferPopMsg() - returns and removes the next message.
 * @return Pointer to the next message, or nullptr if empty
 */
char* MsgCircularBuffer::getCircularBufferNextMsg(void) {
	// This method returns the next messages from the circular buffer
	return circularBufferPopMsg();
}


/**
 * @brief Check if circular buffer is empty
 * @details Returns whether the buffer contains any messages by checking
 * if buffer is not full and head equals tail.
 * @return Empty state (0=not empty, 1=empty)
 */
uint8_t MsgCircularBuffer::circularBufferIsEmpty(void) const {
	// This method returns whether or not the circular buffer is empty
	return (!is_full && (head == tail));
}


/**
 * @brief Check if circular buffer is full
 * @details Returns the current full status of the buffer.
 * @return Full state (0=not full, 1=full)
 */
uint8_t MsgCircularBuffer::circularBufferIsFull(void) const {
	// This method returns whether or not the circular buffer is full
	return is_full;
}


/**
 * @brief Get head pointer (diagnostic)
 * @details Returns current head pointer position for testing and debugging.
 * This method is used for diagnostic purposes only.
 * @return Head pointer value
 */
uint16_t MsgCircularBuffer::getHeadPointer(void) const {
	// This method returns the circular buffer head and is used for diagnostic purposes only
	return head;
}


/**
 * @brief Get tail pointer (diagnostic)
 * @details Returns current tail pointer position for testing and debugging.
 * This method is used for diagnostic purposes only.
 * @return Tail pointer value
 */
uint16_t MsgCircularBuffer::getTailPointer(void) const {
	// This method returns the circular buffer tail and is used for diagnostic purposes only
	return tail;
}


/**
 * @brief Initialize entire buffer array
 * @details Clears all buffer memory by setting every character to null terminator.
 * This ensures a clean buffer state for reliable operation.
 */
void MsgCircularBuffer::initEntireBufferAry(void) {
	//uint16_t msg_index = 0;

	//Serial.println(F(" ~  Initializing Entire Circular Buffer Array ..."));

	//for (msg_index = 0; msg_index < (msg_length * num_msg_capacity); msg_index++) {
	//	circ_buffer[msg_index] = '\0';
	//}

	//Serial.println(F("\n ~ Done Initializing Cicrular Buffer"));
	//Serial.println(F(" ~ Now Reading back the Circular Buffer ..."));

	//printEntireBufferAry();
	
	memset(circ_buffer, 0, sizeof(circ_buffer));

	return;
}


/**
 * @brief Print entire buffer array
 * @details Outputs raw buffer contents character by character for low-level
 * debugging and buffer analysis. Shows the complete buffer memory state.
 */
void MsgCircularBuffer::printEntireBufferAry(void) const {
	uint16_t msg_index = 0;

	Serial.println(F(" ~  Printing Entire Circular Buffer Data - Raw ..."));

	for (msg_index = 0; msg_index < (msg_length * num_msg_capacity); msg_index++) {
		Serial.println(circ_buffer[msg_index]);
	}
	
	Serial.println(F(" ~ Done Printing Entire Circular Buffer ..."));

	return;
}