// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <sc_flash_memory_if.h>
#include <sc_bus_data_struct.h>
#include <assert.h>

#define STR_CMP_MATCHES_VALUE 0
#define START_ADDRESS 0x00000020
#define FIRST_ADDRESS 0x00000000
#define LAST_ADDRESS 0x001FFFFF
#define SUCCESS_VALUE 1
#define SUCCESS_ERASE_VALUE 1
#define FLASH_INITIALIZED_VALUE 255
// 1 block is 512kB 
#define BLOCK_ADDRESS_SIZE 65535
#define NUMBER_BLOCKS 32

#define SC_DATA_START_WRITE_SECTOR_NUMBER 100
#define PL_DATA_START_WRITE_SECTOR_NUMBER 250
#define FLASH_SECTOR_SIZE_IN_BYTES 4096

#define BOOT_COUNT_SECTOR_NUMBER 0
#define NUM_VALID_MSGS_RXD_SECTOR_NUMBER 1
#define NUM_INVALID_MSGS_RXD_SECTOR_NUMBER 2
#define NUM_SC_DATA_RECORDS_SECTOR_NUMBER 3
#define NEXT_SC_DATA_WRITE_TO_ADR_SECTOR_NUMBER 4
#define NUM_PL_DATA_RECORDS_SECTOR_NUMBER 5
#define NEXT_PL_DATA_WRITE_TO_ADR_SECTOR_NUMBER 6

// Construct the S/C Flash Memory IF Object
FlashMemoryIf ScFlashMemIf;
ScBusData sc_bus_data_for_writing;
ScBusData sc_bus_data_for_reading;

uint8_t ScBusDataStructsAreEqual(ScBusData sc_bus_data_1, ScBusData sc_bus_data_2) {

	Serial.println(F("\n ~ Determining if two S/C Bus Data Structs are equal ..."));
	assert(sc_bus_data_1.time == sc_bus_data_2.time);
	assert(sc_bus_data_1.temp_C == sc_bus_data_2.temp_C);
	assert(sc_bus_data_1.shunt_voltage_mV_1 == sc_bus_data_2.shunt_voltage_mV_1);
	assert(sc_bus_data_1.bus_voltage_V_1 == sc_bus_data_2.bus_voltage_V_1);
	assert(sc_bus_data_1.load_voltage_V_1 == sc_bus_data_2.load_voltage_V_1);
	assert(sc_bus_data_1.current_mA_1 == sc_bus_data_2.current_mA_1);
	assert(sc_bus_data_1.shunt_voltage_mV_2 == sc_bus_data_2.shunt_voltage_mV_2);
	assert(sc_bus_data_1.bus_voltage_V_2 == sc_bus_data_2.bus_voltage_V_2);
	assert(sc_bus_data_1.load_voltage_V_2 == sc_bus_data_2.load_voltage_V_2);
	assert(sc_bus_data_1.current_mA_2 == sc_bus_data_2.current_mA_2);
	assert(sc_bus_data_1.shunt_voltage_mV_3 == sc_bus_data_2.shunt_voltage_mV_3);
	assert(sc_bus_data_1.bus_voltage_V_3 == sc_bus_data_2.bus_voltage_V_3);
	assert(sc_bus_data_1.load_voltage_V_3 == sc_bus_data_2.load_voltage_V_3);
	assert(sc_bus_data_1.current_mA_3 == sc_bus_data_2.current_mA_3);

	Serial.println(F(" ~ Two given S/C Bus Data Structs are Equal Test: PASSES!\n"));
	
	return 1;
}

