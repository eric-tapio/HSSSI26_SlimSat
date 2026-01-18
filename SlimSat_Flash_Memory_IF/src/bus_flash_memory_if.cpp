// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <bus_flash_memory_if.h>


//constructors
FlashMemoryIf::FlashMemoryIf(void) : flash(&flashTransport){

	address = 0;
	flash_register_value = 0;
    flash_return_result = 0;
	
	// Trying this for now
	// Automatically start the flash QSPI
	begin();
}


uint8_t FlashMemoryIf::flashEraseChip(void) {
	flash_return_result = flash.eraseChip();
	flash.waitUntilReady();
	
	// Initialize the DB Values
	initializeBusFlashMemoryDbValues();
	
	return flash_return_result;
}


uint8_t FlashMemoryIf::flashEraseSector(uint16_t sector_number) {
	
	if (0) {
        Serial.print(F(" ~ Erasing Flash Memory Sector Number: "));
		Serial.println(sector_number);
    }
	
  	flash_return_result = flash.eraseSector(sector_number);
	flash.waitUntilReady();
	
	if (0) {
        Serial.print(F(" ~ flash_return_result is: "));
		Serial.println(flash_return_result);
		if (flash_return_result) {
			Serial.println(F(" ~ Sector Erased!"));
		}
		else {
			Serial.println(F(" ~ Sector NOT Erased!"));
		}
    }
	return flash_return_result;
}
  

uint8_t FlashMemoryIf::flashEraseAllSectors(void) {
	
	for (uint8_t i=0; i<NUM_FLASH_BLOCKS; i++) {
		flash_return_result = flashEraseBlock(i);
	}
	
	return flash_return_result;
}

  
uint8_t FlashMemoryIf::flashEraseAllBlocks(void) {
	
	for (uint8_t i=0; i<NUM_FLASH_BLOCKS; i++) {
		flash_return_result = flashEraseBlock(i);
	}
	
	return flash_return_result;
}


uint8_t FlashMemoryIf::flashEraseBlock(uint32_t block_num) {
	
    if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
        Serial.print(F(" ~ Erasing Flash Memory block: "));
		Serial.println(block_num);
    }

    flash_return_result = flash.eraseBlock(block_num);
	flash.waitUntilReady(); // Wait for the erase to complete
	
    if (flash_return_result == SUCCESS_ERASE_VALUE) {
        if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
            Serial.println(F(" ~ Block erased."));
        }
    }
    else {
        if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
            Serial.println(F(" ~ Failed to erase block!"));
        }
    }
		
	return flash_return_result;
}


uint8_t FlashMemoryIf::begin(void) {
	// This method begins the flash SPI 
	flash_return_result = flash.begin();
	
	return flash_return_result;
}


void FlashMemoryIf::printJedecId(void) {
	// This method prints the flash Jedec ID 
	Serial.print(" ~ Flash chip OK! JEDEC ID: 0x");
	Serial.println(flash.getJEDECID(), HEX);

	return;
}


uint8_t FlashMemoryIf::getFlashReturnResult(void) const {
    return flash_return_result;
}


uint8_t FlashMemoryIf::initializeBusDataFlashMemoryDbValues(void) {
	// This method initializes the Bus Data Database values in Flash Memory

	flash_return_result = writeValueToFlashAddress(NUM_BUS_DATA_RECORDS_ADDRESS, 0);
	flash_return_result = writeValueToFlashAddress(NEXT_BUS_DATA_WRITE_TO_ADR_ADDRESS, INITIAL_BUS_DATA_START_WRITE_ADDRESS);
	
	return flash_return_result;
}


uint8_t FlashMemoryIf::initializePayloadDataFlashMemoryDbValues(void) {
	// This method initializes the Payload Data Database values in Flash Memory

	flash_return_result = writeValueToFlashAddress(NUM_PAYLOAD_DATA_RECORDS_ADDRESS, 0);
	flash_return_result = writeValueToFlashAddress(NEXT_PAYLOAD_DATA_WRITE_TO_ADR_ADDRESS, INITIAL_PAYLOAD_DATA_START_WRITE_ADDRESS);
	
	return flash_return_result;
}


