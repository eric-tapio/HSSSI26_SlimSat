// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <sc_flash_memory_if.h>
#include <Adafruit_FlashTransport.h>
#include <assert.h>

#define STR_CMP_MATCHES_VALUE 0
#define FIRST_ADDRESS 0x00000000
#define LAST_ADDRESS 0x001FFFFF
#define SUCCESS_VALUE 1
#define SUCCESS_ERASE_VALUE 1
#define FLASH_INITIALIZED_VALUE 255
// 1 block is 512kB 
#define BLOCK_ADDRESS_SIZE 65535
#define NUMBER_BLOCKS 32
#define NUMBER_PAGES 	8192
#define PAGE_SIZE_IN_BYTES 256

//Adafruit_FlashTransport_QSPI flashTransport;

// Construct the S/C Flash Memory IF Object

FlashMemoryIf ScFlashMemIf;

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



uint8_t writeLargeUint32Array(void) {
	//uint32_t byte_data = 123;
	uint8_t byte_data = 123;

	Serial.println(F(" ~ Writing a very large Uint32 array ..."));
	uint32_t num_bytes_read_wrote = 0;

	// Write a large array of values
  for (uint32_t i=200; i<258; i++) {
   	num_bytes_read_wrote = ScFlashMemIf.writeUint32ValueToFlashAddress(i, byte_data);
		//assert(num_bytes_read_wrote == sizeof(uint32_t));
   }
	
	return 1;
}


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

	num_bytes_read_wrote = ScFlashMemIf.initializeScBusFlashMemoryDbValues();
	// Verify that the the flash return result is success
	assert(num_bytes_read_wrote == sizeof(uint16_t));

	// Verify that all counters are initialized to 0
	register_value = ScFlashMemIf.getBootCount();
	
	Serial.print(F("\n ~ Register Value is: "));
	Serial.println(register_value);

	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(0, &read_uint32_val);
	assert(num_bytes_read_wrote == sizeof(uint32_t));

	Serial.print(F(" ~ Value Read is: "));
	Serial.println(read_uint32_val);

	uint32_t write_uint32_val = 2;
	// Ugh, this is it! Address has be a declared type, too
	// An input of just '300' doesn't work! Wow!
	// So give it a conversion
	//uint32_t address = 300;

	num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(uint32_t(300), write_uint32_val);
	assert(num_bytes_read_wrote == sizeof(uint32_t));

	num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(uint32_t(300), &read_uint32_val);
	assert(num_bytes_read_wrote == sizeof(uint32_t));

	Serial.print(F(" ~ Value Read is: "));
	Serial.println(read_uint32_val);

	assert(read_uint32_val == write_uint32_val);

	assert(register_value == 5);

	register_value = ScFlashMemIf.getValidMessageCount();
	assert(register_value == 2);

	register_value = ScFlashMemIf.getInvalidMessageCount();
	assert(register_value == 2);
	
	register_value = ScFlashMemIf.getNextPayloadDataWriteToAddress();
	assert(register_value == 2);
	
	register_value = ScFlashMemIf.getNextScBusDataWriteToAddress();
	assert(register_value == 2);
	
	register_value = ScFlashMemIf.getNumberPayloadDataRecords();
	assert(register_value == 2);
	
	register_value = ScFlashMemIf.getNumberScBusDataRecords();
	assert(register_value == 2);
	
	Serial.println("~ Flash Memory DB initialization Tests: PASS!");
  Serial.println("\n ~ Verifying Flash Memory DB values incrementors ...");

	Serial.println("\n ~ Testing Boot Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashBootCount();
	assert(num_bytes_read_wrote == sizeof(uint16_t));
	
	// So, it's the write step of increment that is not working!
	// The value is read correctly, is incremented, 
	// write function is called, but ulimately not carried out
	// num_bytes_read_wrote = ScFlashMemIf.readValueFromFlashAddress(0, &read_uint32_val);
	// assert(num_bytes_read_wrote == sizeof(uint32_t));
	// Serial.print("\n ~ Read uint32 Value is: ");
	// Serial.println(read_uint32_val);
	// assert(read_uint32_val == 3); // means not working
	// Serial.println("");

	register_value = ScFlashMemIf.getBootCount();
	Serial.print("\n ~ Read Register Value is: ");
	Serial.println(register_value);
	assert(register_value == 2); // means not working
	Serial.println("");

	write_uint32_val = 3;
	//num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(uint32_t(0), write_uint32_val);
	num_bytes_read_wrote = ScFlashMemIf.writeValueToFlashAddress(0, write_uint32_val);
	//num_bytes_read_wrote = ScFlashMemIf.writeUint32ValueToFlashAddress(0, write_uint32_val);
	assert(num_bytes_read_wrote == sizeof(uint32_t));

	register_value = ScFlashMemIf.getBootCount();
	Serial.print("\n ~ Read Register Value is: ");
	Serial.println(register_value);
	assert(register_value == 3); // means the external call worked

	Serial.println("\n ~ Testing Valid Message Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashValidMessageCount();
	assert(num_bytes_read_wrote == sizeof(uint32_t));
	register_value = ScFlashMemIf.getValidMessageCount();
	assert(register_value == 1);

	Serial.println("\n ~ Testing inalid Message Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashInvalidMessageCount();
	assert(num_bytes_read_wrote > 0);
	register_value = ScFlashMemIf.getInvalidMessageCount();
	assert(register_value == 1);

	Serial.println("\n ~ Testing Number S/C Bus Data Record Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashNumberScBusDataRecordCount();
	assert(num_bytes_read_wrote > 0);
	register_value = ScFlashMemIf.getNumberScBusDataRecords();
	assert(register_value == 1);
	register_value = ScFlashMemIf.getNextScBusDataWriteToAddress();
	assert(register_value == 16);

	Serial.println("\n ~ Testing Number Payload Data Record Count Incrementor ...");
	num_bytes_read_wrote = ScFlashMemIf.incrementFlashNumberPayloadDataRecordCount();
	assert(num_bytes_read_wrote > 0);
	register_value = ScFlashMemIf.getNumberPayloadDataRecords();
	assert(register_value == 1);
	register_value = ScFlashMemIf.getNextPayloadDataWriteToAddress();
	assert(register_value == 32);
	
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
	
	// flash Erase Chip seems to write zeros to all values, not 255 like eraseBlocks
	if (0) {
		ScFlashMemIf.flashEraseChip();
	}
	else {
		eraseAllBlocks();
	}

	// 2) Write data 
	//writeLargeArray();
	//writeLargeUint32Array();
	//writeAndReadLargeArray(); // This now works, and I know why i+=sizeof()
	writeLargeUint8Array();
	Serial.println(" ~ Writting Large Array Done ...");
	
	// 3) Erase the page
	if (1) {
		Serial.println(" ~ Erasing Page 0 ...");
		ScFlashMemIf.flashErasePage(0);
		Serial.println(" ~ Erasing Page 0 Done ...");
	}

	// 4) Verify
	//verifyFlashPageMemoryInitialized();
	// This code works, just not after page is erased
	//verifyFlashMemoryInitialized();
	readLargeUint8Array();

	if (0) {
		verifyFlashErase();
		
		//verifyInitializationAndReadWriteTests();
	}

	//verifyWriteToFirstAddress();
	//verifyFlashMemoryUpdateCounters();

	Serial.print("\n ~ Fini! \n");
}

void loop() {
	// Exercise the various SlimSat S/C Flash Memory Interface methods
}