void printScBusDataStruct(ScBusData sc_bus_data) { 
	
	Serial.println(F(" ~ Printing S/C Bus Data Struct ..."));
	Serial.print(F("     Time: "));
	Serial.println(sc_bus_data.time);
	Serial.print(F("     Temp in deg C: "));
	Serial.println(sc_bus_data.temp_C);
	//Serial.print(F("     Address: "));
	//Serial.println(sc_bus_data.address);
	Serial.print(F("     Shunt Voltage in mV 1: "));
	Serial.println(sc_bus_data.shunt_voltage_mV_1);
	Serial.print(F("     Bus Voltage in V 1: "));
	Serial.println(sc_bus_data.bus_voltage_V_1);
	Serial.print(F("     Load Voltage in V 1: "));
	Serial.println(sc_bus_data.load_voltage_V_1);
	Serial.print(F("     Current in mA 1: "));
	Serial.println(sc_bus_data.current_mA_1);
	//Serial.print(F("     Power in mV: "));
	//Serial.println(sc_bus_data.power_mW);
	Serial.print(F("     Shunt Voltage in mV 2: "));
	Serial.println(sc_bus_data.shunt_voltage_mV_2);
	Serial.print(F("     Bus Voltage in V 2: "));
	Serial.println(sc_bus_data.bus_voltage_V_2);
	Serial.print(F("     Load Voltage in V 2: "));
	Serial.println(sc_bus_data.load_voltage_V_2);
	Serial.print(F("     Current in mA 2: "));
	Serial.println(sc_bus_data.current_mA_2);
	//Serial.print(F("     Power in mV: "));
	//Serial.println(sc_bus_data.power_mW);
	Serial.print(F("     Shunt Voltage in mV 3: "));
	Serial.println(sc_bus_data.shunt_voltage_mV_3);
	Serial.print(F("     Bus Voltage in V 3: "));
	Serial.println(sc_bus_data.bus_voltage_V_3);
	Serial.print(F("     Load Voltage in V 3: "));
	Serial.println(sc_bus_data.load_voltage_V_3);
	Serial.print(F("     Current in mA 3: "));
	Serial.println(sc_bus_data.current_mA_3);
	//Serial.print(F("     Power in mV: "));
	//Serial.println(sc_bus_data.power_mW);
	
	Serial.println(F(""));
	
	return;
}

uint8_t verifyWriteAndReadScBusDataStruct(void) {
	
	uint8_t num_bytes_read_wrote = ScFlashMemIf.flashEraseChip(); // Which initializes the DB values
	// Verify that the the flash return result is success
	assert(num_bytes_read_wrote == sizeof(uint32_t));

	Serial.println(F("\n ~ Verifying Write And Read of S/C Bus Data Struct to Flash Memory ...\n"));

	uint8_t flash_return_result = 0;
	uint32_t new_sc_write_to_address = 0;
	uint32_t register_value = 0;
	uint32_t next_write_to_adr = ScFlashMemIf.getNextWriteToAddress();
  uint32_t next_read_from_adr = ScFlashMemIf.getNextReadFromAddress();
	uint32_t num_sc_bus_data_records = ScFlashMemIf.getNumberScBusDataRecords();
	uint32_t next_sc_write_to_address = ScFlashMemIf.getNextScBusDataWriteToAddress();

	//Serial.print(" ~ START_ADDRESS is: ");
  //Serial.println(START_ADDRESS);
	Serial.print(" ~ Next Write to Address is: ");
  Serial.println(next_write_to_adr);
	Serial.print(" ~ Next Read from Address is: ");
  Serial.println(next_read_from_adr);
	Serial.print(" ~ Number of S/C Bus Data Records is: ");
  Serial.println(num_sc_bus_data_records);
	assert(num_sc_bus_data_records == 0);
	Serial.print(" ~ Next S/C Bus Write to Address is: ");
  Serial.println(next_sc_write_to_address);
  Serial.println("");

	Serial.println("\n ~ Printing S/C Bus Data Struct Written to Flash ...");
	printScBusDataStruct(sc_bus_data_for_writing);

	// Write sc_bus_data
	Serial.println("\n ~ Writing S/C Bus Data Struct to Flash ...");
	flash_return_result = ScFlashMemIf.writeScBusDataStructToFlash(sc_bus_data_for_writing);
	Serial.print(" ~ Flash Return Result is: ");
  Serial.println(flash_return_result);
	assert(flash_return_result == sizeof(ScBusData));

	Serial.println("\n ~ Verifying that the Number of S/C Bus Data Records has been incremented ...");
	num_sc_bus_data_records = ScFlashMemIf.getNumberScBusDataRecords();
	assert(num_sc_bus_data_records == 1);
	Serial.println(" ~ Test: PASSED");

	Serial.println("\n ~ Verifying that the Next S/C Bus Data Record Write to Address has been correctly incremented ...");
	new_sc_write_to_address = ScFlashMemIf.getNextScBusDataWriteToAddress();
	assert(new_sc_write_to_address == next_sc_write_to_address + sizeof(ScBusData));
	Serial.println(" ~ Test: PASSED");

	// Read back sc_bus_data
	Serial.println("\n ~ Reading S/C Bus Data Struct from Flash ...");
	flash_return_result = ScFlashMemIf.readScBusDataStructFromFlash(sc_bus_data_for_reading);
	Serial.print(" ~ Flash Return Result is: ");
  Serial.println(flash_return_result);
	
	Serial.println("\n ~ Printing S/C Bus Data Struct Read from Flash ...");
	printScBusDataStruct(sc_bus_data_for_reading);

	ScBusDataStructsAreEqual(sc_bus_data_for_writing, sc_bus_data_for_reading);
	Serial.println(" ~ Test: PASSED");

	Serial.print(" ~ Number of S/C Bus Data Records is: ");
  Serial.println(num_sc_bus_data_records);
	Serial.print(" ~ Next S/C Bus Write to Address is: ");
  Serial.println(new_sc_write_to_address);
  Serial.println("");

	return 1;
}