uint8_t FlashMemoryIf::initializeBusFlashMemoryDbValues(void) {
	// This method initializes the Bus Database values in Flash Memory

	flash_return_result = writeValueToFlashAddress(BOOT_COUNT_ADDRESS, 0);
	flash_return_result = writeValueToFlashAddress(NUM_VALID_MSGS_RXD_ADDRESS, 0);
	flash_return_result = writeValueToFlashAddress(NUM_INVALID_MSGS_RXD_ADDRESS, 0);
	
	flash_return_result = initializeBusDataFlashMemoryDbValues();
	flash_return_result = initializePayloadDataFlashMemoryDbValues();
	 
	return flash_return_result;
}


void FlashMemoryIf::getBusDatabaseRegisterValues(BusDbRegisterValues& bus_db_reg_data) {
	// This method retrieves the Bus Database Register Values
	
	bus_db_reg_data.boot_count = getBootCount();
	bus_db_reg_data.num_valid_msgs = getValidMessageCount();
	bus_db_reg_data.num_invalid_msgs = getInvalidMessageCount();
	bus_db_reg_data.num_bus_data_recs = getNumberBusDataRecords();
	bus_db_reg_data.next_bus_data_write_to_adr = getNextBusDataWriteToAddress();
	bus_db_reg_data.num_payload_data_recs = getNumberPayloadDataRecords();
	bus_db_reg_data.next_payload_data_write_to_adr = getNextPayloadDataWriteToAddress();
	
	return;
}


uint32_t FlashMemoryIf::incrementAndReturnBootCount(void) {
	// This method first increments and the returns the boot count writen to flash memory
	incrementFlashBootCount();
	return getBootCount();
}


// Make these function names consistent with Flash in the name - TBD
uint32_t FlashMemoryIf::incrementAndReturnValidMessageCount(void) {
	// This method first increments and the returns the valid message count writen to flash memory
	incrementFlashValidMessageCount();
	return getValidMessageCount();
}


uint32_t FlashMemoryIf::incrementAndReturnInvalidMessageCount(void) {
	// This method first increments and the returns the invalid message count writen to flash memory
	incrementFlashInvalidMessageCount();
	return getInvalidMessageCount();
}


// Hmm, this function currently not being used!
uint32_t FlashMemoryIf::incrementAndReturnNumberBusDataRecRecords(void) {
	// This method first increments and the returns the number of Bus Data Records (count) writen to flash memory
	incrementFlashNumberBusDataRecordCount();
	
	uint32_t num_recs = getNumberBusDataRecords();
	
	if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
		Serial.print(F(" ~ Flash Memory Number of Bus Data Record Count is now: "));
		Serial.println(num_recs);
    }
	
	return num_recs;
}


uint32_t FlashMemoryIf::incrementAndReturnNumberPayloadDataRecords(void) {
	// This method first increments and the returns the number of Payload Data Records (count) writen to flash memory
	incrementFlashNumberPayloadDataRecordCount();
	
	uint32_t num_recs = getNumberPayloadDataRecords();
	
	if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
		Serial.print(F(" ~ Flash Memory Number of Payload Data Record Count is now: "));
		Serial.println(num_recs);
    }
	
	return num_recs;
}


uint32_t FlashMemoryIf::getBootCount(void) {
	// This method gets the boot count from Bus Flash Memory DB

	flash_register_value = read32(BOOT_COUNT_ADDRESS);

	//Serial.print(F(" ~ After the read, Flash Register Value is: "));
	//Serial.println(flash_register_value);	
	
	return flash_register_value;
}


uint32_t FlashMemoryIf::getValidMessageCount(void) {
	// This method gets the boot count from Bus Flash Memory DB

	flash_register_value = read32(NUM_VALID_MSGS_RXD_ADDRESS);

	return flash_register_value;
}


uint32_t FlashMemoryIf::getInvalidMessageCount(void) {
	// This method gets the boot count from Bus Flash Memory DB

	flash_register_value = read32(NUM_INVALID_MSGS_RXD_ADDRESS);

	return flash_register_value;
}


uint32_t FlashMemoryIf::getNumberBusDataRecords(void) {
	// This method gets the number of Bus Data Records written to Bus Flash Memory DB

	flash_register_value = read32(NUM_BUS_DATA_RECORDS_ADDRESS);

	return flash_register_value;
}


uint32_t FlashMemoryIf::getNumberPayloadDataRecords(void) {
	// This method gets the number of Payload Data Records written to Bus Flash Memory DB

	flash_register_value = read32(NUM_PAYLOAD_DATA_RECORDS_ADDRESS);

	return flash_register_value;
}


