#include <ArduinoJson.h>
#include <FastLED.h>

#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8
#define PIN_Motor_AIN_1 7
#define PIN_Motor_STBY 3
#define UPDATES_PER_SECOND 100

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);
  //right_flag = false;
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  //delay(300);
  //receive command
 
  if (Serial.available() > 0) {
    StaticJsonDocument<200> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, Serial);

    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    long direction = doc["direction"];
    long speed = doc["speed"];  

    Serial.println(direction);  //-- added for debugging
    Serial.println(speed);     //-- added for debugging
    send_data(direction, speed);
  }
}

void send_data(unsigned long motion, unsigned long speed) {
  digitalWrite(PIN_Motor_STBY, HIGH);
  Serial.println(motion);

  switch (motion) {
    case 0: /* constant-expression */
      //stop_it:
      analogWrite(PIN_Motor_PWMA, 0);
      digitalWrite(PIN_Motor_STBY, LOW);
      analogWrite(PIN_Motor_PWMB, 0);
      digitalWrite(PIN_Motor_STBY, LOW);

//      stop_indicator();
      break;
    case 1: /* constant-expression */
      // Forward :
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, speed);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, speed);
      break;
    case 2: /* constant-expression */
      //Complete Right:
      /* code */
      digitalWrite(PIN_Motor_AIN_1, LOW);
      analogWrite(PIN_Motor_PWMA, speed);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, speed);

//      right_indicator();
      break;
    case 3: /* constant-expression */
      //Complete Left:
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, speed);
      digitalWrite(PIN_Motor_BIN_1, LOW);
      analogWrite(PIN_Motor_PWMB, speed);

//      left_indicator();
      break;
    case 4: /* constant-expression */
      //Backward:
      digitalWrite(PIN_Motor_AIN_1, LOW);
      analogWrite(PIN_Motor_PWMA, speed);
      digitalWrite(PIN_Motor_BIN_1, LOW);
      analogWrite(PIN_Motor_PWMB, speed);

      break;
    case 5: /* constant-expression */
      //Right Forward:
      /* code */
      digitalWrite(PIN_Motor_AIN_1, LOW);
      analogWrite(PIN_Motor_PWMA, speed);
      digitalWrite(PIN_Motor_BIN_1, HIGH);
      analogWrite(PIN_Motor_PWMB, speed);
      break;
    case 6: /* constant-expression */
      //Left Forward:
      digitalWrite(PIN_Motor_AIN_1, HIGH);
      analogWrite(PIN_Motor_PWMA, speed);
      digitalWrite(PIN_Motor_BIN_1, LOW);
      analogWrite(PIN_Motor_PWMB, speed);
      break;
      
  }
}
