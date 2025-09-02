// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <slimsat_bus.h>
#include <assert.h>

#define STR_CMP_MATCHES_VALUE 0

char sc_ping_msg[] = "$GS01,1,*24";
char sc_reboot_msg[] = "$GS01,2,*27";
char sc_get_register_msg[] = "$GS01,3,1234*22";
char sc_set_register_msg[] = "$GS01,4,5678*2D";
char sc_get_frequency_msg[] = "$GS01,5,*20";
char sc_set_frequency_msg[] = "$GS01,6,438123456*1B";
char sc_get_bandwidth_msg[] = "$GS01,7,*22";
char sc_set_bandwidth_msg[] = "$GS01,8,30000*1E";
char sc_get_spread_factor_msg[] = "$GS01,9,*2C";
char sc_set_spread_factor_msg[] = "$GS01,10,4*20";
char sc_get_rssi_msg[] = "$GS01,11,*15";
char sc_get_beacon_period_msg[] = "$GS01,12,*16";
char sc_set_beacon_period_msg[] = "$GS01,13,33000*27";
char sc_get_sc_bus_mode_msg[] = "$GS01,14,*10";
char sc_set_sc_bus_mode_msg[] = "$GS01,15,1*20";
char sc_get_sc_bus_epoch_time_msg[] = "$GS01,16,*12";
char sc_set_sc_bus_epoch_time_msg[] = "$GS01,17,12345*22";
char sc_get_sc_bus_time_msg[] = "$GS01,18,*1C";
char sc_get_sc_bus_data_msg[] = "$GS01,19,*1D";	
char sc_get_sc_bus_stream_data_mode_msg[] = "$GS01,20,*17";
char sc_set_sc_bus_stream_data_mode_msg[] = "$GS01,21,1*27";
char sc_get_payload_op_period_msg[] = "$GS01,22,*15";
char sc_set_payload_op_period_msg[] = "$GS01,23,5000*11";
char sc_get_payload_mode_msg[] = "$GS01,24,*13";
char sc_get_payload_data_msg[] = "$GS01,25,*12";
char pl_1_ping_payload_msg[] = "$GS01,30,*16";
char pl_2_get_payload_mode_msg[] = "$GS01,31,*17";
char pl_3_get_num_meas_to_take_msg[] = "$GS01,32,*14";
char pl_4_set_num_meas_to_take_msg[] = "$GS01,33,16*12";
char pl_5_take_meas_msg[] = "$GS01,34,*12";
char pl_6_get_raw_meas_data_msg[] = "$GS01,35,*13";
char pl_7_get_processed_meas_data_msg[] = "$GS01,36,*10";
char pl_8_print_meas_data_msg[] = "$GS01,37,*11";
char pl_9_undefined_msg[] = "$GS01,38,*1E";
char pl_10_undefined_msg[] = "$GS01,39,*1F";
char sc_invalid_incorrect_checksum_msg[] = "$GS01,2,1000*AA";
char sc_invalid_cmd_id_msg[] = "$GS01,66,*15";
char sc_incorrect_slimsat_id_msg[] = "$GS02,2,1000*25";
char sc_invalid_partial_first_half_msg[] = "$GS01,2,";
char sc_invalid_partial_second_half_msg[] = ",1000*25";

// Construct the SlimSat Bus Object
SlimSatBus Slimsat;


void setup() {
  // Open a serial port for SlimSat communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F(" ~ Running: Test SlimSat S/C Bus Commands ... "));
}