uint32_t FlashMemoryIf::getNextBusDataWriteToAddress(void) {
	// This method gets the next Bus Data Record write to address in Flash Memory DB

	flash_register_value = read32(NEXT_BUS_DATA_WRITE_TO_ADR_ADDRESS);

	return flash_register_value;
}


uint32_t FlashMemoryIf::getNextPayloadDataWriteToAddress(void) {
	// This method gets the next Payload Data Record write to address in Flash Memory DB

	flash_register_value = read32(NEXT_PAYLOAD_DATA_WRITE_TO_ADR_ADDRESS);

	return flash_register_value;
}


uint8_t FlashMemoryIf::incrementFlashBootCount(void) {
	// This method increments the current boot count in Flash Memory and writes the incremented value to Flash Memory DB
	
	uint8_t num_bytes_read_wrote = 0;
	
	num_bytes_read_wrote = incrementFlashCounterValue(BOOT_COUNT_SECTOR_NUMBER);
	
	return num_bytes_read_wrote;
}	


uint8_t FlashMemoryIf::incrementFlashValidMessageCount(void) {
	// This method increments the current valid message count in Flash Memory and writes the incremented value to Flash Memory DB
	return incrementFlashCounterValue(NUM_VALID_MSGS_RXD_SECTOR_NUMBER);
}	


uint8_t FlashMemoryIf::incrementFlashInvalidMessageCount(void) {
	// This method increments the current invalid message in Flash Memory and writes the incrementedvalue to Flash Memory DB
	return incrementFlashCounterValue(NUM_INVALID_MSGS_RXD_SECTOR_NUMBER);
}


uint8_t FlashMemoryIf::incrementFlashNumberBusDataRecordCount(void) {
	// This method increments the current number of Bus Data Records in Flash Memory and writes the incremented value to Flash Memory DB
	flash_return_result = incrementFlashCounterValue(NUM_BUS_DATA_RECORDS_SECTOR_NUMBER);
	return flash_return_result;
}	


uint8_t FlashMemoryIf::incrementFlashNumberPayloadDataRecordCount(void) {
	// This method increments the current number of Payload Data Records Records in Flash Memory and writes the incremented value to Flash Memory DB
	flash_return_result = incrementFlashCounterValue(NUM_PAYLOAD_DATA_RECORDS_SECTOR_NUMBER);	
	return flash_return_result;
}


uint8_t FlashMemoryIf::incrementFlashNextBusDataWriteToAddress(uint8_t num_bytes_to_incerment_by) {
	// This method increments the next Bus Data Write To Address in Flash Memory and writes the incremented value to Flash Memory DB
	return incrementFlashCounterValue(NEXT_BUS_DATA_WRITE_TO_ADDRESS_SECTOR_NUMBER, num_bytes_to_incerment_by);
}	


uint8_t FlashMemoryIf::incrementFlashNextPayloadDataWriteToAddress(uint8_t num_bytes_to_incerment_by) {
	// This method increments the next Payload Data Write To Address in Flash Memory and writes the incremented value to Flash Memory DB
	return incrementFlashCounterValue(NEXT_PAYLOAD_DATA_WRITE_TO_ADDRESS_SECTOR_NUMBER, num_bytes_to_incerment_by);
}	


uint8_t FlashMemoryIf::validatePayloadDataStartWriteToAdress(PlDataRec& pl_data) {
	// This method validates the Payload Start Write to Address before writing a new record
	
	// Get the Payload Data Write to address
	address = getNextPayloadDataWriteToAddress();
	uint8_t rec_size = pl_data.getRecordSize();
	uint8_t num_recs = getNumberPayloadDataRecords();
	uint32_t adress_diff_in_bytes = address - (INITIAL_PAYLOAD_DATA_START_WRITE_ADDRESS);
	
	// Get the remainder from adress_diff_in_bytes divided by the rec size
	uint8_t diff_remainder_in_bytes = adress_diff_in_bytes % rec_size;
	
	if (diff_remainder_in_bytes > 0) {
		// Then the record is incomplete
		// Make the number of recs consistent with the start address
		uint8_t backed_out_num_recs = adress_diff_in_bytes / rec_size;
		
		if (backed_out_num_recs == num_recs) {
			// Then increment the Rec Counter by 1 and use it to compute/update the write to address
			flash_return_result = incrementFlashCounterValue(NUM_PAYLOAD_DATA_RECORDS_SECTOR_NUMBER);
			
			// This is not yet complete - TBD
			uint32_t desired_address = (num_recs+1) * rec_size + INITIAL_PAYLOAD_DATA_START_WRITE_ADDRESS;
			
			adress_diff_in_bytes = desired_address - address;
			
			// Increment the payload address by the diff in bytes
			incrementFlashNextPayloadDataWriteToAddress(adress_diff_in_bytes);
			
			if (0) {
				Serial.print(F(" ~ A Partial Payload Record Encountered. increment Needed of: "));
				Serial.println(adress_diff_in_bytes);
			}
		}
		else {
			// Do Nothing
			adress_diff_in_bytes = 0;
			
			if (0) {
				Serial.println(F(" ~ The Payload Start Write Address has been validated!  Complete Record Encountered. No increment needed."));
			}
		}
	}
	
	return (uint8_t)adress_diff_in_bytes;
}


