/**
 * @file lora_radio.cpp
 * @brief LoRa Radio Communication Implementation
 * 
 * @details This file implements the LoRaRadio class methods for managing
 * LoRa radio communication within the SlimSat system. Provides frequency
 * control, bandwidth_in_hz management, spread factor configuration, and register access.
 * 
 * CAPE-Twiggs HSSSI-26 SlimSat Project
 * Copyright (c) 2025, Eric Tapio. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <lora_radio.h>

// flag to indicate that a packet was sent or received
volatile bool packet_received = false;

// this function is called when a complete packet
// is transmitted or received by the module
void setPacketReceivedFlag(void) {
	// we sent or received  packet, set the flag
	packet_received = true;
}


/**
 * @brief Default constructor for the LoRa Wrapper class
 * @details Initializes the LoRa radio with default configuration by
 * calling the initialization method.
 */
LoRaRadio::LoRaRadio(void) {
	// Default constructor for the LoRa Wrapper class
	initializeLoRa();
}


/**
 * @brief Initialize LoRa radio
 * @details Sets up LoRa radio with default configuration parameters including
 * frequency (438MHz), spread factor (6), bandwidth (31kHz), and register settings.
 */
void LoRaRadio::initializeLoRa(void) {
	// This method initializes the LoRa Radio data members
	frequency_in_hz = DEFAULT_SLIMSAT_FREQUENCY_IN_HZ;
	bandwidth_in_hz = DEFAULT_SLIMSAT_BANDWIDTH_IN_HZ;
	spread_factor = DEFAULT_SLIMSAT_SPREAD_FACTOR;
	sync_word = DEFAULT_SLIMSAT_SYNC_WORD;
	coding_rate = DEFAULT_SLIMSAT_CODING_RATE;
	output_power_in_dbm = DEFAULT_SLIMSAT_OUTPUT_POWER_IN_DBM;
	current_limit_in_ma = DEFAULT_SLIMSAT_CURRENT_LIMIT_IN_MA;
	preamble_length = DEFAULT_SLIMSAT_PREAMBLE_LENGTH;
	gain = DEFAULT_SLIMSAT_GAIN;
	rssi = 0.0;
	snr = 0.0;
	frequency_error_in_hz = 0.0;
	datarate = 0.0;
	radio_status_code = 0; 
	
	initializeRxMessageBuffer();

	// Radio Lib 
	radio_return_code = RADIOLIB_ERR_NONE; // save transmission state between loops
	in_receive_mode = true; // Thus not in transmitting mode
		
	return;
}


void LoRaRadio::setInterruptAction(void) {
	// Setting the Interrupt action should only be done after starting the LoRa
	// Set the function that will be called when new packet is received
	radio.setPacketReceivedAction(setPacketReceivedFlag);
	
	return;
}


void LoRaRadio::setRadioParameters(void) {
	// This method sets the radio settings to default values
	
	setFrequency(frequency_in_hz);
	setBandwidth(bandwidth_in_hz);
	setSpreadFactor(spread_factor);
	setSyncWord(sync_word);
	setOutputPower(output_power_in_dbm);
	setFrequency(preamble_length);
	setGain(gain);
	setCodingRate(coding_rate); // Not used in begin
	setCurrentLimit(current_limit_in_ma); // Not used in begin
	
	return;
}


int16_t LoRaRadio::beginUsingStandardDefaultValues(void) {
	// This method starts the LoRa Radio using Radio Lib standard default values
	radio_status_code = radio.begin();
	
	if (VERBOSE_LORA_OUTPUT) {
		if (radio_status_code == RADIOLIB_ERR_NONE) {
			Serial.println(F("[SX1276] Success in Starting LoRa!"));
		} 
		else {
			Serial.print(F("[SX1276] Failed, code "));
			Serial.println(radio_status_code);
		}
	}
	
	// Transition the radio into receive mode
	radio_return_code = radio.startReceive();
	
	if (VERBOSE_LORA_OUTPUT) {
		if (radio_return_code == RADIOLIB_ERR_NONE) {
		  Serial.println(F("[SX1276] Success in transitioning LoRa to Receive!"));
		} 
		else {
		  Serial.print(F("[SX1276] Failed, code "));
		  Serial.println(radio_return_code);
		}
	}
	
	setInterruptAction();
	
	return radio_status_code;
}


