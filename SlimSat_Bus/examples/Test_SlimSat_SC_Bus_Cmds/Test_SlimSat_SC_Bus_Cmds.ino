// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <slimsat_bus.h>
#include <assert.h>

#define STR_CMP_MATCHES_VALUE 0


char bus_ping_msg[] = "$GS01,1*08";
//char bus_ping_msg[] = "$GS01,1,*24";

char bus_get_bus_db_register_values_msg[] = "$GS01,2*0B";
char bus_get_bus_data_msg[] = "$GS01,3*0A";
char bus_get_bus_data_msg_w_rec_num[] = "$GS01,3,1*17";
//char bus_get_bus_data_msg_w_rec_num[] = "$GS01,3,2*14";
char bus_get_bus_data_msg_w_rec_num_range[] = "$GS01,3,2,4*0C";
//char bus_get_bus_data_msg_w_rec_num[] = "$GS01,3,4*12";

char bus_get_payload_data_msg[] = "$GS01,4*0D";
char bus_get_payload_data_msg_w_rec_num[] = "$GS01,4,1*10";
//char bus_get_payload_data_msg_w_rec_num[] = "$GS01,4,4*15";
char bus_get_payload_data_msg_w_rec_num_range[] = "$GS01,4,1,4*08";

char bus_get_frequency_msg[] = "$GS01,5*0C";
char bus_set_frequency_msg[] = "$GS01,6,438123456*1B";
char bus_get_bandwidth_msg[] = "$GS01,7*0E";
//char bus_set_bandwidth_msg[] = "$GS01,8,30000*1E";  // Bandwidth can only take on certain values. Test this in an off nominal version
char bus_set_bandwidth_msg[] = "$GS01,8,31250*18";
char bus_get_spread_factor_msg[] = "$GS01,9*00";
char bus_set_spread_factor_msg[] = "$GS01,10,6*22";
char bus_get_coding_rate_msg[] = "$GS01,11*39";
char bus_set_coding_rate_msg[] = "$GS01,12,5*21";
char bus_get_output_power_msg[] = "$GS01,13*3B";
char bus_set_output_power_msg[] = "$GS01,14,2*22";
char bus_get_current_limit_msg[] = "$GS01,15*3D";
char bus_set_current_limit_msg[] = "$GS01,16,45*13";

char bus_get_preamble_length_msg[] = "$GS01,17*3F";
char bus_set_preamble_length_msg[] = "$GS01,18,6*2A";

char bus_get_gain_msg[] = "$GS01,19*31";
char bus_set_gain_msg[] = "$GS01,20,4*23";

char bus_get_frequency_error_msg[] = "$GS01,21*3A";
char bus_get_snr_msg[] = "$GS01,22*39";

char bus_get_rssi_msg[] = "$GS01,23*38";
char bus_get_beacon_period_msg[] = "$GS01,24*3F";
char bus_set_beacon_period_msg[] = "$GS01,25,33000*22";
char bus_get_bus_mode_msg[] = "$GS01,26*3D";
char bus_set_bus_mode_msg[] = "$GS01,27,1*21";
char bus_get_bus_epoch_time_msg[] = "$GS01,28*33";
char bus_set_bus_epoch_time_msg[] = "$GS01,29,12345*2F";
char bus_get_bus_time_msg[] = "$GS01,30*3A";
//char bus_get_bus_data_msg[] = "$GS01,31*3B";
//char bus_get_bus_stream_data_mode_msg[] = "$GS01,32*38";
//char bus_set_bus_stream_data_mode_msg[] = "$GS01,33,1*24";

//char bus_get_bus_db_register_values_msg[] = "$GS01,32*38";
char bus_get_safe_mode_voltage_msg[] = "$GS01,31*3B";
char bus_set_safe_mode_voltage_msg[] = "$GS01,32,3141*13";

char bus_get_bus_data_record_period_msg[] = "$GS01,33*39";
char bus_set_bus_data_record_period_msg[] = "$GS01,34,5000*17";

char bus_get_payload_op_period_msg[] = "$GS01,35*3F";
char bus_set_payload_op_period_msg[] = "$GS01,36,5000*15";
//char bus_get_payload_mode_msg[] = "$GS01,37*3D";
//char bus_get_payload_data_msg[] = "$GS01,38*32";

