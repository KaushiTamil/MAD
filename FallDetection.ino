#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Thresholds (Adjust based on testing)
const float freefall_threshold = 4.0;  // Lower than 9.8 m/s^2 total acceleration
const float impact_threshold = 25.0;   // High acceleration spike
const int post_fall_wait = 3000;       // Wait 3 seconds to check for movement

void setup(void) {
  Serial.begin(115200);
  if(!bno.begin()) { Serial.println("Check Wiring!"); while(1); }
  bno.setExtCrystalUse(true);
}

void loop(void) {
  // 1. Get Linear Acceleration (excluding gravity)
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  float mag = sqrt(acc.x()*acc.x() + acc.y()*acc.y() + acc.z()*acc.z());

  // 2. Detect Impact
  if (mag > impact_threshold) {
    Serial.println("Potential Fall Detected! Monitoring for recovery...");
    delay(post_fall_wait); // Give the user time to settle
    
    if (isUserDown()) {
      triggerAlarm();
    }
  }
}

bool isUserDown() {
  sensors_event_t event;
  bno.getEvent(&event);
  
  // Check Pitch or Roll. If the device was vertical (0°) and is now horizontal (~90°)
  // Adjust these values based on how the sensor is mounted on the body
  float pitch = abs(event.orientation.y); 
  if (pitch > 60.0) { 
    return true; 
  }
  return false;
}

void triggerAlarm() {
  Serial.println("ALERT: Fall Confirmed - No recovery detected!");
  // Add code here to trigger a piezo buzzer or send a message via Bluetooth/GSM
}