int16_t LoRaRadio::begin(void) {
	// This method starts the LoRa Radio
	radio_status_code = radio.begin(covnertHzToMhz(frequency_in_hz), covnertHzToKhz(bandwidth_in_hz), spread_factor, coding_rate, sync_word, output_power_in_dbm, preamble_length, gain);
	
	if (VERBOSE_LORA_OUTPUT) {
		if (radio_status_code == RADIOLIB_ERR_NONE) {
			Serial.println(F("[SX1276] Success in Starting LoRa!"));
		} 
		else {
			Serial.print(F("[SX1276] Failed, code "));
			Serial.println(radio_status_code);
		}
	}
	
	// Transition the radio into receive mode
	radio_return_code = radio.startReceive();
	
	if (VERBOSE_LORA_OUTPUT) {
		if (radio_return_code == RADIOLIB_ERR_NONE) {
		  Serial.println(F("[SX1276] Success in transitioning LoRa to Receive!"));
		} 
		else {
		  Serial.print(F("[SX1276] Failed, code "));
		  Serial.println(radio_return_code);
		}
	}
	
	setInterruptAction();
	
	return radio_status_code;
}


float LoRaRadio::covnertHzToMhz(uint32_t hertz_value) {
	// This method converts a value in Hz to MHz
	return float(hertz_value) / NUM_HZ_PER_MHZ;
}


float LoRaRadio::covnertHzToKhz(uint32_t hertz_value) {
	// This method converts a value in Hz to kHz
	return float(hertz_value) / NUM_HZ_PER_KHZ;
}


/**
 * @brief Set LoRa radio frequency
 * @details Configures the operating frequency of the LoRa radio with
 * optional verbose output for debugging purposes.
 * @param frequency_value_in_hz New frequency in MHz
 */
int16_t LoRaRadio::setFrequency(uint32_t frequency_value_in_hz){
	// This method sets the LoRa radio frequency
	
	// Set the private data member to the input value
	frequency_in_hz = frequency_value_in_hz;
	
	float frequency_value_in_mhz = covnertHzToMhz(frequency_value_in_hz);
	
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Frequency ..."));
		Serial.print(F("\n    frequency_value_in_hz is: "));
		Serial.println(frequency_value_in_hz);
	}
	
	radio_status_code = radio.setFrequency(frequency_value_in_mhz);

	if (VERBOSE_LORA_OUTPUT) {
		if (radio_status_code == RADIOLIB_ERR_INVALID_FREQUENCY) {
			Serial.println(F("[SX1276] Selected frequency is invalid for this module!"));
		}
		else if (radio_status_code == RADIOLIB_ERR_NONE) {
			Serial.print(F("[SX1276] Frequency set to: "));
			Serial.print(frequency_value_in_mhz);
			Serial.println(F(" MHz"));
		} 
		else {
			Serial.print(F("[SX1276] Failed to set frequency, code: "));
			Serial.println(radio_status_code);
		}
	}
	//print();
	
	return radio_status_code;
}


/**
 * @brief Get LoRa radio frequency
 * @details Returns the current operating frequency of the LoRa radio
 * with optional verbose output for debugging.
 * @return Current frequency in Hz
 */
uint32_t LoRaRadio::getFrequency(void) const {
	// This method gets the LoRa radio frequency
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Frequency ..."));
	}
	
	return frequency_in_hz;
}


/**
 * @brief Set LoRa radio bandwidth
 * @details Configures the bandwidth of the LoRa radio with optional
 * verbose output for debugging purposes.
 * @param bandwidth_value_in_hz New bandwidth in kHz
 */
int16_t LoRaRadio::setBandwidth(uint32_t bandwidth_value_in_hz) {
	// This method sets the LoRa radio bandwidth
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Bandwidth ..."));
	}
	
	//Serial.print(F(" ~ bandwidth_value_in_hz is: "));
	//Serial.println(bandwidth_value_in_hz);
	
	// Set the private data member to the input value
	bandwidth_in_hz = bandwidth_value_in_hz;
	
	float bandwidth_value_in_khz = covnertHzToKhz(bandwidth_value_in_hz);
	
	//Serial.print(F(" ~ bandwidth_value_in_khz is: "));
	//Serial.println(bandwidth_value_in_khz);
	
	radio_status_code = radio.setBandwidth(bandwidth_value_in_khz);

	if (radio_status_code == RADIOLIB_ERR_INVALID_BANDWIDTH) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Selected bandwidth is invalid for this module!"));
		}
	}
	
	return radio_status_code;
}