char bus_reboot_msg[] = "$GS01,37*3D";

char bus_get_flash_register_msg[] = "$GS01,38,1234*1A";
char bus_get_flash_register_2_msg[] = "$GS01,38,1234,4*02";
char bus_set_flash_register_msg[] = "$GS01,39,5678*13"; // This is invalid, or will set to 0
char bus_set_flash_register_2_msg[] = "$GS01,39,5678,6789*3F";

char bus_erase_flash_chip_memory_msg[] = "$GS01,40*3D";
char bus_erase_flash_block_memory_msg[] = "$GS01,41,31*12";
char bus_erase_flash_sector_memory_msg[] = "$GS01,42,1020*10";

char bus_erase_bus_bus_data_blocks_memory_msg[] = "$GS01,43*3E";
char bus_erase_payload_data_blocks_memory_msg[] = "$GS01,44*39";

char bus_get_gps_position_msg[] = "$GS01,45*38";
char bus_cutdown_burn_wire_msg[] = "$GS01,46*3B";


// Yes, both work!
//char payload_1_ping_payload_msg[] = "$GS01,50*3C";
char payload_1_ping_payload_msg[] = "$GS01,50,*10";
char payload_2_get_payload_mode_msg[] = "$GS01,51*3D";
char payload_3_get_num_meas_to_take_msg[] = "$GS01,52*3E";
char payload_4_set_num_meas_to_take_msg[] = "$GS01,53,16*14";
char payload_5_take_meas_msg[] = "$GS01,54*38";
char payload_6_get_raw_meas_data_msg[] = "$GS01,55*39";
char payload_7_get_processed_meas_data_msg[] = "$GS01,56*3A";
char payload_8_print_meas_data_msg[] = "$GS01,57*3B";
char payload_9_undefined_msg[] = "$GS01,58*34";
char payload_10_undefined_msg[] = "$GS01,59*35";

char bus_invalid_incorrect_checksum_msg[] = "$GS01,2,1000*AA";
char bus_invalid_cmd_id_msg[] = "$GS01,66,*15";
char bus_incorrect_slimsat_id_msg[] = "$GS02,2,1000*25";
char bus_invalid_partial_first_half_msg[] = "$GS01,2,";
char bus_invalid_partial_second_half_msg[] = ",1000*25";

// Construct the SlimSat Bus Object
SlimSatBus Slimsat;