void loop() {
	// Test all SlimSat commands

	uint8_t num_bfr_msgs = 0;
	char* sc_data_msg = NULL;
	
  Serial.print(F("\n ~ Sending a Ping Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_ping_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Ping Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,1,0,1*64") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Reboot Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_reboot_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Reboot Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,2,0,2*64") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Register Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_register_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Register Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected register value is the default register value of 10 
	assert(strcmp(sc_data_msg, "$S01G,A,3,10,3*55") == STR_CMP_MATCHES_VALUE);
  

  Serial.print(F("\n ~ Sending a Set Register Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_register_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Register Command, You still need one additional input parameter for this command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,4,5678,4*58") == STR_CMP_MATCHES_VALUE);
  

  Serial.print(F("\n ~ Sending a Get Register Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_register_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Register Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected register value is the value set by the set register cmd: 5678
	assert(strcmp(sc_data_msg, "$S01G,A,3,5678,5*5E") == STR_CMP_MATCHES_VALUE);
	

  Serial.print(F("\n ~ Sending a Get Radio Frequency Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_frequency_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Frequency Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected frequency value is the default fequency of 438E6
	assert(strcmp(sc_data_msg, "$S01G,A,5,438000000,6*68") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Radio Frequency Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_frequency_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Frequency Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,6,438123456,7*6D") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Radio Frequency Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_frequency_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Frequency Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected frequency is the value set by the set frequency cmd: 438123456
	assert(strcmp(sc_data_msg, "$S01G,A,5,438123456,8*61") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Radio Bandwidth Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_bandwidth_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Bandwidth Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected bandwidth value is the default bandwidth of 31000
	assert(strcmp(sc_data_msg, "$S01G,A,7,31000,9*68") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Radio Bandwidth Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_bandwidth_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Bandwidth Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,8,30000,10*5E") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Radio Bandwidth Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_bandwidth_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Bandwidth Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected bandwidth value is the value set by the set bandwidth cmd: 30000
	assert(strcmp(sc_data_msg, "$S01G,A,7,30000,11*50") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio Spread Factor Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_spread_factor_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected bandwidth value is the default bandwidth of 6
	assert(strcmp(sc_data_msg, "$S01G,A,9,6,12*58") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Radio Spread Factor Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_spread_factor_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Radio Spread Factor Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,10,4,13*63") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Get Radio Spread Factor Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_spread_factor_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio Spread Factor Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected spread factor value is the value set by the set spread factor cmd: 30000
	assert(strcmp(sc_data_msg, "$S01G,A,9,4,14*5C") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get Radio RSSI Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_rssi_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Radio RSSSI Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected RSSI value is the default value of 1
	assert(strcmp(sc_data_msg, "$S01G,A,11,1,15*61") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Beacon Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_beacon_period_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Beacon Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected period value is the default period of 2000
	assert(strcmp(sc_data_msg, "$S01G,A,12,2000,16*52") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Beacon Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_beacon_period_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Beacon Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,13,33000,17*60") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Beacon Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_beacon_period_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Beacon Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected beacon period value is the value set by the set beacon period cmd: 33000
	assert(strcmp(sc_data_msg, "$S01G,A,12,33000,18*6E") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get S/C Bus Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_sc_bus_mode_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get S/C Bus Mode Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected S/C Bus Mode value is the default S/C Bus Mode of 0
	assert(strcmp(sc_data_msg, "$S01G,A,14,0,19*69") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set S/C Bus Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_sc_bus_mode_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set S/C Bus Mode Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,15,1,20*63") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get S/C Bus Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_sc_bus_mode_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get S/C Bus Mode Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected S/C Bus Mode value is the value set by the set S/C Bus Mode cmd: 1
	assert(strcmp(sc_data_msg, "$S01G,A,14,1,21*63") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get S/C Bus Epoch Time Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_sc_bus_epoch_time_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get S/C Bus Time Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected S/C Bus Time value is the default S/C Bus Time of 0
	assert(strcmp(sc_data_msg, "$S01G,A,16,0,22*63") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set S/C Bus Epoch Time Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_sc_bus_epoch_time_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set S/C Bus Time Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,17,12345,23*62") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get S/C Bus Epoch Time Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_sc_bus_epoch_time_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get S/C Bus Time Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected S/C Bus Epoch Time value is the value set by the set S/C Bus Epoch Time cmd: 12345
	assert(strcmp(sc_data_msg, "$S01G,A,16,12345,24*64") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Get S/C Bus Time Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_sc_bus_time_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get S/C Bus Time Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected S/C Bus Time value is the default S/C Bus Time of 0
	assert(strcmp(sc_data_msg, "$S01G,A,18,0,25*6A") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get S/C Bus Data Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_sc_bus_data_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get S/C Bus Data Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected S/C Bus Data value is the default S/C Bus Data of 0
	assert(strcmp(sc_data_msg, "$S01G,A,19,0,26*68") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get S/C Bus Stream Data Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_sc_bus_stream_data_mode_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected S/C Bus Stream Data Mode value is the default Mode Value of 0
	assert(strcmp(sc_data_msg, "$S01G,A,20,0,27*63") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set S/C Bus Stream Data Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_sc_bus_stream_data_mode_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Payload Op Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,21,1,28*6C") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get S/C Bus Stream Data Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_sc_bus_stream_data_mode_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected S/C Bus Stream Data Mode value is the value set by the set S/C Bus Stream Data Mode cmd: 1
	assert(strcmp(sc_data_msg, "$S01G,A,20,1,29*6C") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Payload Op Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_payload_op_period_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected period value is the default period of 2000
	assert(strcmp(sc_data_msg, "$S01G,A,22,2000,30*55") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Set Payload Op Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_set_payload_op_period_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Set Payload Op Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,23,5000,31*52") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Payload Op Period Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_payload_op_period_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Op Period Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected period value is the value set by the set period cmd: 5000
	assert(strcmp(sc_data_msg, "$S01G,A,22,5000,32*50") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Payload Mode Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_payload_mode_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Mode Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Payload Mode value is the default Payload Mode value of 0
	assert(strcmp(sc_data_msg, "$S01G,A,24,0,33*62") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Get Payload Data Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_get_payload_data_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Get Payload Data Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Payload Data value is the default Payload Data value of 0
	assert(strcmp(sc_data_msg, "$S01G,A,25,0,34*64") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending a Payload 1 (Ping) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_1_ping_payload_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,30*57") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Payload 2 (Get Payload State) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_2_get_payload_mode_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Payload Mode value is the default Payload Mode value of 0
	assert(strcmp(sc_data_msg, "$S01G,A,31,0*4A") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Payload 3 (Get Number Measurements to Take) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_3_get_num_meas_to_take_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Number of Measurements to Take value is the default value of 1
	assert(strcmp(sc_data_msg, "$S01G,A,32,1*48") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Payload 4 (Set Number Measurements to Take) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_4_set_num_meas_to_take_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,33,16*7F") == STR_CMP_MATCHES_VALUE);


	// For verification, check the value set
	Serial.print(F("\n ~ Sending a Second Payload 3 (Get Number Measurements to Take) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_3_get_num_meas_to_take_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// The expected Number of Measurements to Take value is the value set by the set Number of Measurements to Take cmd: 16
	assert(strcmp(sc_data_msg, "$S01G,A,32,16*7E") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Payload 5 (Take Measurements) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_5_take_meas_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,34*53") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending a Payload 6 (Get Raw Measurements Data) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_6_get_raw_meas_data_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();

	// Verify the circular buffer has the expected number of messages
	assert((2 <= num_bfr_msgs) && (num_bfr_msgs <= 5));

	for (uint8_t j=0; j<num_bfr_msgs; j++) {
		sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
		Serial.println(sc_data_msg);
	}

	
	Serial.print(F("\n ~ Sending a Payload 7 (Get Processed Measurements) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_7_get_processed_meas_data_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();

	// Verify the circular buffer has the expected number of messages
	assert(num_bfr_msgs == 2);

	for (uint8_t j=0; j<num_bfr_msgs; j++) {
		sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
		Serial.println(sc_data_msg);
	}


	Serial.print(F("\n ~ Sending a Payload 8 (Print Measurements) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_8_print_meas_data_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,37*50") == STR_CMP_MATCHES_VALUE);
	

	Serial.print(F("\n ~ Sending a Payload 9 (Not Yet Defined) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_9_undefined_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,38*5F") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending a Payload 10 (Not Yet Defined) Command ..."));
  Slimsat.Msg_handler.handleNmeaMsg(pl_10_undefined_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send a Payload Command
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,A,39*5E") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending an Invalid Command (Incorrect Checksum) ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_invalid_incorrect_checksum_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (invalid checksum)
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,N,0,0*77") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending an Invalid Command (Invalid Cmd ID)..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_invalid_cmd_id_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (Invalid Cmd #)
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,N,0,0*77") == STR_CMP_MATCHES_VALUE);


  Serial.print(F("\n ~ Sending an Invalid Command (Incorrect SlimSat ID) ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_incorrect_slimsat_id_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (Invalid Listener ID)
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,N,0,0*77") == STR_CMP_MATCHES_VALUE);


	Serial.print(F("\n ~ Sending an Invalid Command (Partial Message, 1st half only) ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_invalid_partial_first_half_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (Partial Command, 1st half)
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,N,0,0*77") == STR_CMP_MATCHES_VALUE);

	
	Serial.print(F("\n ~ Sending an Invalid Command (Partial Message, 2nd half only) ..."));
  Slimsat.Msg_handler.handleNmeaMsg(sc_invalid_partial_second_half_msg, Slimsat.Sc_database, Slimsat.Payload1, Slimsat.LoRa, Slimsat.Payload_timer, Slimsat.Beacon_timer); //Send Invalid Command (Partial Command, 2nd half)
	Serial.print(F(" ~ Received response from S/C: "));
	num_bfr_msgs = Slimsat.Msg_handler.Sc_cbfr.getCircularBufferNumMsgs();
	sc_data_msg = Slimsat.Msg_handler.Sc_cbfr.circularBufferPopMsg();
	Serial.println(sc_data_msg);

	// Verify that there is only one message in the circular buffer
	assert(num_bfr_msgs == 1);
	// Verify that the received response is as expected
	assert(strcmp(sc_data_msg, "$S01G,N,0,0*77") == STR_CMP_MATCHES_VALUE);


	Serial.println(F("\n ~ Congrats, all SlimSat Cmd Tests: PASS!\n"));
	
  delay(30000);
}
