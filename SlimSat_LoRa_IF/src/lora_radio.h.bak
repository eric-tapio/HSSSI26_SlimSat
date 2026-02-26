/**
 * @file lora_radio.h
 * @brief LoRa Radio Communication Header
 * 
 * @details This header file defines the LoRaRadio class which provides
 * LoRa radio communication capabilities for the SlimSat system. It manages
 * radio configuration including frequency, bandwidth, spread factor, and
 * register access for LoRa communication operations.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

// Note: SlimSats are using the SX1276 LoRa

#ifndef LORA_RADIO_HEADER
#define LORA_RADIO_HEADER

#include <Arduino.h>
#include <RadioLib.h>
#include <bus_pin_definitions.h>

#define DEFAULT_SLIMSAT_FREQUENCY_IN_HZ 434000000
#define DEFAULT_SLIMSAT_BANDWIDTH_IN_HZ 125000
#define DEFAULT_SLIMSAT_SPREAD_FACTOR 9
#define DEFAULT_SLIMSAT_SYNC_WORD 0x12

#define DEFAULT_SLIMSAT_CODING_RATE 7
#define DEFAULT_SLIMSAT_OUTPUT_POWER_IN_DBM 1
#define DEFAULT_SLIMSAT_CURRENT_LIMIT_IN_MA 120
#define DEFAULT_SLIMSAT_PREAMBLE_LENGTH 8
#define DEFAULT_SLIMSAT_GAIN 0

#define MSG_BUFFER_SIZE (40+1)
#define RADIO_TX_WAIT_IN_MS 10
#define NUM_HZ_PER_MHZ 1000000
#define NUM_HZ_PER_KHZ 1000
#define COMPLETE_END_OF_TRANSMISSION_DELAY_IN_MS 10

#define VERBOSE_LORA_OUTPUT 0
//#define TRANSMIT_BEACON 0

/**
 * @brief LoRa Radio Communication Class
 * 
 * @details The LoRaRadio class provides comprehensive LoRa radio management
 * functionality including frequency control, bandwidth configuration, spread
 * factor adjustment, RSSI monitoring, and register access. This class serves
 * as a wrapper for LoRa radio operations within the SlimSat system.
 */
 
 
 // Note: to successfully receive data, the following
  // settings have to be the same on both transmitter
  // and receiver:
  // - carrier frequency
  // - bandwidth
  // - spreading factor
  // - coding rate
  // - sync word
  
  
  // If needed, 'listen' mode can be disabled by calling any of the following methods:
  // radio.standby()
  // radio.sleep()
  // radio.transmit();
  // radio.receive();
  // radio.scanChannel();
  
 // No longer class members
 //volatile uint8_t transmitted_flag;
 //void setReceiveFlag(void); 
 //void setTransmitFlag(void); 
void setOperationDoneFlag(void);

// Note:
//  SX1276 begin defaults are:
//		float freq = 434.0
//		float bw = 125.0
//		uint8_t sf = 9
//		uint8_t cr = 7
//		uint8_t syncWord = RADIOLIB_SX127X_SYNC_WORD -> 0x12        //  7     0   default LoRa sync word
//		int8_t power = 10
//		uint16_t preambleLength = 8
//		uint8_t gain = 0

class LoRaRadio{
private:
	// Data Members
	// Note: LORA_DIO1_PIN is no longer connected to the LORA! IS this a problem? TBD Substituting BUS_UNUSED_A5_PIN for now, which may be used, if necessary
	
	//SX1276 radio = new Module(LORA_NSS_CS_PIN, LORA_G0_PIN, LORA_RST_PIN, LORA_DIO1_PIN);
	SX1276 radio = new Module(LORA_NSS_CS_PIN, LORA_G0_PIN, LORA_RST_PIN, BUS_UNUSED_A5_PIN);

	uint32_t frequency_in_hz; // LoRa radio frequency in Hz
	uint8_t spread_factor; // LoRa spread factor, accepted range is 6 to 12
	uint8_t coding_rate; // LoRa coding rate, accepted range is 4 to 8
	uint32_t bandwidth_in_hz; // LoRa bandwidth in Hz, only certain bandwidths are accepted: KHZ_VALUES = [7800, 10400, 15600, 20800, 31250, 41700, 62500, 125000, 250000, 500000] 
	uint8_t sync_word; // LoRa syncword
	int8_t output_power_in_dbm; // LoRa output power, accepted range is -3 to 17 dBm
	uint8_t current_limit_in_ma; // LoRa current limit, accepted range is 45 to 240 mA
	uint16_t preamble_length;  // LoRa preamble length, accepted range is 6 to 65535
	uint8_t gain; // LoRa amplifier gain, accepted range is 1 to 6, where 1 is maximum gain
	float rssi; // Received Signal Strength Indicator
	float snr; // Signal to Noise Ratio
	float datarate; // Data rate (only used for Transmit Using Blocking)
	float frequency_error_in_hz; // Frequency Error in Hz
	uint8_t rx_msg_buffer[MSG_BUFFER_SIZE];
	int16_t radio_status_code; //
	
