#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void setup() {
  Serial.begin(9600);
  if (!bno.begin()) {
    Serial.println("BNO055 not detected. Check wiring!");
    while (1);
  }
  bno.setExtCrystalUse(true);
  delay(1000);
}

void loop() {
  sensors_event_t event;
  bno.getEvent(&event);

  // Send as CSV: heading,roll,pitch
  Serial.print(event.orientation.x); // heading
  Serial.print(",");
  Serial.print(event.orientation.y); // roll
  Serial.print(",");
  Serial.println(event.orientation.z); // pitch

  delay(100); // send every 100ms
}