/**
 * @brief Get LoRa radio bandwidth
 * @details Returns the current bandwidth of the LoRa radio with
 * optional verbose output for debugging purposes.
 * @return Current bandwidth in Hz
 */
uint32_t LoRaRadio::getBandwidth(void) const {
	// This method gets the LoRa radio bandwidth
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Bandwidth ..."));
	}
	
	return bandwidth_in_hz;
}


/**
 * @brief Set LoRa radio spread factor
 * @details Configures the spread factor for LoRa modulation with
 * optional verbose output for debugging purposes.
 * @param spread_factor_value New spread factor (typically 6-12)
 */
int16_t LoRaRadio::setSpreadFactor(uint8_t spread_factor_value) {
	// This method sets the LoRa radio spread factor
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Spread Factor ..."));
	}

	// Set the private data member to the input value
	spread_factor = spread_factor_value;
		
	radio_status_code = radio.setSpreadingFactor(spread_factor_value);

	if (radio_status_code == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Selected spread factor is invalid for this module!"));
		}
	}

	return radio_status_code;
}


/**
 * @brief Get LoRa radio spread factor
 * @details Returns the current spread factor setting with optional
 * verbose output for debugging purposes.
 * @return Current spread factor
 */
uint8_t LoRaRadio::getSpreadFactor(void) const {
	// This method gets the LoRa radio spread factor
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Spread Factor ..."));
	}

	return spread_factor;
}


int16_t LoRaRadio::setCodingRate(uint8_t coding_rate_value) {
	// This method sets the LoRa radio coding rate
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Coding Rate ..."));
	}

	// Set the private data member to the input value
	coding_rate = coding_rate_value;
		
	radio_status_code = radio.setCodingRate(coding_rate_value);

	if (radio_status_code == RADIOLIB_ERR_INVALID_CODING_RATE) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Selected coding rate is invalid for this module!"));
		}
	}

	return radio_status_code;
}


uint8_t LoRaRadio::getCodingRate(void) const {
	// This method gets the LoRa radio coding rate
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Coding Rate ..."));
	}

	return coding_rate;
}


int16_t LoRaRadio::setSyncWord(uint8_t sync_word_value) {
	// This method sets the LoRa radio sync word
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Sync Word ..."));
	}

	// Set the private data member to the input value
	sync_word = sync_word_value;
		
	radio_status_code = radio.setSyncWord(sync_word_value);

	if (radio_status_code == RADIOLIB_ERR_NONE) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Unable to set sync word!"));
		}
	}

	return radio_status_code;
}


uint8_t LoRaRadio::getSyncWord(void) const {
	// This method gets the LoRa radio sync word
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Sync Word ..."));
	}

	return sync_word;	
}


int16_t LoRaRadio::setOutputPower(int8_t output_power_value_in_dbm){
	// This method sets the LoRa radio output power
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Output Power ..."));
	}

	// Set the private data member to the input value
	output_power_in_dbm = output_power_value_in_dbm;
		
	radio_status_code = radio.setOutputPower(output_power_value_in_dbm);

	if (radio_status_code == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Selected output power is invalid for this module!"));
		}
	}

	return radio_status_code;
}


int8_t LoRaRadio::getOutputPower(void) const {
	// This method gets the LoRa radio output power
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Output Power ..."));
	}

	return output_power_in_dbm;
}


int16_t LoRaRadio::setCurrentLimit(uint8_t current_limit_value_in_ma) {
	// This method sets the LoRa radio current limit
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Current Limit ..."));
	}

	// Set the private data member to the input value
	current_limit_in_ma = current_limit_value_in_ma;
		
	radio_status_code = radio.setCurrentLimit(current_limit_value_in_ma);

	if (radio_status_code == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Selected current limit is invalid for this module!"));
		}
	}
	
	return radio_status_code;
}


