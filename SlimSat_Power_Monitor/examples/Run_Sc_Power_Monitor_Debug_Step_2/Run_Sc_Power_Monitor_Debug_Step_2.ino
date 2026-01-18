#include <Adafruit_TinyUSB.h>
#include <sc_power_monitor.h>
#include <sc_bus_data_struct.h>

class { }
// See! It's working both ways! So, it can't be this!
ScPowerMonitor ScPwrMon(0x45, 0x42, 0x40);
//ScPowerMonitor ScPwrMon;
ScBusData sc_bus_data;

void getScPowerSubsystemTlm(void) {
	for (uint8_t i=0; i<1; i++) {
    Serial.print(" ~ i is: ");
    Serial.println(i);
		getIna219Tlm(i);
    Serial.println("");
	}

  return;
}

void getIna219Tlm(uint8_t ina219_index) {

  Serial.print(F(" ~ Reading INA219_IF tlm for INA219: "));
  Serial.println(ina219_index);
	
	float sv = ScPwrMon.getShuntVoltage(ina219_index);
	float lv = ScPwrMon.getLoadVoltage(ina219_index);
	float bv = ScPwrMon.getBusVoltage(ina219_index);
	float bc = ScPwrMon.getCurrent(ina219_index);

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

  Serial.println("");
  
  return;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for serial to initialize

  Serial.println(" ~ Running S/C Bus Power Subsystem ... ");
  Serial.println("");

  ScPwrMon.print();
  ScPwrMon.initialize();
  //ScPwrMon.printScBusData();
  
  // Get the current data struct
  ScPwrMon.printScBusDataStruct(sc_bus_data);

  // Update the data struct by collecting new data
  ScPwrMon.collectScBusData(sc_bus_data);
  
  // Verify that it is updated
  ScPwrMon.printScBusDataStruct(sc_bus_data);
}


void loop() {
  // put your main code here, to run repeatedly:
  getScPowerSubsystemTlm();
  delay(2000);
}
