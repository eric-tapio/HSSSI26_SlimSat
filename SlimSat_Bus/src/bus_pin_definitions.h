/**
 * @file bus_pin_definitions.h

 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * August 30, 2025
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef BUS_PIN_NUMBERS_HEADER
#define BUS_PIN_NUMBERS_HEADER

// Pin Numbering

// ItsyBitsy nRF Pin numbers
#define BUS_RESET_PIN A0 // Digital Output
#define BUS_UNUSED_A1_PIN A1
#define BUS_UNUSED_A2_PIN A2
#define BUS_UNUSED_A3_PIN A3
#define BUS_THERMISTOR_AIN_PIN A4 // Analog Input
#define BUS_UNUSED_A5_PIN A5
#define BUS_UNUSED_D2_PIN 2 // Was allocated for TMP Temp Sensor Alert Pin.  No longer being used
#define BUS_WAG_WDT_PIN 5  // Digital Output
#define LORA_RST_PIN 7 
#define LORA_NSS_CS_PIN 9
#define LORA_G0_PIN 10
//#define LORA_DIO1_PIN 11 // HMM, LORA is not using this pin! Should it be? TBD
#define CUTDOWN_DO_PIN 11 // Switching Pin 11 over to this instead

#define BUS_UNUSED_D12_PIN 12 // Set as an Output and set low for now
#define BUS_UNUSED_D13_PIN 13  // Set as an Output and set low for now


// I2C Addresses
#define INA219_1_I2C_ADR 0x45
#define INA219_2_I2C_ADR 0x42
#define INA219_3_I2C_ADR 0x40
#define TMP75A_I2C_ADR 0x4B

#endif