uint8_t LoRaRadio::getCurrentLimit(void) const {
	// This method gets the LoRa radio current limit
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Current Limit ..."));
	}

	return current_limit_in_ma;
}
	
	
int16_t LoRaRadio::setPreambleLength(uint16_t preamble_length_value) {
	// This method sets the LoRa radio preamble length
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Preamble Length ..."));
	}

	// Set the private data member to the input value
	preamble_length = preamble_length_value;
		
	radio_status_code = radio.setPreambleLength(preamble_length_value);

	if (radio_status_code == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Selected preamble length is invalid for this module!"));
		}
	}
	
	return radio_status_code;
}


uint16_t LoRaRadio::getPreambleLength(void) const {
	// This method gets the LoRa radio preamble length
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Preamble Length ..."));
	}

	return preamble_length;
}
	

int16_t LoRaRadio::setGain(uint8_t gain_value) {
	// This method sets the LoRa radio gain
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Setting LoRa Gain ..."));
	}

	// Set the private data member to the input value
	gain = gain_value;
		
	radio_status_code = radio.setGain(gain_value);

	if (radio_status_code == RADIOLIB_ERR_INVALID_GAIN) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Selected gain is invalid for this module!"));
		}
	}
	
	return radio_status_code;
}


uint8_t LoRaRadio::getGain(void) const {
	// This method gets the LoRa radio gain
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Gain ..."));
	}

	return gain;
}


int16_t LoRaRadio::transmitUsingInterrupt(const char* msg) {
	// This method transmitts a message (using an interrupt)
	
	in_receive_mode = false;
	radio_return_code = startTransmit(msg);
	
	if (radio_return_code == RADIOLIB_ERR_NONE) {
		if (VERBOSE_LORA_OUTPUT) {
			// Packet was successfully sent
			Serial.println(F("[SX1276] Transmission successful/finished!"));
		}
	}
	else {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.print(F("[SX1276] Failed, code "));
			Serial.println(radio_return_code);
		}
	}
		
	return radio_return_code;
}


int16_t LoRaRadio::transmitUsingInterrupt(char* msg) {
	// This method transmitts a message (using an interrupt)
	
	in_receive_mode = false;
	radio_return_code = startTransmit(msg);
	
	if (radio_return_code == RADIOLIB_ERR_NONE) {
		if (VERBOSE_LORA_OUTPUT) {
			// Packet was successfully sent
			Serial.println(F("[SX1276] Transmission successful/finished!"));
		}
	}
	else {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.print(F("[SX1276] Failed, code "));
			Serial.println(radio_return_code);
		}
	}
		
	return radio_return_code;
}


int16_t LoRaRadio::startTransmit(const char* msg) {
	// This method starts the transmission of a message (using an interrupt)
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Starting Transmit ..."));
		Serial.print(F("   msg is: "));
		Serial.println(msg);
	}
	
	in_receive_mode = false; // Set the radio to Transmit Mode
	radio_return_code = radio.startTransmit(msg);

	return radio_return_code;
}


int16_t LoRaRadio::startTransmit(char* msg) {
	// This method starts the transmission of a message (using an interrupt)

	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Starting Transmit ..."));
		Serial.print(F("   msg is: "));
		Serial.println(msg);
	}
	
	in_receive_mode = false; // Set the radio to Transmit Mode
	radio_return_code = radio.startTransmit(msg);

	return radio_return_code;
}


int16_t LoRaRadio::transmitUsingBlocking(char* msg) {
	// This method transmits a message (blocking) 
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Transmitting (Blocking) ..."));
	}
	
	radio_status_code = radio.transmit(msg);

	if (radio_status_code == RADIOLIB_ERR_NONE) {
		// The packet was successfully transmitted
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(F("[SX1276] Success!"));
		}
		
		// Get DataRate has been removed from RadioLib in Vers 7.6.0
		//getDataRate();
	}
	else if (radio_status_code == RADIOLIB_ERR_PACKET_TOO_LONG) {
		if (VERBOSE_LORA_OUTPUT) {
			// The supplied packet was longer than 256 bytes
			Serial.println(F("[SX1276] Too long!"));
		}
	}
	else if (radio_status_code == RADIOLIB_ERR_TX_TIMEOUT) {
		if (VERBOSE_LORA_OUTPUT) {
			// Timeout occurred while transmitting packet
			Serial.println(F("[SX1276] Timeout!"));
		}
	}
	else {
		if (VERBOSE_LORA_OUTPUT) {
			// some other error occurred
			Serial.print(F("[SX1276] Failed, code "));
			Serial.println(radio_status_code);
		}
	}
  
	return radio_status_code;
}


