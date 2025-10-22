#include <WiFi.h>
#include <HTTPClient.h>

#define RXD2 33
#define TXD2 4

//const char* ssid = "ESP32-Access-Point";
//const char* password = "123456789";

const char* ssid = "MyInternet";
const char* password = "1234567";


//Your IP address or domain name with URL path
const char* serverNameTemp = "http://10.0.0.24/motorVal";

#include <Wire.h>

String controlValue;
unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //  unsigned long currentMillis = millis();

  //  if(currentMillis - previousMillis >= interval) {
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    controlValue = httpGETRequest(serverNameTemp);
    Serial.println(controlValue);
    delay(100);
    Serial.println("mySerial: ");

    Serial2.print(controlValue);
    Serial2.println();
    delay(500);
    // save the last HTTP GET Request
    //      previousMillis = currentMillis;
  } else {
    Serial.println("WiFi Disconnected");
  }
  //  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}