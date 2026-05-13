#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Raspberry Pi IP
const char* mqttServer = "192.168.0.21";

// Pins
const int buzzerPin = 9;          // Buzzer pin
const int sensorEchoPin = 2;      // Sensor pin (ECHO) 
const int sensorTrigPin = 3;      // Sensor pin (TRIG) 
const int yellowLedPin = 4;       // Yellow LED pin 
const int redLedPin = 5;          // Red LED pin

void connect_wifi() {
  Serial.println("Connecting to Wi-Fi..");
  while (WiFi.begin(SECRET_SSID, SECRET_PASS) != WL_CONNECTED) {
    delay(2000);
    Serial.println("Retrying Wi-Fi..");
  }
  Serial.println("Wi-Fi connected.");
}

// Connect to MQTT
void connect_mqtt() {
  if (mqttClient.connected()) {
    return;
  }

  Serial.println("Attempting MQTT connection..");

  if (mqttClient.connect("ArduinoClient")) {
    Serial.println("MQTT connected.");
  } 
  else {
    Serial.print("MQTT failed. State: ");
    Serial.println(mqttClient.state());
  }
}

void setup() {

  // Start Serial Monitor
  Serial.begin(9600);

  // Pin Modes
  pinMode(buzzerPin, OUTPUT);
  pinMode(sensorEchoPin, INPUT);
  pinMode(sensorTrigPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  connect_wifi();

  mqttClient.setServer(mqttServer, 1883);
  connect_mqtt();
}


void loop() {
  connect_mqtt();
  mqttClient.loop();
  
  // Declare distance variable by calling readDistance method
  int distance = readDistance();
  Serial.print("Distance reading: ");
  Serial.println(distance);

  // Ensure LEDs & buzzer are reset each iteration
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(redLedPin, LOW);
  noTone(buzzerPin);

  String status = "No Object Detected.";
  
  if (distance > 0 && distance <= 15) {
    tone(buzzerPin, 1000);
    digitalWrite(redLedPin, HIGH);
    status = "PROXIMITY DANGER";
  }
  else if (distance > 15 && distance <= 30) {
    tone(buzzerPin, 600);
    digitalWrite(yellowLedPin, HIGH);
    status = "PROXIMITY WARNING";
  }
  else if (distance > 30) {
    status = "SAFE PROXIMITY";
  }

  // Send this data to the Raspberry Pi
  Serial.print(status);
  Serial.print(", ");
  Serial.println(distance);

  // Publish distance
  char distanceMsg[10];
  sprintf(distanceMsg, "%d", distance);
  
  if (mqttClient.connected()) {
  mqttClient.loop();

    mqttClient.publish("vision/distance", distanceMsg);
    mqttClient.publish("vision/status", status.c_str());
  }

  delay(2000);

}

// Ultrasonic Sensor measures distance
int readDistance() {
  // Trigger the sensor to send out a pulse
  digitalWrite(sensorTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensorTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensorTrigPin, LOW);

  // Measure the duration of the pulse
  long duration = pulseIn(sensorEchoPin, HIGH, 30000);    // LEDs have a 30ms timeout
  if (duration == 0) {
    return 0;
  }

  // Calculate distance (in cm)
  return duration * 0.034 / 2;
}
