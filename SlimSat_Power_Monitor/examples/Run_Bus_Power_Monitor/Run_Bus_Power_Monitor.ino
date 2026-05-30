// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// This sketch file runs the Bus Power Monitor code
// It may be used to verify that the system Power Monitor sensors are functioning properly
// This sketch reads the following bus power monitor sensors:
//   1) All three INA219 sensors
//   2) One TMP75A sensor
//   3) One thermistor input on Pin A0

#include <Adafruit_TinyUSB.h>
#include <bus_power_monitor.h>

#define INA_1_ADR 0x45
#define INA_2_ADR 0x41
#define INA_3_ADR 0x40
#define TMP_ADR 0x4B
#define NUM_INAS_USED 3

// Construct the Bus Power Monitor object
//BusPowerMonitor BusPwrMon(INA_1_ADR, INA_2_ADR, INA_3_ADR); // Specific constructor using only the specified INA I2C addresses
BusPowerMonitor BusPwrMon(INA_1_ADR, INA_2_ADR, INA_3_ADR, TMP_ADR); // Specific constructor using both the specified INA & TMP I2C addresses


void getPowerMonitorTelemetry(void) {
  getInaTelemetry();
  getTmpTlm();
  getThermistorTlm();

  return;
}


void getTmpTlm(void) {
  Serial.println(F(" ~ Reading TMP75 temperature tlm ..."));
  float temperature = BusPwrMon.getTemperature();
  Serial.print(" ~ Temperature is: ");
  Serial.print(temperature);
  Serial.println(" deg. C");

  return;  
}


void getThermistorTlm(void) {
  Serial.println(F(" ~ Reading Thermistor tlm ..."));
  float analog_value = BusPwrMon.readThermistor();
  Serial.print(" ~ Thermistor value is: ");
  Serial.print(analog_value);
  Serial.println(" V");
  Serial.println();

  return;  
}


void getInaTelemetry(void) {
	for (uint8_t i=0; i<NUM_INAS_USED; i++) {
    Serial.print(" ~ i is: ");
    Serial.println(i);
		getIna219Tlm(i);
    Serial.println();
	}

  return;
}

void getIna219Tlm(uint8_t ina219_index) {

  Serial.print(F(" ~ Reading INA219_IF tlm for INA219: "));
  Serial.println(ina219_index);
	
	float sv = BusPwrMon.getShuntVoltage(ina219_index);
	float lv = BusPwrMon.getLoadVoltage(ina219_index);
	float bv = BusPwrMon.getBusVoltage(ina219_index);
	float bc = BusPwrMon.getCurrent(ina219_index);

  Serial.print(" ~ Shunt Voltage is: ");
  Serial.print(sv);
  Serial.println(" V");

  Serial.print(" ~ Load Voltage is: ");
  Serial.print(lv);
  Serial.println(" V");
  
  Serial.print(" ~ Bus Voltage is: ");
  Serial.print(bv);
  Serial.println(" V");
  
  Serial.print(" ~ Bus Current is: ");
  Serial.print(bc);
  Serial.println(" mA");

  Serial.println();
  
  return;
}


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial to initialize
  }

  Serial.println(" ~ Running Bus Power Monitor ... ");
  Serial.println("");

  BusPwrMon.start();
  BusPwrMon.print();
  
  return;
}


void loop() {
  getPowerMonitorTelemetry();
  delay(2000);

  return;
}
