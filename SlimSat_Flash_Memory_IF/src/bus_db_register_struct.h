//
// Bus Database Register Struct
//
// CAPE-Twiggs HSSSI-26 SlimSat Project
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef BUS_DATABASE_REGISTER_HEADER
#define BUS_DATABASE_REGISTER_HEADER

struct BusDbRegisterValues {
	uint32_t boot_count;
	uint32_t num_valid_msgs;
	uint32_t num_invalid_msgs;
	uint32_t num_bus_data_recs;
	uint32_t next_bus_data_write_to_adr;
	uint32_t num_payload_data_recs;
	uint32_t next_payload_data_write_to_adr;
		
	// Default constructor
    BusDbRegisterValues(void) {
	boot_count = 0;
	num_valid_msgs = 0;
	num_invalid_msgs = 0;
	num_bus_data_recs = 0;
	next_bus_data_write_to_adr = 0;
	num_payload_data_recs = 0;
	next_payload_data_write_to_adr = 0;
    }	
};

#endif