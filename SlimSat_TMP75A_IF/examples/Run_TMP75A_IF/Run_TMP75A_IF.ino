#include <Adafruit_TinyUSB.h>
#include <tmp75a_if.h>

// Construct the TMP7A IF object
Tmp75aIf tmp75a_1;


void setup() {
	// Open a serial port to the CPU
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

  Serial.println("\n ~ Running TMP75A IF ...\n");

  // Start th TMP75A sensor
  bool success = tmp75a_1.begin();
  if (success) {
    Serial.println(" ~ TMP75A started.");
  }
  else {
    Serial.println(" ~ Error: TMP75A was not started!");
  }

  // Print the object
  tmp75a_1.print();

  Serial.print("\n ~ TMP75A is Connected is: ");
  Serial.println(tmp75a_1.isConnected());

  return;
}


void loop() {
  // Get the measured temperature
  Serial.print(" ~ Temp in Deg C is: ");
  Serial.println(tmp75a_1.getTemperatureInDegC());
  delay(1000);

  return;
}
