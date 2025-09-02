// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#ifndef SLIMSAT_LORA_RADIO_HEADER
#define SLIMSAT_LORA_RADIO_HEADER

#include <Arduino.h>

#define VERBOSE_LORA_OUTPUT 0

class LoRaRadio{
private:
	// Data Members
	uint32_t frequency;
	uint32_t spread_factor;
	uint32_t bandwidth;
	uint32_t rssi;
	uint16_t register_adress;
	uint16_t register_value;

public:
	// Constructors
	LoRaRadio(void);
	
	// Methods
	void initializeLoRa(void);
	void setFrequency(uint32_t frequency_value);
	uint32_t getFrequency(void) const;
	void setBandwidth(uint32_t bandwidth_value);
	uint32_t getBandwidth(void) const;
	void setSpreadFactor(uint32_t spread_factor_value);
	uint32_t getSpreadFactor(void) const;
	uint32_t getRssi(void) const;
	void setRegister(uint16_t register_adr, uint16_t register_val);
	uint16_t getRegister(uint16_t register_adr) const;
	void print(void) const;
};


#endif