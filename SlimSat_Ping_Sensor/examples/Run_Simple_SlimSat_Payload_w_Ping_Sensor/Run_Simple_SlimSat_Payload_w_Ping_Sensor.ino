// CAPE-Twiggs HSSSI-26 SlimSat Project
// Run Sample payload

// Adafruit TinyUSB is needed if running on the Adafruit ItsyBitsy. Otherwise it can be commented out 
#include <Adafruit_TinyUSB.h> // for Serial
#include <payload_data.h>
#include <simple_payload.h>

#define PAYLOAD_OP_PERIOD_IN_MS 2000

// Elaborate objects, including the Payload
PlDataRec PayloadDataRec;
Payload SimplePing;

void setup(void) {
	// Open a serial port for SlimSat SW Simulator communication
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

	Serial.println(F(" ~ Running Simple SlimSat Payload Example ... "));

	// Initialize the Payload
	SimplePing.initializePayload();

	return;
}


void loop(void) {
	// Infinite loop - This is where SlimSat operations gets performed
	SimplePing.performPayloadOperationIteration(PayloadDataRec);
	PayloadDataRec.printRecord();
	delay(PAYLOAD_OP_PERIOD_IN_MS);
}