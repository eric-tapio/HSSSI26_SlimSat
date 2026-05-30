//
//  flash_memory_if.h
//  Flash_Memory
//
// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef BUS_FLASH_MEMORY_IF_HEADER
#define BUS_FLASH_MEMORY_IF_HEADER

#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include <SPI.h>
#include <Adafruit_SPIFlash.h>
#include "SdFat_Adafruit_Fork.h"
#include <bus_data.h>
#include <bus_db_register_struct.h>
#include <payload_data.h>

// Not used though helpful to know
//#define FLASH_MEMORY_SIZE_BYTES  (512 * 4096) = 2,097,152 bytes = 2MB Flash Memory Size
//#define NUM_FLASH_SECTORS 512
//#define FLASH_PAGE_SIZE 256
//#define NUM_FLASH_PAGES 8192

#define BUS_DATA_START_WRITE_BLOCK_NUMBER 1
#define BUS_DATA_STOP_WRITE_BLOCK_NUMBER 15
#define PAYLOAD_DATA_START_WRITE_BLOCK_NUMBER 16
#define PAYLOAD_DATA_STOP_WRITE_BLOCK_NUMBER 31

#define BUS_DATA_START_WRITE_SECTOR_NUMBER 16
#define PAYLOAD_DATA_START_WRITE_SECTOR_NUMBER 256
#define BOOT_COUNT_SECTOR_NUMBER 0
#define NUM_VALID_MSGS_RXD_SECTOR_NUMBER 1
#define NUM_INVALID_MSGS_RXD_SECTOR_NUMBER 2
#define NUM_BUS_DATA_RECORDS_SECTOR_NUMBER 3
#define NEXT_BUS_DATA_WRITE_TO_ADDRESS_SECTOR_NUMBER 4
#define NUM_PAYLOAD_DATA_RECORDS_SECTOR_NUMBER 5
#define NEXT_PAYLOAD_DATA_WRITE_TO_ADDRESS_SECTOR_NUMBER 6

#define NUM_FLASH_BLOCKS 32
#define FLASH_SECTOR_SIZE_IN_BYTES 4096
#define SUCCESS_ERASE_VALUE 1

#define BOOT_COUNT_ADDRESS BOOT_COUNT_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES
#define NUM_VALID_MSGS_RXD_ADDRESS NUM_VALID_MSGS_RXD_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES
#define NUM_INVALID_MSGS_RXD_ADDRESS NUM_INVALID_MSGS_RXD_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES
#define NUM_BUS_DATA_RECORDS_ADDRESS NUM_BUS_DATA_RECORDS_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES
#define NEXT_BUS_DATA_WRITE_TO_ADR_ADDRESS NEXT_BUS_DATA_WRITE_TO_ADDRESS_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES
#define NUM_PAYLOAD_DATA_RECORDS_ADDRESS NUM_PAYLOAD_DATA_RECORDS_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES
#define NEXT_PAYLOAD_DATA_WRITE_TO_ADR_ADDRESS NEXT_PAYLOAD_DATA_WRITE_TO_ADDRESS_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES

#define INITIAL_BUS_DATA_START_WRITE_ADDRESS BUS_DATA_START_WRITE_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES
#define INITIAL_PAYLOAD_DATA_START_WRITE_ADDRESS PAYLOAD_DATA_START_WRITE_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES

#define VERBOSE_BUS_FLASH_MEMORY_OUTPUT 0


class FlashMemoryIf {
private:

	// Data Members
	Adafruit_FlashTransport_QSPI flashTransport;
	Adafruit_SPIFlash flash;
    
	uint32_t address;
	uint8_t flash_return_result;
	uint32_t flash_register_value;

    // Methods

public:
    // Constructors
    FlashMemoryIf(void);
  
	uint8_t begin(void);
	void printJedecId(void);
	
    // Methods
	// Low-level Flash IO methods
	  
	template <typename T>
	uint32_t writeValueToFlashAddress(uint32_t address, T value_to_write)  {
		uint32_t return_val = flash.writeBuffer(address, (uint8_t*)&value_to_write, sizeof(T));	
		flash.waitUntilReady(); // Wait for the write operation to complete
		
        if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
            Serial.print(F("\n - Writing value to specified Flash address: "));
			Serial.println(address);
            Serial.print(F(" - Value to write is: "));
			Serial.println(*(&value_to_write));
            Serial.print(F(" - Size of T is: "));
			Serial.println(sizeof(T));
        }
		