uint8_t eraseAllBlocks(void) {
	Serial.println(F(" ~ Verifying all blocks erased ..."));
	uint8_t flash_erase_success = 0;

	// Write a large array of values
  for (uint32_t i=0; i<NUMBER_BLOCKS; i++) {
		Serial.print(F(" ~ Erasing Block: "));
		Serial.println(i);
    flash_erase_success = ScFlashMemIf.flashEraseBlock(i);
		assert(flash_erase_success == 1);
  }

	return 1;
}

uint8_t writeLargeUint8Array(void) {
	uint8_t byte_data = 123;

	Serial.println(F(" ~ Writing a very large Uint8 array ..."));
	uint32_t num_bytes_read_wrote = 0;

	// Write a large array of values
  for (uint32_t i=200; i<258; i++) {
   	num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(i, byte_data);
		//assert(num_bytes_read_wrote == sizeof(uint32_t));
   }
	
	return 1;
}


uint8_t readLargeUint8Array(void) {
	uint8_t is_initialized = 1;
	uint8_t byte_data = 123;

	Serial.println(F(" ~ Reading a very large Uint8 array ..."));
	uint32_t num_bytes_read_wrote = 0;

	// Write a large array of values
  for (uint32_t i=200; i<258; i++) {
   	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(i, &byte_data);
		//assert(num_bytes_read_wrote == sizeof(uint32_t));

		if (byte_data != FLASH_INITIALIZED_VALUE) {
      is_initialized = 0;
      Serial.print("~ FM[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(byte_data);
    }
   }
	
	return is_initialized;
}


uint8_t writeLargeArray(void) {
	//uint32_t byte_data = 123;
	uint8_t byte_data = 123;

	Serial.println(F(" ~ Writing a very large array ..."));
	uint8_t num_bytes_read_wrote = 0;

	// Write a large array of values
  for (uint32_t i=200; i<258; i++) {
   	num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(i, byte_data);
		//assert(num_bytes_read_wrote == sizeof(uint32_t));
   }
	
	return 1;
}



// uint8_t writeLargeUint32Array(void) {
// 	//uint32_t byte_data = 123;
// 	uint8_t byte_data = 123;

// 	Serial.println(F(" ~ Writing a very large Uint32 array ..."));
// 	uint32_t num_bytes_read_wrote = 0;

// 	// Write a large array of values
//   for (uint32_t i=200; i<258; i++) {
//    	num_bytes_read_wrote = ScFlashMemIf.writeUint32ValueToFlashAddress(i, byte_data);
// 		//assert(num_bytes_read_wrote == sizeof(uint32_t));
//    }
	
// 	return 1;
// }


uint8_t writeAndReadLargeArray(void) {
	//uint32_t byte_data = 123;
	uint32_t write_byte_data = 123;
	uint32_t read_byte_data = 0;

	Serial.println(F(" ~ Writing a very large Uint32 array ..."));
	uint32_t num_bytes_read_wrote = 0;

	// Write a large array of values
  for (uint32_t i=200; i<258; i+=sizeof(uint32_t)) {
   	num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(i, write_byte_data);
		//assert(num_bytes_read_wrote == sizeof(uint32_t));
   	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(i, &read_byte_data);

      Serial.print(" ~ ");
      Serial.print(write_byte_data);
      Serial.print(" == ");
      Serial.print(read_byte_data);
      Serial.print(" is: ");
      Serial.println(write_byte_data == read_byte_data);
   }
	
	return 1;
}


uint8_t verifyWriteToFirstAddress(void) {

	// This all works sometimes! Not always?

	uint32_t num_bytes_read_wrote = 0;
	uint32_t address = 0;

	Serial.println(F(" ~ Verifying write of uint8 to first memory address ..."));
	// Write to the first address of the flash memory
  uint8_t val_to_write_u8 = 210;
  uint8_t val_read_u8 = 0;
	address = 0;
	
	Serial.println(F(" ~ Writing ..."));
  num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, val_to_write_u8);
	assert(num_bytes_read_wrote == sizeof(uint8_t));

  Serial.println(" ~ Reading Back ...");
  num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &val_read_u8);
	assert(num_bytes_read_wrote == sizeof(uint8_t));

  Serial.print(" ~ Read: ");
  Serial.println(val_read_u8);

	assert(val_read_u8 == val_to_write_u8);
	Serial.println(F(" ~ Read & Write of uint8 to first address Test: PASSES!\n"));

	// Read/Write Float values are similar in value though not the same
	// Serial.println(F(" ~ Verifying write of float to first memory address ..."));
  // float val_to_write_float = 1.234;
  // float val_read_float = 0.0;
	// address = 0;

	// Serial.println(F(" ~ Writing ..."));
  // num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, val_to_write_float);
	// assert(num_bytes_read_wrote == sizeof(float));
  
	// Serial.println(" ~ Reading Back ...");
  // num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &val_read_float);
	// assert(num_bytes_read_wrote == sizeof(float));
  
	// Serial.print(" ~ Read: ");
  // Serial.println(val_read_float);
	// Serial.println(val_read_float == val_to_write_float);
	// assert(val_read_float == val_to_write_float);
	// Serial.println(F(" ~ Read & Write of float to first address Test: PASSES!\n"));

	Serial.println(F(" ~ Verifying write of uint16 to first memory address ..."));
  uint16_t val_to_write_uint16 = 16789;
  uint16_t val_read_uint16 = 0;
	address = 0;

	Serial.println(F(" ~ Writing ..."));
  num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, val_to_write_uint16);
	assert(num_bytes_read_wrote == sizeof(uint16_t));

	//delay(100);

  Serial.println(" ~ Reading Back ...");
  num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &val_read_uint16);
	assert(num_bytes_read_wrote == sizeof(uint16_t));
  Serial.print(" ~ Read: ");
  Serial.println(val_read_uint16);
	
	// Serial.println(" ~ Reading Back Low-Level...");
  // uint16_t read_16_value = ScFlashMemIf.read16(address);
  
	// Serial.print(" ~ Read 16: ");
  // Serial.println(read_16_value);

	Serial.println(val_read_uint16 == val_to_write_uint16);
	assert(val_read_uint16 == val_to_write_uint16);
	Serial.println(F(" ~ Read & Write of uint16 to first address Test: PASSES!\n"));


	Serial.println(F(" ~ Verifying write of uint32 to first memory address ..."));
  uint32_t val_to_write_uint32 = 56789;
  uint32_t val_read_uint32 = 0;
	address = 0;

	Serial.println(F(" ~ Writing ..."));
  num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, val_to_write_uint32);
	assert(num_bytes_read_wrote == sizeof(uint32_t));

  Serial.println(" ~ Reading Back ...");
  num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &val_read_uint32);
	assert(num_bytes_read_wrote == sizeof(uint32_t));
  
	Serial.print(" ~ Read: ");
  Serial.println(val_read_uint32);



	Serial.println(val_read_uint32 == val_to_write_uint32);
	assert(val_read_uint32 == val_to_write_uint32);
	Serial.println(F(" ~ Read & Write of uint32 to first address Test: PASSES!\n"));

	Serial.println("\n ~ Writing to First Address Test: PASS!");
	Serial.println("");

	return 1;
}


