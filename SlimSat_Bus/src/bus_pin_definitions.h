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

#define USING_SLIMSAT_MODULE_CONFIG 1

// Pin Numbering
#if (USING_SLIMSAT_MODULE_CONFIG == 1)
	// ItsyBitsy nRF Pin numbers for SlimSat Module Beta
	#define THERMISOTOR_AIN_PIN A0 // Unique Config Name, Analog Input does not need to be initialized
	#define LORA_NSS_CS_PIN 2 // Common Config Name
	#define BUS_PL_IF_D7_PIN 7 // Unique Config Name
	#define LORA_DIO0_G0_PIN 9 // Common Config Name
	#define BUS_WAG_WDT_PIN 10 // Unique Config Name
	#define LORA_INT_PIN 12 // Common Config Name
	#define LORA_RST_PIN 13 // Common Config Name
	
	// I2C Addresses
	#define TMP75A_I2C_ADR 0x4F 
#else
	// ItsyBitsy nRF Pin numbers for SW Dev Board
	#define SW_DEV_BUS_RESET_PIN A0 // DO // Unique Config Name
	#define THERMISOTOR_AIN_PIN A5 // Unique Config Name, Analog Input does not need to be initialized
	#define LORA_INT_PIN 2 // Common Config Name
	#define LORA_RST_PIN 7 // Common Config Name
	#define LORA_NSS_CS_PIN 9 // Common Config Name
	#define LORA_DIO0_G0_PIN 10 // Common Config Name
	#define BUS_PL_IF_D12_PIN 12 // Unique Config Name
	#define BUS_PL_IF_D13_PIN 13 // Unique Config Name
	
	// I2C Addresses
	#define TMP75A_I2C_ADR 0x4B
#endif

// Common Pin assignments
#define BUS_PL_IF_A1_PIN A1
#define BUS_PL_IF_A2_PIN A2
#define BUS_PL_IF_A3_PIN A3
#define BUS_PL_IF_A4_PIN A4
//#define BUS_PL_IF_A5_PIN A5
#define BUS_PL_IF_D5_PIN 5
#define CUTDOWN_DO_PIN 11
#define BUS_GPS_RX_PIN 0
#define BUS_GPS_TX_PIN 1

// Common I2C Addresses
#define INA219_1_I2C_ADR 0x45 // Battery Monitor
#define INA219_2_I2C_ADR 0x41 // System Monitor, was 42
#define INA219_3_I2C_ADR 0x40 // Solar Monitor
// Note: TMP75A_I2C_ADR is set in the conditional code above
//#define TMP75A_I2C_ADR 0x4F   // Panel Temp Sensor, was 4B, and still is 4B on the Dev board
#endif