uint8_t FlashMemoryIf::validateBusDataStartWriteToAdress(BusDataRec& bus_data) {
	// This method validates the Bus Start Write to Address before writing a new record
	
	// Get the Bus Data Write to address
	address = getNextBusDataWriteToAddress();
	uint8_t rec_size = bus_data.getRecordSize();
	uint8_t num_recs = getNumberBusDataRecords();
	uint32_t adress_diff_in_bytes = address - (INITIAL_BUS_DATA_START_WRITE_ADDRESS);
	
	// Get the remainder from adress_diff_in_bytes divided by the rec size
	uint8_t diff_remainder_in_bytes = adress_diff_in_bytes % rec_size;
	
	if (diff_remainder_in_bytes > 0) {
		// Then the record is incomplete
		// Make the number of recs consistent with the start address
		uint8_t backed_out_num_recs = adress_diff_in_bytes / rec_size;
		
		if (backed_out_num_recs == num_recs) {
			// Then increment the Rec Counter by 1 and use it to compute/update the write to address
			flash_return_result = incrementFlashCounterValue(NUM_BUS_DATA_RECORDS_SECTOR_NUMBER);
			
			// This is not yet complete - TBD
			uint32_t desired_address = (num_recs+1) * rec_size + INITIAL_BUS_DATA_START_WRITE_ADDRESS;
			
			adress_diff_in_bytes = desired_address - address;
			
			// Increment the Bus address by the diff in bytes
			incrementFlashNextBusDataWriteToAddress(adress_diff_in_bytes);
			
			Serial.print(F(" ~ A Partial Bus Record Encountered. increment Needed of: "));
			Serial.println(adress_diff_in_bytes);
			
		}
		else {
			// Do Nothing
			adress_diff_in_bytes = 0;
			if (0) {
				Serial.println(F(" ~ The Bus Start Write Address has been validated!  Complete Record Encountered. No increment needed."));
			}
		}
	}
	
	
	return (uint8_t)adress_diff_in_bytes;
}


uint8_t FlashMemoryIf::incrementFlashCounterValue(uint16_t sector_number, uint8_t increment_value) {
	// This method increments the specified counter in Flash Memory and writes the incremented counter value to Flash Memory DB

	uint32_t read_write_value = read32(sector_number*FLASH_SECTOR_SIZE_IN_BYTES);

	if (0) {
		Serial.print(F("\n ~ Read Flash Counter Value at address "));
		Serial.print(sector_number*FLASH_SECTOR_SIZE_IN_BYTES);
		Serial.print(F(" is: "));
		Serial.println(read_write_value);
	}
	
	// Increment the count
	read_write_value += increment_value;
	
	// Before writing the value, erase the sector
	flash_return_result = flashEraseSector(sector_number);
	
	flash_return_result = writeValueToFlashAddress(sector_number*FLASH_SECTOR_SIZE_IN_BYTES, read_write_value);
	
	if (0) {
		Serial.print(F(" ~ Written Flash Counter Value is: "));
		Serial.println(read_write_value);
	}

	// For debug purposes, read the value written
	read_write_value = read32(sector_number*FLASH_SECTOR_SIZE_IN_BYTES);
	
	if (0) {
		Serial.print(F("\n --> For Debug Read Flash Counter Value at address "));
		Serial.print(sector_number*FLASH_SECTOR_SIZE_IN_BYTES);
		Serial.print(F(" is: "));
		Serial.println(read_write_value);
	}

	return flash_return_result;
}