        if (return_val) {
            if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
				Serial.print(F(" - Value wrote is: "));
				Serial.println(*(&value_to_write));
				Serial.print(F(" to specified address: "));
				Serial.println(address);
            }
        }
        else {
            if (1) {
                Serial.println(F(" - Failed to write to flash!"));
            }
        }

        return return_val;
    }
  
	
	template <typename T>
	uint32_t readValueFromFlashAddress(uint32_t address, T* read_value_ptr) {
		uint32_t return_val = flash.readBuffer(address, (uint8_t*)read_value_ptr, sizeof(T));

		if (return_val) {
            if (VERBOSE_BUS_FLASH_MEMORY_OUTPUT) {
				Serial.print(F(" - Size of T is: "));
				Serial.println(sizeof(T));
				Serial.print(" - Read value from flash: ");
				Serial.println(*(read_value_ptr));
				Serial.print(F(" - From specified address: "));
				Serial.println(address);
            }
        }
		else {
            if (1) {
				Serial.println(" - Failed to read from flash!");
				Serial.print(F("     From specified address: "));
				Serial.println(address);
			}
		}

        return return_val;
    }
    
	
	// High-level Flash IO methods
	uint8_t flashEraseChip(void);
	uint8_t flashEraseAllSectors(void);
	uint8_t flashEraseSector(uint16_t sector_number);
	uint8_t flashEraseAllBlocks(void);
    uint8_t flashEraseBlock(uint32_t block_num);
	uint8_t getFlashReturnResult(void) const;
	
	uint32_t incrementAndReturnBootCount(void);
	uint32_t incrementAndReturnValidMessageCount(void);
	uint32_t incrementAndReturnInvalidMessageCount(void);
	uint32_t incrementAndReturnNumberBusDataRecRecords(void);
	uint32_t incrementAndReturnNumberPayloadDataRecords(void);
	uint8_t initializeBusFlashMemoryDbValues(void);
	uint32_t getBootCount(void);
	uint32_t getValidMessageCount(void);
	uint32_t getInvalidMessageCount(void);
	uint32_t getNextPayloadDataWriteToAddress(void);
	uint32_t getNextBusDataWriteToAddress(void);
	uint32_t getNumberPayloadDataRecords(void);
	uint32_t getNumberBusDataRecords(void);
	uint8_t incrementFlashBootCount(void);
	uint8_t incrementFlashValidMessageCount(void);
	uint8_t incrementFlashInvalidMessageCount(void);
	uint8_t incrementFlashNumberBusDataRecordCount(void);
	uint8_t incrementFlashNumberPayloadDataRecordCount(void);
	uint8_t incrementFlashCounterValue(uint16_t sector_number, uint8_t increment_value=1);
	void writeBusDataRecordToFlash(BusDataRec& bus_data);
	void readBusDataRecordFromFlash(uint32_t address, BusDataRec& bus_data);
	void getBusDatabaseRegisterValues(BusDbRegisterValues& bus_db_reg_data);
	void writePayloadDataRecordToFlash(PlDataRec& pl_data);
	void readPayloadDataRecordFromFlash(uint32_t address, PlDataRec& pl_data);
	uint8_t flashEraseAllBusDataRecordBlocks(void);
	uint8_t flashEraseAllPayloadDataRecordBlocks(void);
	uint8_t getBusDataRecordFromFlash(uint16_t bus_data_rec_num, BusDataRec& bus_data);
	uint8_t getPayloadDataRecordSize(void);
	uint8_t getPayloadDataRecordFromFlash(int16_t pl_data_rec_num, PlDataRec& pl_data);
	uint8_t initializePayloadDataFlashMemoryDbValues(void);
	uint8_t initializeBusDataFlashMemoryDbValues(void);
	uint8_t validatePayloadDataStartWriteToAdress(PlDataRec& pl_data);
	uint8_t incrementFlashNextBusDataWriteToAddress(uint8_t num_bytes_to_incerment_by);
	uint8_t incrementFlashNextPayloadDataWriteToAddress(uint8_t num_bytes_to_incerment_by);
	uint8_t validateBusDataStartWriteToAdress(BusDataRec& bus_data);
	uint32_t read32(uint32_t addr);
	
	// These methods are used only for testing and verification and will be removed
	void writePartialPayloadDataRecordToFlash(PlDataRec& pl_data);
	void writePartialBusDataRecordToFlash(BusDataRec& bus_data);
};

#endif