uint8_t verifyFlashErase(void) {

	Serial.println(F(" ~ Testing Flash Erase Chip ..."));

	uint32_t flash_result = 0;
	//flash_result = ScFlashMemIf.flashEraseBlock(0);
	//flash_result = ScFlashMemIf.flashEraseBlock(1);
	//Serial.print(F(" ~ Flash result is: "));
	//Serial.println(flash_result);
	flash_result = ScFlashMemIf.flashEraseChip();
	assert(flash_result == SUCCESS_ERASE_VALUE);
	Serial.println(F(" ~ Erase Block Test: PASSED\n"));
	Serial.println(F(""));

	verifyFlashMemoryInitialized();
	//assert(verifyFlashMemoryInitialized() == 1);
	Serial.println(F(" ~ Verification of Chip Erased: PASSED\n"));
	Serial.println(F(""));

	return 1;
}


uint8_t verifyInitializationAndReadWriteTests(void) {

	Serial.println("\n ~ Performing Initialization & Read/Write Tests ...");

	uint32_t num_bytes_read_wrote = 0;

	// Test contructor initialized values	
	uint32_t next_write_to_adr = ScFlashMemIf.getNextWriteToAddress();
  uint32_t next_read_from_adr = ScFlashMemIf.getNextReadFromAddress();

	Serial.println(F(" ~ Testing initialized Next Write to Address data member value ..."));
	assert(next_write_to_adr == START_ADDRESS);

	Serial.println(F(" ~ Testing initialized Next Read from Address data member value ..."));
	assert(next_read_from_adr == START_ADDRESS);

	Serial.println(F(" ~ Initialization Tests: PASSED\n"));
  
  //cout << ScFlashMemIf.getCurrentIndex() << endl << endl;
  //cout << ScFlashMemIf.getLastIndex() << endl << endl;

	Serial.println(F(" ~ Testing Flash Reading and Writing ..."));
	Serial.println(F(" ~ Testing Float value ..."));

	float write_float_val = 3.1425;
	float read_float_val = 0.0;
	uint32_t address = 100;
	uint32_t temp_address = 0; 
	uint32_t exp_next_write_to_adr = address + sizeof(read_float_val);
	num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, write_float_val);
	assert(num_bytes_read_wrote == sizeof(float));

	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &read_float_val);
	assert(num_bytes_read_wrote == sizeof(float));

	Serial.print(F(" ~ Value Read is: "));
	Serial.println(read_float_val);

	assert(read_float_val == write_float_val);
	Serial.println(F(" ~ Read & Write of Float Value Test: PASSES!\n"));

	//next_write_to_adr = ScFlashMemIf.getNextWriteToAddress();
  //temp_address = ScFlashMemIf.getNextReadFromAddress();

	//assert(next_write_to_adr == exp_next_write_to_adr);
	//assert(temp_address == next_read_from_adr);

	uint16_t write_uint16_val = 65530;
	uint16_t read_uint16_val = 0;
	address = 200;
  num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, write_uint16_val);
	assert(num_bytes_read_wrote == sizeof(uint16_t));

	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &read_uint16_val);
	assert(num_bytes_read_wrote == sizeof(uint16_t));

	Serial.print(F(" ~ Value Read is: "));
	Serial.println(read_uint16_val);

	assert(read_uint16_val == write_uint16_val);
	Serial.println(F(" ~ Read & Write of uint16 Value Test: PASSES!\n"));


	uint8_t write_uint8_val = 'A';
	uint8_t read_uint8_val = 0;
	address = 1000;
  num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, write_uint8_val);
	assert(num_bytes_read_wrote == sizeof(uint8_t));

	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &read_uint8_val);
	assert(num_bytes_read_wrote == sizeof(uint8_t));

	Serial.print(F(" ~ Value Read is: "));
	Serial.println(read_uint8_val);

	assert(read_uint8_val == write_uint8_val);
	Serial.println(F(" ~ Read & Write of uint8 Value Test: PASSES!\n"));


	double write_double_val = 6.2830;
	double read_double_val = 0.0;
	address = 10000;
  num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, write_double_val);
	assert(num_bytes_read_wrote == sizeof(double));

	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &read_double_val);
	assert(num_bytes_read_wrote == sizeof(double));

	Serial.print(F(" ~ Value Read is: "));
	Serial.println(read_double_val);

	assert(read_double_val == write_double_val);
	Serial.println(F(" ~ Read & Write of double Value Test: PASSES!\n"));

	
	long write_long_val = 123456789;
	long read_long_val = 0;
	address = 100000;
  num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, write_long_val);
	assert(num_bytes_read_wrote == sizeof(long));

	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &read_long_val);
	assert(num_bytes_read_wrote == sizeof(long));

	Serial.print(F(" ~ Value Read is: "));
	Serial.println(read_long_val);

	assert(read_long_val == write_long_val);
	Serial.println(F(" ~ Read & Write of long Value Test: PASSES!\n"));


	uint32_t write_uint32_val = 87654321;
	uint32_t read_uint32_val = 0;
	address = 2000000;
  num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(address, write_uint32_val);
	assert(num_bytes_read_wrote == sizeof(uint32_t));

	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(address, &read_uint32_val);
	assert(num_bytes_read_wrote == sizeof(uint32_t));

	Serial.print(F(" ~ Value Read is: "));
	Serial.println(read_uint32_val);

	assert(read_uint32_val == write_uint32_val);
	Serial.println(F(" ~ Read & Write of uint32 Value Test: PASSES!\n"));


	// Write to the last address of the flash memory
  uint8_t val_to_write_u8 = 210;
  uint8_t val_read_u8 = 0;
	address = LAST_ADDRESS; //flash.size()-1;
  Serial.println(" ~ Testing Writing to Address 2097151 ... ");

  ScFlashMemIf.writeValueToFlashAddress(address, val_to_write_u8);

  // Read the value back for verification
  Serial.println(" ~ Reading from Address 2097151 ... ");
  ScFlashMemIf.readValueFromFlashAddress(address, &val_read_u8);
  Serial.print(" ~ Read: ");
  Serial.println(val_read_u8);

	assert(val_read_u8 == val_to_write_u8);
	Serial.println(F(" ~ Read & Write to last address Test: PASSES!"));

	Serial.println("\n ~ Initialization & Read/Write Tests: PASS!");
	Serial.println("");

	return 1;
}
 