void setup() {
  // Open a serial port for SlimSat communication
	Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F("\n ~ Running: Test SlimSat Bus Commands ... "));

	// Start the radio (or initialize the bus)
	Serial.println(F(" ~ Erasing Flash Memory Chip & Reinitializing Bus ... "));
	Slimsat.Bus_database.Flash_Memory.flashEraseChip();
	Slimsat.initializeBus();

	// Test all SlimSat commands
	uint8_t num_bfr_msgs = 0;
	char* bus_data_msg = NULL;
	int i = 0;
	
	
  Serial.print(F("\n ~ Sending a Ping Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_ping_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Ping Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,1,1*78") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,1,1,1*65") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Bus DB Register Values Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_db_register_values_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	assert(num_bfr_msgs == 2);
	//assert(strcmp(bus_data_msg, "$S01G,A,2,1,1,2,0,0,65536*67") == STR_CMP_MATCHES_VALUE); // Then changed the order
	assert(strcmp(bus_data_msg, "$S01G,A,2,1,1,2,0,0,0*64") == STR_CMP_MATCHES_VALUE);

	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);
	//assert(strcmp(bus_data_msg, "$S01G,2,2,0,1048576*1C") == STR_CMP_MATCHES_VALUE); // Then changed the order
	assert(strcmp(bus_data_msg, "$S01G,2,2,65536,1048576*1F") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Bus Data Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_data_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Data Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	// Verify that there is only one message in the circular buffer
	//assert(num_bfr_msgs == 4);
		
	for (i=0; i<(num_bfr_msgs-1); i++) {
		bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
		Serial.println(bus_data_msg);
	}
	
	
	// Record Bus Data, then request the data back
	Slimsat.recordBusData();
	delay(1000);
	Slimsat.recordBusData();
	delay(1000);
	Slimsat.recordBusData();
	delay(1000);
	Slimsat.recordBusData();
	delay(1000);
	Slimsat.recordBusData();
	delay(1000);


	Serial.print(F("\n ~ Sending a Get Bus Data Command w/ Rec number ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_data_msg_w_rec_num, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Data Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	// Verify that there is only one message in the circular buffer
	//assert(num_bfr_msgs == 4);
		
	for (i=0; i<(num_bfr_msgs-1); i++) {
		bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
		Serial.println(bus_data_msg);
	}


	Serial.print(F("\n ~ Sending a Get Bus Data Command w/ Rec number Range ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_data_msg_w_rec_num_range, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Data Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);
	

	Serial.print(F("\n ~ Sending a Get Payload Data Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_payload_data_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Data Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	// Verify that there is only one message in the circular buffer
	//assert(num_bfr_msgs == 10);

	for (i=0; i<(num_bfr_msgs-1); i++) {
		bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
		Serial.println(bus_data_msg);
	}


	// Record Payload Data, then request the data back
	Slimsat.performPayloadOp();
	delay(1000);
	Slimsat.performPayloadOp();
	delay(1000);
	Slimsat.performPayloadOp();
	delay(1000);
	Slimsat.performPayloadOp();
	delay(1000);
	Slimsat.performPayloadOp();
	delay(1000);

	Serial.print(F("\n ~ Sending a Get Payload Data Command w/ Rec number ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_payload_data_msg_w_rec_num, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Data Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	// Verify that there is only one message in the circular buffer
	//assert(num_bfr_msgs == 4);
			
	for (i=0; i<(num_bfr_msgs-1); i++) {
		bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
		Serial.println(bus_data_msg);
	}


	Serial.print(F("\n ~ Sending a Get Payload Data Command w/ Rec number Range ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_payload_data_msg_w_rec_num_range, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Data Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	for (i=0; i<(num_bfr_msgs-1); i++) {
		bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
		Serial.println(bus_data_msg);
	}
	

  Serial.print(F("\n ~ Sending a Get Radio Frequency Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_frequency_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Frequency Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected frequency value is the default fequency of 438E6
	//assert(strcmp(bus_data_msg, "$S01G,A,5,438000000,6*68") == STR_CMP_MATCHES_VALUE);
	// The expected frequency value is the default fequency of 434,000,000 Hz	
	//assert(strcmp(bus_data_msg, "$S01G,A,5,434000000,9*6B") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,5,1,434000000,9*76") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Radio Frequency Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_frequency_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Frequency Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,6,438123456,7*6D") == STR_CMP_MATCHES_VALUE);
	//assert(strcmp(bus_data_msg, "$S01G,A,6,438123456,4*6E") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,6,1,438123456,10*46") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Radio Frequency Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_frequency_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Frequency Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected frequency is the value set by the set frequency cmd: 438,123,456 Hz
	//assert(strcmp(bus_data_msg, "$S01G,A,5,438123456,8*61") == STR_CMP_MATCHES_VALUE);
	//assert(strcmp(bus_data_msg, "$S01G,A,5,438123456,5*6C") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,5,1,438123456,11*44") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Radio Bandwidth Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bandwidth_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Bandwidth Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected bandwidth value is the default bandwidth of 31000
	//assert(strcmp(bus_data_msg, "$S01G,A,7,31000,9*68") == STR_CMP_MATCHES_VALUE);
	// The expected bandwidth value is the default bandwidth of 125,000 Hz
	//assert(strcmp(bus_data_msg, "$S01G,A,7,125000,6*53") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,7,1,125000,12*7B") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Radio Bandwidth Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_bandwidth_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Bandwidth Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,8,31250,7*6E") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,8,1,31250,13*46") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Radio Bandwidth Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bandwidth_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Bandwidth Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected bandwidth value is the value set by the set bandwidth cmd: 30000
	//assert(strcmp(bus_data_msg, "$S01G,A,7,31250,8*6E") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,7,1,31250,14*4E") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio Spread Factor Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_spread_factor_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected bandwidth value is the default bandwidth of 6
	//assert(strcmp(bus_data_msg, "$S01G,A,9,9,9*6D") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,9,1,9,15*4D") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Radio Spread Factor Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_spread_factor_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,10,6,10*62") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,10,1,6,16*79") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Get Radio Spread Factor Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_spread_factor_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected coding rate value is the value set by the set coding rate cmd
	//assert(strcmp(bus_data_msg, "$S01G,A,9,6,11*5B") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,9,1,6,17*40") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio Coding Rate Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_coding_rate_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected coding rate value is the default coding rate of 6
	//assert(strcmp(bus_data_msg, "$S01G,A,11,7,12*60") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,11,1,7,18*77") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Radio Coding Rate Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_spread_factor_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,10,6,13*61") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,10,1,6,19*76") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Get Radio Coding Rate Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_spread_factor_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected coding rate value is the value set by the set coding rate cmd
	//assert(strcmp(bus_data_msg, "$S01G,A,9,6,14*5E") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,9,1,6,20*44") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio Output Power Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_output_power_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected output power value is the default output power of 10
	//assert(strcmp(bus_data_msg, "$S01G,A,13,10,15*53") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,13,1,-3,21*56") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Output Power Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_output_power_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,14,2,16*64") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,14,1,2,22*7E") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Get Radio Output Power Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_output_power_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected output power value is the value set by the set output power cmd
	//assert(strcmp(bus_data_msg, "$S01G,A,13,2,17*62") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,13,1,2,23*78") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio Current Limit Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_current_limit_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected current limit value is the default current limit of 80 mA
	//assert(strcmp(bus_data_msg, "$S01G,A,15,80,18*51") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,15,1,80,24*43") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Current Limit Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_current_limit_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,16,45,19*5A") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,16,1,45,25*48") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Get Radio Current Limit Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_current_limit_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected current limit value is the value set by the set current limit cmd
	//assert(strcmp(bus_data_msg, "$S01G,A,15,45,20*53") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,15,1,45,26*48") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio Preamble Length Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_preamble_length_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected preamble length value is the default preamble length of 8
	//assert(strcmp(bus_data_msg, "$S01G,A,17,8,21*69") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,17,1,8,27*72") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Preamble Length Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_preamble_length_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,18,6,22*6B") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,18,1,6,28*7C") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Get Radio Preamble Length Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_preamble_length_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The preamble length limit value is the value set by the set preamble length cmd
	//assert(strcmp(bus_data_msg, "$S01G,A,17,6,23*65") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,17,1,6,29*72") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio Gain Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_gain_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected gain value is the default gain of 0
	//assert(strcmp(bus_data_msg, "$S01G,A,19,0,24*6A") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,19,1,1,30*73") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Gain Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_gain_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,20,1,25*60") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,20,1,4,31*7D") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Get Radio Gain Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_gain_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The gain value is the value set by the set gain cmd
	//assert(strcmp(bus_data_msg, "$S01G,A,19,1,26*69") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,19,1,4,32*74") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio Frequency Error Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_frequency_error_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio RSSSI Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected frequency error value is the default value of 0
	//assert(strcmp(bus_data_msg, "$S01G,A,21,0.00,27*4C") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,21,1,0.00,33*54") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio SNR Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_snr_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio RSSSI Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected SNR value is the default value of 0
	//assert(strcmp(bus_data_msg, "$S01G,A,22,0.00,28*40") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,22,1,0.00,34*50") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio RSSI Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_rssi_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio RSSSI Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected RSSI value is the default value of -164
	//assert(strcmp(bus_data_msg, "$S01G,A,23,-164.00,29*6E") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,23,1,-164.00,35*7E") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Beacon Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_beacon_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Beacon Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected period value is the default period of 2000
	//assert(strcmp(bus_data_msg, "$S01G,A,24,2000,30*53") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,24,1,2000,36*48") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Beacon Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_beacon_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Beacon Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,25,33000,31*61") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,25,1,33000,37*7A") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Beacon Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_beacon_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Beacon Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected beacon period value is the value set by the set beacon period cmd: 33000
	//assert(strcmp(bus_data_msg, "$S01G,A,24,33000,32*63") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,24,1,33000,38*74") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Bus Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_mode_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Mode Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Bus Mode value is the default Bus Mode of 0
	//assert(strcmp(bus_data_msg, "$S01G,A,26,0,33*60") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,26,1,0,39*77") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Bus Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_bus_mode_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Bus Mode Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,27,1,34*67") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,27,1,1,40*79") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Bus Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_mode_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Mode Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Bus Mode value is the value set by the set Bus Mode cmd: 1
	//assert(strcmp(bus_data_msg, "$S01G,A,26,1,35*67") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,26,1,1,41*79") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Bus Epoch Time Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_epoch_time_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Time Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Bus Time value is the default Bus Time of 0
	//assert(strcmp(bus_data_msg, "$S01G,A,28,0,36*6B") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,28,1,0,42*75") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Bus Epoch Time Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_bus_epoch_time_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Bus Time Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,29,12345,37*6A") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,29,1,12345,43*74") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Bus Epoch Time Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_epoch_time_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Time Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Bus Epoch Time value is the value set by the set Bus Epoch Time cmd: 12345
	//assert(strcmp(bus_data_msg, "$S01G,A,28,12345,38*64") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,28,1,12345,44*72") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Bus Time Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_time_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Time Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Bus Time value is the default Bus Time of 0
	//assert(strcmp(bus_data_msg, "$S01G,A,30,0,39*6D") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,30,1,0,45*7B") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Safe Mode Voltage Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_safe_mode_voltage_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Frequency Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Safe Mode Voltage value is the default voltage of 2850 mV (or 2.85 V)	
	//assert(strcmp(bus_data_msg, "$S01G,A,39,2850,8*69") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,31,1,2850,46*46") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Safe Mode Voltage Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_safe_mode_voltage_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Frequency Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,40,3141,9*6E") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,32,1,3141,47*4C") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Safe Mode Voltage Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_safe_mode_voltage_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Frequency Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected safe mode voltage is the value set by the set safe mode voltage cmd: 3141 mV (or 3.141 V)
	//assert(strcmp(bus_data_msg, "$S01G,A,39,3141,10*58") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,31,1,3141,48*40") == STR_CMP_MATCHES_VALUE);



  Serial.print(F("\n ~ Sending a Get Bus Data Record Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_data_record_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected period value is the default period of 2000
	//assert(strcmp(bus_data_msg, "$S01G,A,35,2000,3*63") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,33,1,2000,49*46") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Bus Data Record Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_bus_data_record_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,36,5000,4*60") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,34,1,5000,50*4E") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Bus Data Record Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_data_record_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected period value is the value set by the set period cmd: 5000
	//assert(strcmp(bus_data_msg, "$S01G,A,35,5000,5*62") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,33,1,5000,51*48") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Payload Op Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_payload_op_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected period value is the default period of 2000
	assert(strcmp(bus_data_msg, "$S01G,A,35,1,2000,52*4A") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Payload Op Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_payload_op_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,36,5000,4*60") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,36,1,5000,53*4F") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Payload Op Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_payload_op_period_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected period value is the value set by the set period cmd: 5000
	//assert(strcmp(bus_data_msg, "$S01G,A,35,5000,5*62") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,35,1,5000,54*4B") == STR_CMP_MATCHES_VALUE);

	// Note: Reboot command is tested at the end of the Bus Section

  Serial.print(F("\n ~ Sending a Get Flash Register Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_flash_register_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Register Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected register value is the default register value of 10 
	assert(strcmp(bus_data_msg, "$S01G,A,38,1,4294967295,55*4F") == STR_CMP_MATCHES_VALUE);
  
	/// This should be invalid, add in validation
  Serial.print(F("\n ~ Sending a Set Flash Register Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_flash_register_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Register Command, You still need one additional input parameter for this command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,A,39,1,0,56*70") == STR_CMP_MATCHES_VALUE);
  

  Serial.print(F("\n ~ Sending a Get Flash Register Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_flash_register_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Register Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected register value is the value set by the set register cmd: 5678
	//assert(strcmp(bus_data_msg, "$S01G,A,3,4294967295,13*68") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,38,1,4294967295,57*4D") == STR_CMP_MATCHES_VALUE);
	


  Serial.print(F("\n ~ Sending a Set Flash Register 2 Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_set_flash_register_2_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Register Command, You still need one additional input parameter for this command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	//assert(strcmp(bus_data_msg, "$S01G,A,4,6789,14*65") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,39,1,6789,58*4E") == STR_CMP_MATCHES_VALUE);
  

  Serial.print(F("\n ~ Sending a Get Flash Register 2 Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_get_flash_register_2_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Register Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected register value is the value set by the set register cmd: 5678
	//assert(strcmp(bus_data_msg, "$S01G,A,3,4294967295,15*6E") == STR_CMP_MATCHES_VALUE);
	assert(strcmp(bus_data_msg, "$S01G,A,38,1,4294967295,59*43") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a S/C Erase Chip Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_erase_flash_chip_memory_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Ping Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,A,40,1,60*67") == STR_CMP_MATCHES_VALUE);


	// Verify that the DB Registers have been reset to their default values
	Serial.print(F("\n ~ Sending a Get Bus DB Register Values Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_db_register_values_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	assert(num_bfr_msgs == 2);
	//assert(strcmp(bus_data_msg, "$S01G,A,2,1,1,2,0,0,65536*67") == STR_CMP_MATCHES_VALUE); // Then changed the order
	assert(strcmp(bus_data_msg, "$S01G,A,2,1,0,1,0,0,0*66") == STR_CMP_MATCHES_VALUE);

	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);
	//assert(strcmp(bus_data_msg, "$S01G,2,2,0,1048576*1C") == STR_CMP_MATCHES_VALUE); // Then changed the order
	assert(strcmp(bus_data_msg, "$S01G,2,2,65536,1048576*1F") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a S/C Erase Block Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_erase_flash_block_memory_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Ping Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,A,41,1,2*52") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a S/C Erase Sector Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_erase_flash_sector_memory_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Ping Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,A,42,1,3*50") == STR_CMP_MATCHES_VALUE);


// Record Bus Data
	Slimsat.recordBusData();
	delay(1000);
	Slimsat.recordBusData();
	delay(1000);
	Slimsat.recordBusData();
	delay(1000);
	Slimsat.recordBusData();
	delay(1000);
	Slimsat.recordBusData();
	delay(1000);
	
	// Record Payload Data
	Slimsat.performPayloadOp();
	delay(1000);
	Slimsat.performPayloadOp();
	delay(1000);
	Slimsat.performPayloadOp();
	delay(1000);
	Slimsat.performPayloadOp();
	delay(1000);
	Slimsat.performPayloadOp();
	delay(1000);

  // Get the register values set before the erase
	Serial.print(F("\n ~ Sending a Get Bus DB Register Values Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_db_register_values_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	assert(num_bfr_msgs == 2);
	//assert(strcmp(bus_data_msg, "$S01G,A,2,1,0,4,0,5,65846*6F") == STR_CMP_MATCHES_VALUE); // Then changed the order
	assert(strcmp(bus_data_msg, "$S01G,A,2,1,0,4,0,5,5*63") == STR_CMP_MATCHES_VALUE); // Then changed the order

	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);
	//assert(strcmp(bus_data_msg, "$S01G,2,2,5,1049566*19") == STR_CMP_MATCHES_VALUE); // Then changed the order
	assert(strcmp(bus_data_msg, "$S01G,2,2,65846,1049566*15") == STR_CMP_MATCHES_VALUE); // Then changed the order
	

	Serial.print(F("\n ~ Sending a S/C Erase Bus Data Blocks Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_erase_bus_bus_data_blocks_memory_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Ping Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,A,43,1,5*57") == STR_CMP_MATCHES_VALUE);


	// Verify that the Bus DB Registers have been reset to their default values
	Serial.print(F("\n ~ Sending a Get Bus DB Register Values Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_db_register_values_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	assert(num_bfr_msgs == 2);
	//assert(strcmp(bus_data_msg, "$S01G,A,2,1,0,6,0,0,65536*62") == STR_CMP_MATCHES_VALUE); // Then I changed the order
	assert(strcmp(bus_data_msg, "$S01G,A,2,1,0,6,0,0,5*64") == STR_CMP_MATCHES_VALUE); // Then I changed the order

	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);
	//assert(strcmp(bus_data_msg, "$S01G,2,2,5,1049566*19") == STR_CMP_MATCHES_VALUE); // Then I changed the order
	assert(strcmp(bus_data_msg, "$S01G,2,2,65536,1049566*1F") == STR_CMP_MATCHES_VALUE); // Then I changed the order


	Serial.print(F("\n ~ Sending a S/C Erase Payload Data Blocks Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_erase_payload_data_blocks_memory_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Ping Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,A,44,1,7*52") == STR_CMP_MATCHES_VALUE);


	// Verify that the Payload DB Registers have been reset to their default values
	Serial.print(F("\n ~ Sending a Get Bus DB Register Values Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_bus_db_register_values_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	assert(num_bfr_msgs == 2);
	//assert(strcmp(bus_data_msg, "$S01G,A,2,1,0,8,0,0,65536*6C") == STR_CMP_MATCHES_VALUE); // Then I changed the order
	assert(strcmp(bus_data_msg, "$S01G,A,2,1,0,8,0,0,0*6F") == STR_CMP_MATCHES_VALUE); // Then I changed the order

	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);
	//assert(strcmp(bus_data_msg, "$S01G,2,2,0,1048576*1C") == STR_CMP_MATCHES_VALUE); // Then I changed the order
	assert(strcmp(bus_data_msg, "$S01G,2,2,65536,1048576*1F") == STR_CMP_MATCHES_VALUE); // Then I changed the order


	Serial.print(F("\n ~ Sending a Get GPS Position Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_get_gps_position_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Data Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	// If the GPS has a valid position fix, then 3 messages will be received
	// Otherwise only one message will be received
	assert((num_bfr_msgs == 1) or (num_bfr_msgs == 3));

	if (num_bfr_msgs == 3) {
		for (i=0; i<(num_bfr_msgs-1); i++) {
			bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
			Serial.println(bus_data_msg);
		}
	}


	Serial.print(F("\n ~ Sending a Cutdown (Burn Wire) Command ..."));
	Slimsat.Msg_handler.handleNmeaMsg(bus_cutdown_burn_wire_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Bus Data Command
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	Serial.print(" ~ num_bfr_msgs is: ");
	Serial.println(num_bfr_msgs);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	assert(strcmp(bus_data_msg, "$S01G,A,46,1,10*66") == STR_CMP_MATCHES_VALUE);


  Serial.println(F("\n\n ~ Bus Commands: PASS!"));
  Serial.println(F("\n ~ Running Payload Commands ..."));


  Serial.print(F("\n ~ Sending an Invalid Command (Incorrect Checksum) ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_invalid_incorrect_checksum_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (invalid checksum)
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,N,0,1,0*6A") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending an Invalid Command (Invalid Cmd ID)..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_invalid_cmd_id_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (Invalid Cmd #)
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,N,0,1,0*6A") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending an Invalid Command (Incorrect SlimSat ID) ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_incorrect_slimsat_id_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (Invalid Listener ID)
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,N,0,1,0*6A") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending an Invalid Command (Partial Message, 1st half only) ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_invalid_partial_first_half_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (Partial Command, 1st half)
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,N,0,1,0*6A") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending an Invalid Command (Partial Message, 2nd half only) ..."));
  Slimsat.Msg_handler.handleNmeaMsg(bus_invalid_partial_second_half_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (Partial Command, 2nd half)
	Serial.print(F("\n ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
	bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
	Serial.println(bus_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(bus_data_msg, "$S01G,N,0,1,0*6A") == STR_CMP_MATCHES_VALUE);


	Serial.println(F("\n ~ Congrats, all SlimSat Cmd Tests: PASS!\n"));	
	Serial.println(F("\n ~ Now Rebooting ..."));

	if (1) {
		// You will never get an ack from this command. The reboot will be performed before the message is sent
		Serial.print(F("\n ~ Sending a Reboot Command ..."));
		Slimsat.Msg_handler.handleNmeaMsg(bus_reboot_msg, Slimsat.Bus_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Gps, Slimsat.Bus_timer, Slimsat.Payload_timer, Slimsat.Beacon_timer);
		//Serial.print(F("\n ~ Received response from S/C: "));
		//num_bfr_msgs = Slimsat.Msg_handler.Bus_cbfr.getCircularBufferNumMsgs();
		//bus_data_msg = Slimsat.Msg_handler.Bus_cbfr.circularBufferPopMsg();
		//Serial.println(bus_data_msg);

		// Verify that there is only one message in the circular buffer
		//assert(num_bfr_msgs == 1);
		// Verify that the received response is as expected
		//assert(strcmp(bus_data_msg, "$S01G,A,37,1,11*61") == STR_CMP_MATCHES_VALUE);
	}
}


void loop() {
}