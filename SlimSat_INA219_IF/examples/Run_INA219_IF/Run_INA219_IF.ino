// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// This sketch file runs the INA219 Interface code
// It may be used to verify that the system INA219 sensor(s) are functioning properly
// Note, this sketch files only exercises an individual INA219
#include <Adafruit_TinyUSB.h>
#include <ina219_if.h>

#define INA_1_ADR 0x45
#define INA_2_ADR 0x41
#define INA_3_ADR 0x40

// Construct the INA219 Object with the INA219 I2C address 
Ina219If INA219_1(INA_1_ADR); 


void getIna219Tlm(void) {
  // This function gets and displays the INA219 telemetry
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

  // Initialize the INA219 Device
  INA219_1.initializeDevice();

  // Print the INA219_IF object
  INA219_1.print();

  return;
}


void loop() {
  // Get the INA219 Telemetry
  getIna219Tlm();
  delay(2000);

  return;
}