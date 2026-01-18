#include <Adafruit_TinyUSB.h>
#include <bus_data.h>

// Construct the Payload Data Object
BusDataRec bus_data_rec;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for serial to initialize

  Serial.println(" ~ Testing Bus Data Rec ... ");
  Serial.println();

  // Get the current data record
  bus_data_rec.printRecord();

  // Assign data record values
  bus_data_rec.time = 3456;
	bus_data_rec.bus_rec_number = 8;
	bus_data_rec.temp_C = 31.02;
	bus_data_rec.thermistor_V = 1.84;
	bus_data_rec.shunt_voltage_mV_1 = 3.21;
	bus_data_rec.bus_voltage_V_1 = 4.56;
	bus_data_rec.load_voltage_V_1 = 5.67;
	bus_data_rec.current_mA_1 = 8.90;
	bus_data_rec.shunt_voltage_mV_2 = 2.34;
	bus_data_rec.bus_voltage_V_2 = 3.45;
	bus_data_rec.load_voltage_V_2 = 7.65;
	bus_data_rec.current_mA_2 = 5.43;
	bus_data_rec.shunt_voltage_mV_3 = 4.32;
	bus_data_rec.bus_voltage_V_3 = 3.21;
	bus_data_rec.load_voltage_V_3 = 2.10;
	bus_data_rec.current_mA_3 = 1.09;

  // Print the updated record
  bus_data_rec.printRecord();
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