uint32_t FlashMemoryIf::read32(uint32_t addr) {
	
	uint32_t read_val = 0;
	
	read_val = flash.read32(addr);
	
	if (0) {
		Serial.print(F(" --> Read32 Read Value is: "));
		Serial.println(read_val);
	}
	
	return read_val;
}


void FlashMemoryIf::writeBusDataRecordToFlash(BusDataRec& bus_data) {
	// This method writes a Bus Data Record to Flash Memory
	
	if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
		Serial.println(F(" ~ Writing Bus Data Record to Flash Memory ..."));
	}
	
	validateBusDataStartWriteToAdress(bus_data);
	
	// Get the Bus Data Write to address
	address = getNextBusDataWriteToAddress();
	
	// Assign the rec number
	// The number of data records will formally get incremented once all data has been written to flash
	bus_data.bus_rec_number = 1 + getNumberBusDataRecords();
	
	if (0) {
		Serial.print(F(" --> In writeBusDataRecordToFlash, writing bus_data.bus_rec_number is: "));
		Serial.println(bus_data.bus_rec_number);
		Serial.print(F("     Starting Bus Data Write to address is: "));
		Serial.println(address);
		Serial.print(F("     Bus Data Record to write is: "));
		bus_data.printRecord();
	}
	
	// Ugh! Must remember we're dealing with flash, which must be erased before being updated, and writeValueToFlashAddress only writes. It assumes that the memory has already been initialized
	
	uint8_t num_bytes_written = 0;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.time);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.bus_rec_number);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.temp_C);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.thermistor_V);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.shunt_voltage_mV_1);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.bus_voltage_V_1);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.load_voltage_V_1);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.current_mA_1);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.shunt_voltage_mV_2);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.bus_voltage_V_2);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.load_voltage_V_2);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;

	num_bytes_written = writeValueToFlashAddress(address, bus_data.current_mA_2);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.shunt_voltage_mV_3);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.bus_voltage_V_3);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.load_voltage_V_3);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.current_mA_3);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	
	//incrementFlashNumberBusDataRecordCount();
	// For debug purposes, use this one to get visibility into the count
	incrementAndReturnNumberBusDataRecRecords();
	//}
	
	return;
}


void FlashMemoryIf::readBusDataRecordFromFlash(uint32_t address, BusDataRec& bus_data) {
	// This method reads a Bus Data Record from Flash Memory
	
	if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
		Serial.println(F("\n ~ Reading Bus Data Record From Flash Memory ..."));
	}
	
	// Need to initialize or specify the address to read from, since can be any record, not just the next record
	flash_return_result = readValueFromFlashAddress(address, &bus_data.time);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.bus_rec_number);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.temp_C);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.thermistor_V);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.shunt_voltage_mV_1);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.bus_voltage_V_1);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.load_voltage_V_1);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.current_mA_1);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.shunt_voltage_mV_2);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.bus_voltage_V_2);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.load_voltage_V_2);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.current_mA_2);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.shunt_voltage_mV_3);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.bus_voltage_V_3);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.load_voltage_V_3);
	address += flash_return_result;
	flash_return_result = readValueFromFlashAddress(address, &bus_data.current_mA_3);
	address += flash_return_result;
		
	return;
}


void FlashMemoryIf::writePartialBusDataRecordToFlash(BusDataRec& bus_data) {
	// This method writes a partial payload record to flash and is for testing/verification purposes only

	validateBusDataStartWriteToAdress(bus_data);
	
	uint8_t num_bytes_written = 0;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.time);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.bus_rec_number);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.temp_C);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.thermistor_V);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.shunt_voltage_mV_1);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.bus_voltage_V_1);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.load_voltage_V_1);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	num_bytes_written = writeValueToFlashAddress(address, bus_data.current_mA_1);
	incrementFlashNextBusDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;

	// No other data is written
	
	return;
}


