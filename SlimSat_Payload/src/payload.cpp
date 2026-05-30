/**
 * @file payload.cpp
 * @brief SlimSat Payload System Implementation
 * 
 * @details This file implements the Payload class methods for managing
 * the payload subsystem including sensor operations, measurement collection,
 * data processing, and command handling for SlimSat payload instruments.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <payload.h>

/**
 * @brief Default constructor for the payload class
 * @details Initializes the payload system by setting up data members
 * and performing system initialization. This is the only constructor
 * available for the payload class.
 */
Payload::Payload(void) {
	// The default constructor, and only constructor, for the payload class

	return;
}


/**
 * @brief Initialize payload system
 * @details Performs complete payload initialization including hardware
 * configuration, pin setup, and system state initialization. Sets
 * payload to STOPPED state for safe startup.
 */
void Payload::initializePayload(void) {
	// This method initializes the payload, which can include setting hardware pin directions and input/output states
	Serial.println(F(" ~ Initializing Payload ...")); 
 
	// Set pin mode directions
	pinMode(ADC_INPUT_PIN, INPUT);
	pinMode(RADFET_CTRL_R2_PIN, OUTPUT);
	pinMode(RADFET_CTRL_EN_PIN, OUTPUT);
	pinMode(RADFET_CTRL_R1_PIN, OUTPUT);
	pinMode(V_BOOST_EN_PIN, OUTPUT);
	pinMode(XIAO_EN_PIN, OUTPUT);

	// Initialize pin states
	turn12vBoostOff();
	turnXiaoOff();
	setRadfetCtrlEnLow();
	setRadfetCtrlR1Low();
	setRadfetCtrlR2Low();
  
	return;
}


void Payload::performPayloadLoopIteration(PlDataRec& pl_data) {
	// This function performs the payload control actions required each time the payload is run
	if (VERBOSE_PAYLOAD_OUTPUT) {
		Serial.println(F("\n ~ Performing Payload Loop Iteration ..."));
	}
	
	readRadfetOutputs(pl_data);
	Serial.println();

	//turnXiaoOn();

	return;
}


void Payload::turn12vBoostOn(void) {
  // This function turns the 12V Boost ON

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Turning 12V Boost On ..."));
  }
  digitalWrite(V_BOOST_EN_PIN, HIGH);

  return;
}


void Payload::turn12vBoostOff(void) {
  // This function turns the 12V Boost OFF

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Turning 12V Boost Off ..."));
  }
  digitalWrite(V_BOOST_EN_PIN, LOW);
  return;
}


void Payload::turnXiaoOn(void) {
  // This function turns the FET switch ON, which turns the XIAO uC ON

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Turning Xiao uC On ..."));
  }
  digitalWrite(XIAO_EN_PIN, HIGH);

  return;
}


void Payload::turnXiaoOff(void) {
  // This function turns the FET switch OFF, which turns the XIAO uC OFF

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Turning Xiao uc Off ..."));
  }
  digitalWrite(XIAO_EN_PIN, LOW);
  return;
}


void Payload::setRadfetCtrlEnHigh(void) {
  // This function sets RADFET CTRL EN High

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Setting RADFET CTRL EN High ..."));
  }
  digitalWrite(RADFET_CTRL_EN_PIN, HIGH);

  return;
}


void Payload::setRadfetCtrlEnLow(void) {
  // This function sets RADFET CTRL EN Low

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Setting RADFET CTRL EN Low ..."));
  }
  digitalWrite(RADFET_CTRL_EN_PIN, LOW);
  return;
}


void Payload::setRadfetCtrlR1High(void) {
  // This function sets RADFET CTRL R1 High

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Setting RADFET CTRL R1 High ..."));
  }
  digitalWrite(RADFET_CTRL_R1_PIN, HIGH);

  return;
}


void Payload::setRadfetCtrlR1Low(void) {
  // This function sets RADFET CTRL R1 Low

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Setting RADFET CTRL R1 Low ..."));
  }
  digitalWrite(RADFET_CTRL_R1_PIN, LOW);
  return;
}


void Payload::setRadfetCtrlR2High(void) {
  // This function sets RADFET CTRL R2 High

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Setting RADFET CTRL R2 High ..."));
  }
  digitalWrite(RADFET_CTRL_R2_PIN, HIGH);

  return;
}


void Payload::setRadfetCtrlR2Low(void) {
  // This function sets RADFET CTRL R2 Low

  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.println(F(" ~ Setting RADFET CTRL R2 Low ..."));
  }
  digitalWrite(RADFET_CTRL_R2_PIN, LOW);
  
  return;
}


void Payload::readRadfetOutputs(PlDataRec& pl_data) {
	// This function reads the RADFET analog outputs

	double radfet_counts_value = 0.0;
  
	// Start the sequence by turning the 12V Boost ON
	turn12vBoostOn();

	// Give the 12V Boost time to start
	delay(SHORT_DELAY_IN_MS);

	// Then perform the read from the specified RADFET output
	for (uint8_t i=0; i<NUMBER_RADFETS; i++) {
		radfet_counts_value = readRadfetOutput(i);
		pl_data.setArrayElement(i, radfet_counts_value);
	}
	
	// Finish the sequence by turning the 12V Boost OFF
	turn12vBoostOff();

	pl_data.printArray();

  return;
}


uint16_t Payload::readRadfetOutput(uint8_t radfet_index) {
  if (VERBOSE_PAYLOAD_OUTPUT) {
    Serial.print(F(" ~ Getting Analog Output for RADFET "));
    Serial.println(radfet_index+1);
  }

  uint16_t adc_value_in_counts = 0;

  // First, transition CTRL_EN HIGH
  setRadfetCtrlEnHigh();
  
  // Then set the RADFET CTRL Rx HIGH
  if (radfet_index == 0) {
    setRadfetCtrlR1High();
  }
  else {
    setRadfetCtrlR2High();
  }

  // Wait a short delay
  delay(SHORT_DELAY_IN_MS);

  // Next, perform an analog read
  for (uint8_t i=0; i<NUM_ADC_MEASUREMENTS_TO_TAKE; i++) {
    adc_value_in_counts = analogRead(ADC_INPUT_PIN);

    // Display the results
    Serial.print(F(" ~ Analog Read value is: "));
    Serial.println(adc_value_in_counts);

    delay(VERY_SHORT_DELAY_IN_MS);
  }

  // Wait a short delay
  delay(SHORT_DELAY_IN_MS);

  // Then set the RADFET CTRL Rx LOW
  if (radfet_index == 0) {
    setRadfetCtrlR1Low();
  }
  else {
    setRadfetCtrlR2Low();
  }

  // Finaly, transition CTRL_EN LOW
  setRadfetCtrlEnLow();

  // Wait a short delay before the next iteration
  // This wait should stay in place
  delay(SHORT_DELAY_IN_MS);

  return adc_value_in_counts;
}