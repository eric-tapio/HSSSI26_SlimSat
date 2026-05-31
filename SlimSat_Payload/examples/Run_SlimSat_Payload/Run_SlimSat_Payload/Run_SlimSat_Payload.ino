#include <Adafruit_TinyUSB.h> // for Serial
#include <payload.h>

Payload Payload1; // Payload subsystem object
PlDataRec Payload_data_record; // Payload data record object

void setup() {
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

  Serial.println(F(" ~ Running Stand-alone Payload Test Program ..."));

  // Initialize the payload
  Serial.println(F(" ~ Initializing Payload ..."));
  Payload1.initializePayload();
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(F("\n ~ Looping ..."));

  // Perform a payload loop iteration
  Payload1.performPayloadLoopIteration(Payload_data_record);

  if (1) {
    // Print the entire payload data record
    //Payload_data_record.printRecord();

    // Print only the payload data array
    Payload_data_record.printArray();
  }

  // Pause a bit before running another loop iteration
  delay(5000);
}