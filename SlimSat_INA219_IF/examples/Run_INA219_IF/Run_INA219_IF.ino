#include <Adafruit_TinyUSB.h>
#include <ina219_if.h>

// Construct the INA219 Object
Ina219If INA219_1(0x45);


void getIna219Tlm(void) {
  Serial.println(" ~ Reading INA219_IF Telemetry ...");
	
	float sv = INA219_1.getShuntVoltage();
	float lv = INA219_1.getLoadVoltage();
	float bv = INA219_1.getBusVoltage();
	float bc = INA219_1.getCurrent();
  float bp = INA219_1.getPower();

  Serial.print(" ~ Shunt Voltage is: ");
  Serial.print(sv);
  Serial.println(" V");

  Serial.print(" ~ Load Voltage is: ");
  Serial.print(lv);
  Serial.println(" V");
  
  Serial.print(" ~ Bus Voltage is: ");
  Serial.print(bv);
  Serial.println(" V");
  
  Serial.print(" ~ Current is: ");
  Serial.print(bc);
  Serial.println(" mA");
  
  Serial.print(" ~ Power is: ");
  Serial.print(bp);
  Serial.println(" mW\n");

  return;
}


void setup() {
  // Open a serial port with the CPU
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}
  Serial.println("\n ~ Running INA219_IF ...\n");

  // Print the INA219_IF object
  INA219_1.print();

  // Initialize the INA219 Device
  INA219_1.initializeDevice();

  return;
}


void loop() {
  // Get the INA219 Telemetry
  getIna219Tlm();
  delay(2000);

  return;
}