int16_t LoRaRadio::receiveUsingBlocking(void) {
	// This method receives a message (blocking) 
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Receiving (Blocking) ..."));
	}
	
	initializeRxMessageBuffer();
	radio_status_code = radio.receive(rx_msg_buffer, MSG_BUFFER_SIZE);
	
	if (radio_status_code == RADIOLIB_ERR_NONE) {
		if (VERBOSE_LORA_OUTPUT) {
			// packet was successfully received
			Serial.println(F("[SX1276] Success!"));

			// print the data of the packet
			Serial.print(F("[SX1276] Data:\t\t\t"));
			//Serial.println(rx_msg_buffer);
			char* char_rx_msg_buffer = reinterpret_cast<char*>(rx_msg_buffer);
			Serial.println(char_rx_msg_buffer);
		}
		
		getRssi();
		getSnr();
		getFrequencyError();
	} 

	if (VERBOSE_LORA_OUTPUT) {
		if (radio_status_code == RADIOLIB_ERR_RX_TIMEOUT) {
			// timeout occurred while waiting for a packet
			Serial.println(F("[SX1276] Timeout!"));
		}
		else if (radio_status_code == RADIOLIB_ERR_CRC_MISMATCH) {
			// packet was received, but is malformed
			Serial.println(F("[SX1276] CRC error!"));
		}
		else {
				// some other error occurred
			Serial.print(F(" ~ Failed, code "));
			Serial.println(radio_status_code);
		}
	}
	
	return radio_status_code;
}


char* LoRaRadio::receiveUsingInterrupt(void) {
	// This method receives a message (using an interrupt)
	
	if (packet_received) {
		// Clear the packet_received immediately
		packet_received = false;

		if (in_receive_mode) {
			// Handle the received packet
			char* msg_ptr = handleReceivedPacket();
			
			return msg_ptr;
		} 
		else {
			handleEndOfTransmission();
		}
	}

	return nullptr;
}


char* LoRaRadio::handleReceivedPacket(void) {
  // Handle the received packet
    initializeRxMessageBuffer(); // Empty the message buffer so that the LoRa does not take the last message transmitted as a newly received message
	
    uint8_t num_bytes = radio.getPacketLength();
  	radio_return_code = radio.readData(rx_msg_buffer, num_bytes);

	if (radio_return_code == RADIOLIB_ERR_NONE) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.print(F(" ~ Received packet: "));
		}
		char* char_rx_msg_buffer = reinterpret_cast<char*>(rx_msg_buffer);
		
		if (VERBOSE_LORA_OUTPUT) {
			Serial.println(char_rx_msg_buffer);
		}
		
		// As a result return a ptr to the message
		return char_rx_msg_buffer;
		}
	else {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.print(F("Receive failed, code "));
			Serial.println(radio_return_code);
		}
		
		return nullptr;
	}
}


 void LoRaRadio::handleEndOfTransmission(void) {
	// Handle the end of transmission
	// This method cleans up the internal state and puts the LoRa module in standby
	radio.finishTransmit();

	radio.standby(); // This is being used to explicitly clear the internal radio buffer (does not do it all by itself
	packet_received = false;  // Hard flag set prevents ground-station from listening to itsself
	delay(COMPLETE_END_OF_TRANSMISSION_DELAY_IN_MS);  // delay to allow radio state to settle

	// Switch back to receive mode using interrupt-driven method

	radio.startReceive();
	in_receive_mode = true;

	return;
 }


void LoRaRadio::transmitResponse(void) {
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\nSwitching to Tx Mode to send response..."));
	}
	
	// The `startTransmit` method is non-blocking and uses an interrupt
	String data = "Acknowledged - ";// + String(count);
	int state = radio.startTransmit(data);
	//count++;
      
	if (state != RADIOLIB_ERR_NONE) {
		if (VERBOSE_LORA_OUTPUT) {
			Serial.print(F("Transmit failed, code "));
			Serial.println(state);
		}
	}

	in_receive_mode = false;
  
	return;
}


int16_t LoRaRadio::startReceive(void) {
	// This method puts of a message (using an interrupt)
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Starting Receive ..."));
	}
	
	radio_return_code = radio.startReceive();
	in_receive_mode = false; // Set the radio to Recieve Mode

	return radio_return_code;
}


