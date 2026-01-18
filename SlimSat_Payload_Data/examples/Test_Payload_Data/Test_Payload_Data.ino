#include <Adafruit_TinyUSB.h>
#include <payload_data.h>

// Construct the Payload Data Object
PlDataRec pl_data_rec;
double value = 0.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for serial to initialize

  Serial.println(" ~ Testing Payload Data Rec ... ");
  Serial.println();

  // Get the current data struct
  pl_data_rec.printRecord();

  for (uint8_t i=0; i<30; i++) {
    //Serial.println(i);
    pl_data_rec.setArrayElement(i, i*1.111);
  }

  pl_data_rec.printArray();


  for (uint8_t i=0; i<30; i++) {
    value = pl_data_rec.getArrayElement(i);
    Serial.println(value);
  }

  // Set and get the record time
  pl_data_rec.setTime(12);
  uint32_t time = pl_data_rec.getTime();
  Serial.println(time);

  pl_data_rec.setRecordNumber(20);
	uint16_t rec_number = pl_data_rec.getRecordNumber();
  Serial.println(rec_number);

  pl_data_rec.time = 22;
  Serial.println(pl_data_rec.time);

  pl_data_rec.pl_rec_number = 100;
  Serial.println(pl_data_rec.pl_rec_number);

}


void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
