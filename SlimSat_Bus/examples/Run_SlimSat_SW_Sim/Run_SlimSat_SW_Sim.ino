// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include "NRF52TimerInterrupt.h"
#include "NRF52_ISR_Timer.h"

#include <slimsat_bus.h>

#define DEFAULT_PAYLOAD_OP_PERIOD_IN_S 10
#define DEFAULT_BEACON_PERIOD_IN_S 1000
#define DEFAULT_BUS_DATA_RECORD_PERIOD_IN_S 20 
#define WAG_WDT_PERIOD_IN_MS 500L
#define NUM_MS_PER_S 1000

#define PAYLOAD_OP_PERIOD_IN_MS DEFAULT_PAYLOAD_OP_PERIOD_IN_S*NUM_MS_PER_S
#define BEACON_PERIOD_IN_MS DEFAULT_BEACON_PERIOD_IN_S*NUM_MS_PER_S
#define BUS_DATA_RECORD_PERIOD_IN_MS DEFAULT_BUS_DATA_RECORD_PERIOD_IN_S*NUM_MS_PER_S

#define VERBOSE_OUTPUT 0


// Define a global volatile int flag for wagging the WDT
volatile uint8_t wag_wdt_time = 0;

// Construct the SlimSat Bus
SlimSatBus Slimsat;

// Create the ISR Timer for wagging the WDT
NRF52Timer ITimer(NRF_TIMER_1);
NRF52_ISR_Timer ISR_Timer;


void startTimer() {
  ISR_Timer.run();
}


void wagWatchDog(void) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

	return;
}


void setup(void) {
	// Initialize the SlimSat SW Simulator

	// Open a serial port for SlimSat SW Simulator communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	if (VERBOSE_OUTPUT) {
		Serial.println(F(" ~ Running SlimSat SW Simulator ... "));
	}

  // configure pin in output mode
  pinMode(LED_BUILTIN, OUTPUT);

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(WAG_WDT_PERIOD_IN_MS * 1000, startTimer)) {
		if (VERBOSE_OUTPUT) {
    	Serial.print(F("Starting ITimer OK, millis() = "));
    	Serial.println(millis());
		}
  }
  else {
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));
	}

 ISR_Timer.setInterval(WAG_WDT_PERIOD_IN_MS,  wagWatchDog);

	// Set the payload and beacon Timer periods
	Slimsat.Payload_timer.setIntervalInMs(PAYLOAD_OP_PERIOD_IN_MS);
	Slimsat.Beacon_timer.setIntervalInMs(BEACON_PERIOD_IN_MS);
	Slimsat.Bus_timer.setIntervalInMs(BUS_DATA_RECORD_PERIOD_IN_MS);

	// Initialize the S/C Bus
	Slimsat.initializeBus();

	return;
}


void loop(void) {
	// Infinite loop - This is where SlimSat operations gets performed
	Slimsat.performBusOperationIteration();

	if (Slimsat.Payload_timer.timerHasElapsed()) {
		if (VERBOSE_OUTPUT) {
			Serial.println("\n ~ Performing Payload Op ...");
		}
		Slimsat.performPayloadOp();
	}

	if (Slimsat.Beacon_timer.timerHasElapsed()) {
		if (VERBOSE_OUTPUT) {
			Serial.println("\n ~ Transmitting Beacon Message ...");
		}
		Slimsat.transmitBeaconMessage();
	}

	if (Slimsat.Bus_timer.timerHasElapsed()) {
		if (VERBOSE_OUTPUT) {
			Serial.println("\n ~ Recording Bus Data ...");
		}
		Slimsat.recordBusData();
	}
}