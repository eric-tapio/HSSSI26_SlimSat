//
//  tmp75a_if.h
//
// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef SLIMSAT_TMP75A_IF_HEADER
#define SLIMSAT_TMP75A_IF_HEADER

#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include <Wire.h>
#include <M2M_LM75A.h>  // temp sensor
#include <bus_pin_definitions.h>

#define VERBOSE_TMP75A_OUTPUT 0
 
class Tmp75aIf {
private:
	// Data Members
    M2M_LM75A lm75a;
    float temperature_in_deg_c;
	bool is_connected;

public:
    // Constructors
    Tmp75aIf(void);
  
    // Methods
	bool begin(void);
	float getTemperatureInDegC(void);
	bool isConnected(void) const;
    void print(void) const;
};

#endif