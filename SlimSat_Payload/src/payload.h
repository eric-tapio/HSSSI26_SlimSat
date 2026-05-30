/**
 * @file payload.h
 * @brief SlimSat Payload System Header
 * 
 * @details This header file defines the Payload class which manages the
 * payload subsystem for the SlimSat spacecraft. It handles sensor operations,
 * measurement collection, data processing, and command execution for the
 * payload instruments including ultrasonic range sensors.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef SLIMSAT_PAYLOAD_HEADER
#define SLIMSAT_PAYLOAD_HEADER

#include <Arduino.h>
#include <payload_data.h>

#define VERBOSE_PAYLOAD_OUTPUT 1
#define VERY_SHORT_DELAY_IN_MS 100
#define SHORT_DELAY_IN_MS 1000
#define LONG_DELAY_IN_MS 5000

const uint8_t RADFET_CTRL_R2_PIN = A1;
const uint8_t RADFET_CTRL_EN_PIN = A2;
const uint8_t RADFET_CTRL_R1_PIN = A3;
const uint8_t V_BOOST_EN_PIN = A4;
const uint8_t ADC_INPUT_PIN = A5;
const uint8_t XIAO_EN_PIN = 9;
const uint8_t NUMBER_RADFETS = 2;
const uint8_t NUM_ADC_MEASUREMENTS_TO_TAKE = 5;


/**
 * @brief SlimSat Payload System Class
 * 
 * @details The Payload class manages the complete payload subsystem including
 * sensor operations, measurement collection, data processing, and command handling.
 * It can be expanded and customized by the Payload Team as needed for specific
 * mission requirements. The class integrates ultrasonic sensors and provides
 * comprehensive measurement and data management capabilities.
 */
class Payload {
private:
	// The private portion of the class can be changed to whatever is needed to tailor 
    // the interface to support the sensor(s) used by the SlimSat Team
	
	void turn12vBoostOn(void);
	void turn12vBoostOff(void);
	void turnXiaoOn(void);
	void turnXiaoOff(void);
	void setRadfetCtrlEnHigh(void);
	void setRadfetCtrlEnLow(void);
	void setRadfetCtrlR1High(void);
	void setRadfetCtrlR1Low(void);
	void setRadfetCtrlR2High(void);
	void setRadfetCtrlR2Low(void);
	void readRadfetOutputs(PlDataRec& pl_data_rec);
	uint16_t readRadfetOutput(uint8_t radfet_index);

public:
	// **************************************************
	// The following public interface shall not be modified.
	// However, new methods may be added, and requires approval by the U of L SW team
	
	// Constructors
	Payload(void);

	void initializePayload(void);
	void performPayloadLoopIteration(PlDataRec& pl_data_rec);
};

#endif