	// Radio Lib parameters
	int8_t radio_return_code;
	bool in_receive_mode;

public:
	// Constructors
	/**
	 * @brief Default constructor for LoRaRadio
	 * @details Initializes LoRa radio with default configuration values
	 */
	LoRaRadio(void);
	
	// Destructors
	//~LoRaRadio(void);
	
	int16_t beginUsingStandardDefaultValues(void);
	int16_t begin(void);
	
	// Methods
	/**
	 * @brief Initialize LoRa radio
	 * @details Sets up LoRa radio with default configuration parameters
	 */
	void initializeLoRa(void);

	/**
	 * @brief Set LoRa radio frequency
	 * @details Configures the operating frequency of the LoRa radio
	 * @param frequency_value_in_hz New frequency in Hz
	 */
	int16_t setFrequency(uint32_t frequency_value_in_hz);

	/**
	 * @brief Get LoRa radio frequency
	 * @details Returns the current operating frequency of the LoRa radio
	 * @return Current frequency in Hz
	 */
	uint32_t getFrequency(void) const;

	/**
	 * @brief Set LoRa radio bandwidth
	 * @details Configures the bandwidth of the LoRa radio
	 * @param bandwidth_value New bandwidth in Hz
	 */
	int16_t setBandwidth(uint32_t bandwidth_value_in_hz);

	/**
	 * @brief Get LoRa radio bandwidth
	 * @details Returns the current bandwidth of the LoRa radio
	 * @return Current bandwidth in Hz
	 */
	uint32_t getBandwidth(void) const;

	/**
	 * @brief Set LoRa radio spread factor
	 * @details Configures the spread factor for LoRa modulation
	 * @param spread_factor_value New spread factor (typically 6-12)
	 */
	int16_t setSpreadFactor(uint8_t spread_factor_value);

	/**
	 * @brief Get LoRa radio spread factor
	 * @details Returns the current spread factor setting
	 * @return Current spread factor
	 */
	uint8_t getSpreadFactor(void) const;
	
	int16_t setCodingRate(uint8_t coding_rate_value);
	uint8_t getCodingRate(void) const;

	int16_t setSyncWord(uint8_t sync_word_value);
	uint8_t getSyncWord(void) const;
	
	int16_t setOutputPower(int8_t output_power_value);
	int8_t getOutputPower(void) const;
	
	int16_t setCurrentLimit(uint8_t current_limit_value);
	uint8_t getCurrentLimit(void) const;

	int16_t setPreambleLength(uint16_t preamble_length_value);
	uint16_t getPreambleLength(void) const;	
	
	int16_t setGain(uint8_t gain_value);
	uint8_t getGain(void) const;
	
	int16_t transmitUsingBlocking(char* msg);
	
	int16_t transmitUsingInterrupt(char* msg);
	int16_t transmitUsingInterrupt(const char* msg);
	
	int16_t startTransmit(const char* msg);
	int16_t startTransmit(char* msg);
	//void finishTransmit(void);
	
	int16_t receiveUsingBlocking(void);
	//int16_t receiveUsingInterrupt(void);
	//int8_t receiveUsingInterrupt(void);
	char* receiveUsingInterrupt(void);
	int16_t startReceive(void);
	//uint8_t readData(void);
	
	void initializeRxMessageBuffer(void);
	void setRadioParameters(void);
	
	float covnertHzToMhz(uint32_t hertz_value);
	float covnertHzToKhz(uint32_t hertz_value);
	
	/**
	 * @brief Get LoRa radio RSSI
	 * @details Returns the Received Signal Strength Indicator value
	 * @return Current RSSI value
	 */
	float getRssi(void);
	float getSnr(void);
	float getFrequencyError(void);
	float getDataRate(void);

	/**
	 * @brief Print LoRa radio information
	 * @details Outputs current LoRa radio configuration for debugging
	 */
	void print(void) const;
	
	char* getReceivedMsg(void);
	//uint8_t readDataOriginal(void);
	
	void setInterruptAction(void);
	char* handleReceivedPacket(void);
	void handleEndOfTransmission(void);
	void transmitResponse(void);
};


#endif