/**
 * @brief Get LoRa radio RSSI
 * @details Returns the Received Signal Strength Indicator value with
 * optional verbose output for debugging purposes.
 * @return Current RSSI value
 */
float LoRaRadio::getRssi(void) {
	// This method gets the LoRa radio RSSI (Received Signal Strength Indicator)
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa RSSI Value ..."));
	}
	
	rssi = radio.getRSSI();
	
	if (VERBOSE_LORA_OUTPUT) {
		// print RSSI (Received Signal Strength Indicator)
		Serial.print(F("[SX1276] RSSI:\t\t"));
		Serial.print(rssi);
		Serial.println(F(" dBm"));
	}
	
	return rssi;
}


float LoRaRadio::getSnr(void) {
	// This method gets the LoRa radio Signal to Noise Ratio (SNR)
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa SNR Value ..."));
	}
	
	snr = radio.getSNR();
	
	if (VERBOSE_LORA_OUTPUT) {
		// print SNR (Signal-to-Noise Ratio)
		Serial.print(F("[SX1276] SNR:\t\t"));
		Serial.print(snr);
		Serial.println(F(" dB"));
	}
	
	return snr;
}


float LoRaRadio::getFrequencyError(void) {
	// This method gets the LoRa radio RSSI
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F("\n ~ Getting LoRa Frequency Error Value ..."));
	}
	
	frequency_error_in_hz = radio.getFrequencyError();
	
	if (VERBOSE_LORA_OUTPUT) {
		// print Frequency Error
		Serial.print(F("[SX1276] Frequency Error:\t\t"));
		Serial.print(frequency_error_in_hz);
		Serial.println(F(" Hz"));
	}
		
	return frequency_error_in_hz;
}


void LoRaRadio::initializeRxMessageBuffer(void) {
	// This method initializes the radio Rx message buffer

	memset(rx_msg_buffer, 0, sizeof(rx_msg_buffer));
	
	return;
}


char* LoRaRadio::getReceivedMsg(void) {
	// This method returns the pointer to the Rx message buffer where the Rx'd message can be retreived
	
	if (VERBOSE_LORA_OUTPUT) {
		Serial.println(F(" ~ In getReceivedMsg, Rx'd Message is: "));
		Serial.print(reinterpret_cast<char*>(rx_msg_buffer));
	}
	
	return reinterpret_cast<char*>(rx_msg_buffer);
}


/**
 * @brief Print LoRa radio information
 * @details Outputs comprehensive LoRa radio configuration including frequency,
 * bandwidth, spread factor, RSSI, and register information for debugging
 * and diagnostic purposes.
 */
void LoRaRadio::print(void) const {
	// This method prints the Lora Radio Wrapper class data members

	Serial.println(F("\n ~ Printing LoRa Data ..."));

	Serial.print(F("    Frequency is: "));
	Serial.print(frequency_in_hz);
	Serial.println(" Hz");
	Serial.print(F("    Bandwidth is: "));
	Serial.print(bandwidth_in_hz);
	Serial.println(" Hz");
	Serial.print(F("    Spread Factor is: "));
	Serial.println(spread_factor);
	Serial.print(F("    Coding Rate is: "));
	Serial.println(coding_rate);
	Serial.print(F("    Sync Word is: "));
	Serial.println(sync_word);	
	Serial.print(F("    Output Power is: "));
	Serial.print(output_power_in_dbm);
	Serial.println(" dBm");
	Serial.print(F("    Current Limit is: "));
	Serial.print(current_limit_in_ma);
	Serial.println(" mA");
	Serial.print(F("    Gain is: "));
	Serial.println(gain);	
	Serial.print(F("    RSSI is: "));
	Serial.print(rssi);
	Serial.println(" dBm");
	Serial.print(F("    SNR is: "));
	Serial.print(snr);
	Serial.println(" dB");
	Serial.print(F("    Frequency Error is: "));
	Serial.print(frequency_error_in_hz);
	Serial.println(" Hz");
	Serial.print(F("    Data Rate is: "));
	Serial.print(datarate);
	Serial.println(" bps");	
	Serial.print(F("    Transmission State is: "));
	Serial.println(radio_return_code);
	Serial.print(F("    Radio Status Code is: "));
	Serial.println(radio_status_code);

	Serial.println(F(""));
	
	return;
}