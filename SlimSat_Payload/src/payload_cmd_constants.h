/**
 * @file payload_cmd_constants.h
 * @brief Payload Command Constants Header
 * 
 * @details This header file defines command ID constants for payload operations
 * within the SlimSat system. These constants are used for command identification
 * and validation in payload command processing and response generation.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef PAYLOAD_CMD_CONSTANTS_HEADER
#define PAYLOAD_CMD_CONSTANTS_HEADER

// PayloadCmdConstantsHeader
#define MIN_PL_CMD_ID 50 // Minimum valid payload command ID
#define MAX_PL_CMD_ID 59 // Maximum valid payload command ID

// As a payload designer, I have decided on the following Cmd IDs
// You can define your own commands as needed
#define PING_PAYLOAD_CMD_ID 50 // Payload ping command
#define GET_NUMBER_MEASUREMENTS_TO_TAKE_CMD_ID 51 // Get number of measurements to take
#define SET_NUMBER_MEASUREMENTS_TO_TAKE_CMD_ID 52 // Set number of measurements to take
#define TAKE_MEASUREMENTS_CMD_ID 53 // Execute measurement sequence
#define PRINT_MEASUREMENTS_CMD_ID 54 // Print measurements to console

#endif
