// CAPE-Twiggs HSSSI-26 SlimSat Project
// August 30, 2025
// Copyright (c) 2025, Eric Tapio. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Wire.h>
#include <M2M_LM75A.h>
#include <Adafruit_TinyUSB.h>

// 0x48 is the default LM75A address, and must specify otherwise
#define I2C_ADR 0x4B
//#define I2C_ADR 0x4F

// Construct the LM75A Object
M2M_LM75A lm75a(I2C_ADR);

void setup() {
	// Open a serial port to the CPU
  Serial.begin(115200);
  while (!Serial) {
		delay(10);   
	}

  Serial.println("\n ~ Running M2M_LM75A Verification and Validation Test ...\n");

  // Start the LM75A
  lm75a.begin();

  return;
}


void loop() {
  Serial.print(F(" ~ Is Connected is: "));
  Serial.println(lm75a.isConnected());

  Serial.print(F(" ~ Config Register Value is: "));
  Serial.println(lm75a.getConfig());
  
  // Temperature
  Serial.print(F(" ~ Temperature in Celsius: "));
  Serial.print(lm75a.getTemperature());
  Serial.println(F(" *C"));

  Serial.print(F(" ~ Temperature in Farenheit: "));
  Serial.print(lm75a.getTemperatureInFarenheit());
  Serial.println(F(" *F"));

  Serial.print(F(" ~ Hysteris temperature: "));
  Serial.print(lm75a.getHysterisisTemperature());
  Serial.println(F(" *C"));

  Serial.print(F(" ~ OS trip temperature: "));
  Serial.print(lm75a.getOSTripTemperature());
  Serial.println(F(" *C"));

  // Shutdown/Wake up
  Serial.println(F(" ~ Shutting down"));
  lm75a.shutdown();

  Serial.print(F(" ~ Is shutdown: "));
  Serial.println(lm75a.isShutdown());
  delay(5000);

  Serial.println(F(" ~ Waking up"));
  lm75a.wakeup();
  Serial.print(F(" ~ Is shutdown: "));
  Serial.println(lm75a.isShutdown());
  Serial.println();  
  delay(1000);

  Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
  Serial.println();
  
  delay(1000);
}