void FlashMemoryIf::writePartialPayloadDataRecordToFlash(PlDataRec& pl_data) {
	// This method writes a partial payload record to flash and is for testing/verification purposes only

	validatePayloadDataStartWriteToAdress(pl_data);
	
	// This method writes a Payload Data Record to Flash Memory
	uint8_t num_bytes_written = 0;
	
	//if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
	if (1) {
		Serial.println(F(" ~ Writing Partial Payload Data Record to Flash Memory ..."));
	}
	// uint8_t num_bytes_read_wrote = 0;
	
	// Get the Payload Data Write to address
	address = getNextPayloadDataWriteToAddress();
		
	// Assign the payload rec number
	// The number of payload data records will formally get incremented once all data has been written to flash
	pl_data.pl_rec_number = 1 + getNumberPayloadDataRecords();
	
	// Write the payload record time
	num_bytes_written = writeValueToFlashAddress(address, pl_data.time);
	incrementFlashNextPayloadDataWriteToAddress(num_bytes_written);
	
	if (0) {
		Serial.print(F("\n ~ num_bytes_written is: "));
		Serial.println(num_bytes_written);
	}
	
	address += num_bytes_written;
	
	if (0) {
		Serial.print(F("\n ~ Writing Payload Record to Flash Starting Address is now: "));
		Serial.println(address);
	}
	
	// Write the payload rec number
	num_bytes_written = writeValueToFlashAddress(address, pl_data.pl_rec_number);
	incrementFlashNextPayloadDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	if (0) {
		Serial.print(F("\n ~ pl_rec_number is: "));
		Serial.println(pl_data.pl_rec_number);
	}
	
	// No Data array is written
	
	return;
}


void FlashMemoryIf::writePayloadDataRecordToFlash(PlDataRec& pl_data) {
	
	validatePayloadDataStartWriteToAdress(pl_data);
	
	// This method writes a Payload Data Record to Flash Memory
	uint8_t num_bytes_written = 0;
	
	//if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
	if (0) {
		Serial.println(F(" ~ Writing Payload Data Record to Flash Memory using Reference Function..."));
	}
	
	// Get the Payload Data Write to address
	address = getNextPayloadDataWriteToAddress();

	if (0) {	
		Serial.print(F("\n ~ Writing Payload Record to Flash: Starting Address is: "));
		Serial.println(address);
	}
	
	// Assign the payload rec number
	// The number of payload data records will formally get incremented once all data has been written to flash
	pl_data.pl_rec_number = 1 + getNumberPayloadDataRecords();
	
	// Write the payload record time
	num_bytes_written = writeValueToFlashAddress(address, pl_data.time);
	incrementFlashNextPayloadDataWriteToAddress(num_bytes_written);
	
	if (0) {
		Serial.print(F("\n ~ num_bytes_written is: "));
		Serial.println(num_bytes_written);
	}
	
	address += num_bytes_written;
	
	if (0) {
		Serial.print(F("\n ~ Writing Payload Record to Flash Starting Address is now: "));
		Serial.println(address);
	}
	
	// Write the payload rec number
	num_bytes_written = writeValueToFlashAddress(address, pl_data.pl_rec_number);
	incrementFlashNextPayloadDataWriteToAddress(num_bytes_written);
	address += num_bytes_written;
	
	if (0) {
		Serial.print(F("\n ~ pl_rec_number is: "));
		Serial.println(pl_data.pl_rec_number);
	}
	
	// Write the data array
	for (uint8_t i=0; i<PAYLOAD_DATA_ARY_SIZE; i++) {
		if (0) {
			Serial.print(F("\n ~ Writing payload data array element: "));
			Serial.print(i);
			Serial.print(F(" = "));
			Serial.print(pl_data.getArrayElement(i));
			Serial.print(F(" to Address: "));
			Serial.println(address);
		}
		num_bytes_written = writeValueToFlashAddress(address, pl_data.getArrayElement(i));
		incrementFlashNextPayloadDataWriteToAddress(num_bytes_written);
		address += num_bytes_written;
	}
	
	// For debug purposes, use this one to get visibility into the count
	incrementAndReturnNumberPayloadDataRecords();
	
	address = getNextPayloadDataWriteToAddress();
	
	if (0) {
		Serial.print(F("\n ~ After Writing Payload Record to Flash: Read Next PL Write to Address is: "));
		Serial.println(address);
	}
	
	return;
}


 void FlashMemoryIf::readPayloadDataRecordFromFlash(uint32_t address, PlDataRec& pl_data) {
	// This method reads a Payload Data Record from Flash Memory
	
	double ary_value = 0.0;
	
	if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
		Serial.println(F("\n ~ Reading Payload Data Record From Flash Memory ..."));
	}
	
	flash_return_result = readValueFromFlashAddress(address, &pl_data.time);
	address += flash_return_result;
	
	flash_return_result = readValueFromFlashAddress(address, &pl_data.pl_rec_number);
	address += flash_return_result;
	
	if (0) {
		Serial.print(F("\n ~ pl_rec_number is: "));
		Serial.println(pl_data.pl_rec_number);
	}
	
	for (uint8_t i=0; i<PAYLOAD_DATA_ARY_SIZE; i++) {
		flash_return_result = readValueFromFlashAddress(address, &ary_value);
		address += flash_return_result;
		
		// Assign the array value
		pl_data.setArrayElement(i, ary_value);
		
		if (0) {
			Serial.print(F("\n ~ ary_value: "));
			Serial.println(ary_value);
		}
	}
		
	return;
}


