// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <lora_radio.h>


LoRaRadio::LoRaRadio(void) {
	// Default constructor for the LoRa Wrapper class
	initializeLoRa();
}


void LoRaRadio::initializeLoRa(void) {
	// This method initializes the LoRa Radio data members
	frequency = 438E6;
	spread_factor = 6;
	bandwidth = 31000;
	rssi = 1;
	register_adress = 0x10;
	register_value = 10;

	return;
}

void LoRaRadio::setFrequency(uint32_t frequency_value){
	// This method sets the LoRa radio frequency
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Frequency ..."));
	}

	frequency = frequency_value;

	return;
}


uint32_t LoRaRadio::getFrequency(void) const {
	// This method gets the LoRa radio frequency
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Frequency ..."));
	}

	return frequency;
}


void LoRaRadio::setBandwidth(uint32_t bandwidth_value) {
	// This method sets the LoRa radio bandwidth
		if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Bandwidth ..."));
	}

	bandwidth = bandwidth_value;

	return;
}


uint32_t LoRaRadio::getBandwidth(void) const {
	// This method gets the LoRa radio bandwidth
		if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Bandwidth ..."));
	}

	return bandwidth;
}


void LoRaRadio::setSpreadFactor(uint32_t spread_factor_value) {
	// This method sets the LoRa radio spread factor
		if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Spread Factor ..."));
	}

	spread_factor = spread_factor_value;

	return;
}


uint32_t LoRaRadio::getSpreadFactor(void) const {
	// This method gets the LoRa radio spread factor
		if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Spread Factor ..."));
	}

	return spread_factor;
}


void LoRaRadio::setRegister(uint16_t register_adr, uint16_t register_val) {
	// This method sets the specified LoRa radio register
		if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Register Value ..."));
	}

	register_adress = register_adr;
	register_value = register_val;

	return;
}


uint16_t LoRaRadio::getRegister(uint16_t register_adr) const {
	// This method gets the specified LoRa radio register
		if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Register Value ..."));
	}
	
	return register_value;
}


uint32_t LoRaRadio::getRssi(void) const {
	// This method gets the LoRa radio RSSI
		if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa RSSI Value ..."));
	}
	
	return rssi;
}


void LoRaRadio::print(void) const {
	// This method prints the Lora Radio Wrapper class data members
		if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Printing LoRa Data ..."));
	}

	Serial.print(F("    Frequency is: "));
	Serial.println(frequency);
	Serial.print(F("    Bandwidth is: "));
	Serial.println(bandwidth);
	Serial.print(F("    Spread Factor is: "));
	Serial.println(spread_factor);
	Serial.print(F("    RSSI is: "));
	Serial.println(rssi);
	Serial.print(F("    Register Address is: "));
	Serial.println(register_adress);
	Serial.print(F("    Register Value is: "));
	Serial.println(register_value);
	Serial.println(F(""));

	return;
}