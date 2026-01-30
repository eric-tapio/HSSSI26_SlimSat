/**
 * @file msg_circular_buffer.h
 * @brief Message Circular Buffer Header
 * 
 * @details This header file defines the MsgCircularBuffer class which provides
 * a FIFO (First In, First Out) circular buffer for message storage and retrieval.
 * The buffer manages fixed-size message strings with automatic wraparound and
 * overflow protection for reliable message queuing in the SlimSat system.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef MSG_CIRCULAR_BUFFER_HEADER
#define MSG_CIRCULAR_BUFFER_HEADER

#include <Arduino.h>

#define BUFFER_CAPACITY_NUM_MSGS 20
#define BUFFER_MSG_LENGTH 32

//#define BUFFER_CAPACITY_NUM_MSGS 10
//#define BUFFER_MSG_LENGTH 34

#define VERBOSE_MSG_CIRCULAR_BUFFER_OUTPUT 0

// This is FIFO Message Circular Buffer

/**
 * @brief FIFO Message Circular Buffer Class
 * 
 * @details The MsgCircularBuffer class implements a circular buffer for storing
 * and retrieving fixed-size message strings. It provides FIFO operation with
 * automatic wraparound, overflow protection, and comprehensive buffer management.
 * The buffer maintains head and tail pointers for efficient message queuing.
 */
class MsgCircularBuffer {
private:
	// Data Members
	char circ_buffer[BUFFER_CAPACITY_NUM_MSGS * BUFFER_MSG_LENGTH]; // Main circular buffer storage
	char* buffer_ptr; // Pointer to the circular buffer
	uint16_t head; // Head pointer (write position) - uint16_t for large buffers
	uint16_t tail; // Tail pointer (read position) - uint16_t for large buffers
	uint8_t num_msg_capacity; // Maximum number of messages the buffer can hold
	uint8_t msg_length; // Length of each message in the buffer
	uint8_t num_msgs_in_buffer; // Current number of messages in buffer
	uint8_t is_full; // Buffer full flag (0=not full, 1=full)
	
	// Methods
	/**
	 * @brief Advance the head pointer
	 * @details Moves the head pointer to the next write position with wraparound
	 */
	void advanceHeadPointer(void);

	/**
	 * @brief Advance the tail pointer
	 * @details Moves the tail pointer to the next read position with wraparound
	 */
	void advanceTailPointer(void);

	/**
	 * @brief Reset circular buffer
	 * @details Resets all buffer pointers and counters to initial state
	 */
	void resetCircularBuffer(void); // Moving to public for testing
	
public:
	// Constructors
	/**
	 * @brief Default constructor for MsgCircularBuffer
	 * @details Initializes the circular buffer with default capacity and message length
	 */
	MsgCircularBuffer(void);
	
	// Methods
	/**
	 * @brief Print circular buffer information
	 * @details Outputs buffer state and configuration for debugging
	 */
	void print(void) const;

	/**
	 * @brief Print all messages in circular buffer
	 * @details Outputs all stored messages for debugging purposes
	 */
	void printCircularBufferMsgs(void) const;

	/**
	 * @brief Check if circular buffer is empty
	 * @details Returns whether the buffer contains any messages
	 * @return Empty state (0=not empty, 1=empty)
	 */
	uint8_t circularBufferIsEmpty(void) const;

	/**
	 * @brief Check if circular buffer is full
	 * @details Returns whether the buffer has reached maximum capacity
	 * @return Full state (0=not full, 1=full)
	 */
	uint8_t circularBufferIsFull(void) const;

	/**
	 * @brief Push message to circular buffer (const char*)
	 * @details Adds a message to the buffer from a const char pointer
	 * @param msg_data_ptr Pointer to message data to store
	 */
	void circularBufferPushMsg(const char* msg_data_ptr);

	/**
	 * @brief Push message to circular buffer (char*)
	 * @details Adds a message to the buffer from a char pointer
	 * @param msg_data Pointer to message data to store
	 */
	void circularBufferPushMsg(char* msg_data);

	/**
	 * @brief Get circular buffer capacity
	 * @details Returns the maximum number of messages the buffer can hold
	 * @return Buffer capacity in number of messages
	 */
	uint8_t getCircularBufferCapacity(void) const;

	/**
	 * @brief Get number of messages in circular buffer
	 * @details Returns the current number of messages stored in the buffer
	 * @return Current number of messages
	 */
	uint8_t getCircularBufferNumMsgs(void) const;

	/**
	 * @brief Pop message from circular buffer
	 * @details Removes and returns the oldest message from the buffer
	 * @return Pointer to the popped message, or nullptr if empty
	 */
	char* circularBufferPopMsg(void);

	/**
	 * @brief Get next message from circular buffer
	 * @details Alias for circularBufferPopMsg() - returns next message
	 * @return Pointer to the next message, or nullptr if empty
	 */
	char* getCircularBufferNextMsg(void);

	/**
	 * @brief Initialize entire buffer array
	 * @details Clears all buffer memory to null characters
	 */
	void initEntireBufferAry(void);

	/**
	 * @brief Print entire buffer array
	 * @details Outputs raw buffer contents character by character for debugging
	 */
	void printEntireBufferAry(void) const;

	// To be removed after unit tested
	/**
	 * @brief Get head pointer (diagnostic)
	 * @details Returns current head pointer position for testing
	 * @return Head pointer value
	 */
	uint16_t getHeadPointer(void) const;

	/**
	 * @brief Get tail pointer (diagnostic)
	 * @details Returns current tail pointer position for testing
	 * @return Tail pointer value
	 */
	uint16_t getTailPointer(void) const;
};


#endif