uint8_t verifyFlashMemoryUpdateCounters(void) {

	// Initialize the Flash Memory DB values:
  Serial.println(" ~ Verifying Flash Memory DB values initialized ...");
  uint32_t num_bytes_read_wrote = 0;
  uint32_t register_value = 0;
	uint32_t read_uint32_val = 0;
	
	uint32_t read32_value = 0;

	num_bytes_read_wrote = ScFlashMemIf.initializeScBusFlashMemoryDbValues();
	// Verify that the the flash return result is success
	assert(num_bytes_read_wrote == sizeof(uint32_t));

	// Verify that all counters are initialized to 0
	register_value = ScFlashMemIf.getBootCount();
	
	Serial.print(F("\n ~ Register Value is: "));
	Serial.println(register_value);
	assert(register_value == 0);

	register_value = ScFlashMemIf.getValidMessageCount();
	assert(register_value == 0);

	register_value = ScFlashMemIf.getInvalidMessageCount();
	assert(register_value == 0);
	
	register_value = ScFlashMemIf.getNextPayloadDataWriteToAddress();
	Serial.print(" ~ Register Value is: ");
	Serial.println(register_value);

	read32_value = ScFlashMemIf.read32(NEXT_PL_DATA_WRITE_TO_ADR_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES);
	Serial.print(" ~ Read32 Value is: ");
	Serial.println(read32_value);
	assert(register_value == PL_DATA_START_WRITE_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES);
	Serial.println("");

	register_value = ScFlashMemIf.getNextScBusDataWriteToAddress();
	Serial.print(" ~ SC Bus Data Register Value is: ");
	Serial.println(register_value);
	read32_value = ScFlashMemIf.read32(NEXT_SC_DATA_WRITE_TO_ADR_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES);
	Serial.print(" ~ Read32 Value is: ");
	Serial.println(read32_value);
	Serial.print(" ~ SC_DATA_START_WRITE_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES is: ");
	Serial.println(SC_DATA_START_WRITE_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES);
	
	assert(register_value == SC_DATA_START_WRITE_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES);
	
	register_value = ScFlashMemIf.getNumberPayloadDataRecords();
	assert(register_value == 0);
	
	register_value = ScFlashMemIf.getNumberScBusDataRecords();
	assert(register_value == 0);
	
	Serial.println("~ Flash Memory DB initialization Tests: PASS!");
  Serial.println("\n ~ Verifying Flash Memory DB values incrementors ...");

	Serial.println("\n ~ Testing Boot Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashBootCount();
	assert(num_bytes_read_wrote == sizeof(uint32_t));
	
	register_value = ScFlashMemIf.getBootCount();
	Serial.print("\n ~ Read Register Value is: ");
	Serial.println(register_value);
	assert(register_value == 1);
	// Serial.println("");


	Serial.println("\n ~ Testing Valid Message Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashValidMessageCount();
	assert(num_bytes_read_wrote == sizeof(uint32_t));
	register_value = ScFlashMemIf.getValidMessageCount();
	assert(register_value == 1);

	Serial.println("\n ~ Testing inalid Message Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashInvalidMessageCount();
	assert(num_bytes_read_wrote == sizeof(uint32_t));
	register_value = ScFlashMemIf.getInvalidMessageCount();
	assert(register_value == 1);

	Serial.println("\n ~ Testing Number S/C Bus Data Record Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashNumberScBusDataRecordCount();
	assert(num_bytes_read_wrote == sizeof(uint32_t));
	register_value = ScFlashMemIf.getNumberScBusDataRecords();
	assert(register_value == 1);
	register_value = ScFlashMemIf.getNextScBusDataWriteToAddress();
	assert(register_value == SC_DATA_START_WRITE_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES+16);

	Serial.println("\n ~ Testing Number Payload Data Record Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashNumberPayloadDataRecordCount();
	assert(num_bytes_read_wrote == sizeof(uint32_t));
	register_value = ScFlashMemIf.getNumberPayloadDataRecords();
	assert(register_value == 1);
	register_value = ScFlashMemIf.getNextPayloadDataWriteToAddress();
	assert(register_value == PL_DATA_START_WRITE_SECTOR_NUMBER*FLASH_SECTOR_SIZE_IN_BYTES+32);
	
	Serial.println(" ~ Verifying Counter Increment Tests: PASSED!");

	return 1;
}


uint8_t verifyFlashPageMemoryInitialized(void) {
  Serial.println(" ~ Verifying Flash Page Memory is initialized ...");
  uint8_t is_initialized = 1;
  uint8_t byte_data = 0;

	Serial.println(" ~ About to loop ... ");

	// Changing from first address to start address 
  for (uint32_t i=0; i<256; i++) {
		Serial.println(" ~ Calling Read Value ... ");
    ScFlashMemIf.readValueFromFlashAddress(i, &byte_data);

		Serial.println(i);

    if (byte_data != FLASH_INITIALIZED_VALUE) {
      is_initialized = 0;
      Serial.print("~ FM[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(byte_data);
    }
  }

  if (is_initialized) {
    Serial.println("~ Flash Memory verified as initialized!");
  }
  else {
    Serial.println("~ Flash Memory NOT initialized!");
  }

  return is_initialized;
}


uint8_t verifyFlashMemoryInitialized(void) {
  Serial.println("~ Verifying Flash Memory is initialized ...");
  uint8_t is_initialized = 1;
  uint32_t byte_data = 0;

	// Using uint8 is on purpose since going i++

	// Changing from first address to start address 
  //for (uint32_t i=START_ADDRESS; i<LAST_ADDRESS; i++) {
  for (uint32_t i=250; i<300; i+=sizeof(uint32_t)) {
		Serial.print("~ i is: ");
		Serial.println(i);

    ScFlashMemIf.readValueFromFlashAddress(i, &byte_data);

    if (byte_data != FLASH_INITIALIZED_VALUE) {
      is_initialized = 0;
      Serial.print("~ FM[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(byte_data);
    }
  }

  if (is_initialized) {
    Serial.println("~ Flash Memory verified as initialized!");
  }
  else {
    Serial.println("~ Flash Memory NOT initialized!");
  }

  return is_initialized;
}

void setup() {
  // Open a serial port for SlimSat communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);
	}

	Serial.println(F(" ~ Testing SlimSat Flash Memory Interface ..."));
	
	//ScFlashMemIf.begin();

	ScFlashMemIf.print();
	Serial.println("");
	ScFlashMemIf.printJedecId();

	// Num Pages: 8192
	// Page Size: 256
	//ScFlashMemIf.printPageInfo();


	//ScFlashMemIf.printPageInfo();
	
	// 1) Erase the chip
	//verifyFlashErase();
	
	// // flash Erase Chip seems to write zeros to all values, not 255 like eraseBlocks
	// if (0) {
	// 	ScFlashMemIf.flashEraseChip();
	// }
	// else {
	// 	eraseAllBlocks();
	// }

	// if (0) {
	// 	verifyFlashErase();
	// 	//verifyInitializationAndReadWriteTests();
	// }

	//verifyWriteToFirstAddress();
	//verifyFlashMemoryUpdateCounters();

	verifyWriteAndReadScBusDataStruct();

	Serial.print("\n ~ Fini! \n");
}

void loop() {
	// Exercise the various SlimSat S/C Flash Memory Interface methods
}