uint8_t FlashMemoryIf::flashEraseAllBusDataRecordBlocks(void) {
	// This method erases all the bus Data Flash Memory blocks
	
	if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
		Serial.println(F(" ~ Erasing all Bus Data Flash Memory Blocks ..."));
	}
	
	for (uint8_t i=BUS_DATA_START_WRITE_BLOCK_NUMBER; i<PAYLOAD_DATA_START_WRITE_BLOCK_NUMBER; i++) {
		flash_return_result = flashEraseBlock(i);
	}
	
	// Then reinitialize the Bus Data DB Values
	initializeBusDataFlashMemoryDbValues();
	
	return flash_return_result;
}


uint8_t FlashMemoryIf::flashEraseAllPayloadDataRecordBlocks(void) {
	// This method erases all the Payload Data Flash Memory blocks
	
	if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
		Serial.println(F(" ~ Erasing all Payload Data Flash Memory Blocks ..."));
	}
	
	for (uint8_t i=PAYLOAD_DATA_START_WRITE_BLOCK_NUMBER; i<NUM_FLASH_BLOCKS; i++) {
		flash_return_result = flashEraseBlock(i);
	}
	
	// Then reinitialize the Payload Data DB Values
	initializePayloadDataFlashMemoryDbValues();
	
	return flash_return_result;	
}


//uint8_t FlashMemoryIf::getPayloadDataRecordFromFlash(uint16_t pl_data_rec_num, PlDataRec& pl_data) {
uint8_t FlashMemoryIf::getPayloadDataRecordFromFlash(int16_t pl_data_rec_num, PlDataRec& pl_data) {
	// This method gets the specified Payload Data struct from Flash memory
	
	uint8_t pl_data_rec_size_in_bytes = pl_data.getRecordSize();
	
	// Compute the address for the data rec
	// -1 since the rec num is not zero based
	uint32_t address = (pl_data_rec_num-1) * pl_data_rec_size_in_bytes + INITIAL_PAYLOAD_DATA_START_WRITE_ADDRESS;

	// Validate the input
	// Get the current number of payload data records
	uint16_t current_num_recs = getNumberPayloadDataRecords();
	
	if (0) {
		Serial.print(F("\n -> pl_data_rec_size_in_bytes is: "));
		Serial.println(pl_data_rec_size_in_bytes);	
		Serial.print(F(" -> address is: "));
		Serial.println(address);	
		Serial.print(F(" -> current_num_recs is: "));
		Serial.println(current_num_recs);
		Serial.println();
	}

	if (pl_data_rec_num <= current_num_recs) {
		// Then get the data record
		readPayloadDataRecordFromFlash(address, pl_data);
		return 1;
	}
	else {
		// The data record input is currently invalid
		return 0;
	}
}


uint8_t FlashMemoryIf::getBusDataRecordFromFlash(uint16_t bus_data_rec_num, BusDataRec& bus_data) {
	// This method gets the specified Bus Data record from Flash memory
	
	if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
		Serial.println(F("\n ~ Getting Bus Data Record from Flash ..."));
	}
	
	// Compute the address for the data rec
	// -1 since the rec num is not zero based
	uint32_t address = (bus_data_rec_num-1) * bus_data.getRecordSize() + INITIAL_BUS_DATA_START_WRITE_ADDRESS;

	// Validate the input
	// Get the current number of payload data records
	uint16_t current_num_recs = getNumberBusDataRecords();
	
	Serial.print(F(" -> bus_data_rec_num is: "));
	Serial.println(bus_data_rec_num);
	Serial.print(F(" -> current_num_recs is: "));
	Serial.println(current_num_recs);
		
	if (bus_data_rec_num <= current_num_recs) {
		// Then get the data record
		readBusDataRecordFromFlash(address, bus_data);
		return 1;
	}
	else {
		// The data struct input is currently invalid
		return 0;
	}
}