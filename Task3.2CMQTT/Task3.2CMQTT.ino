#include <PubSubClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

// Wifi credentials (assigned in the arduino_secrets tab)
char ssid[] = SECRET_SSID;    // Network SSID (name)
char pass[] = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)
char user[] = SECRET_USER;    // Username to connect to MQTT client
char pword[] = SECRET_PWORD;  // Password to connect to MQTT client

// MQTT broker details
const char* host = "broker.emqx.io";
const int port = 1883;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Message variables
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (500)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Define pin connections
const int trigPin = 12;
const int echoPin = 11;
const int grnLedPin = 8;
const int redLedPin = 7;

// Set boolean variables for states
boolean greenLedON = false;
boolean redLedON = false;
boolean sensorDetected = false;

// Set a distance threshold for detecting movement (in cm)
const int movementThreshold = 30; // Adjust this to any desired distance/

// Connect to WiFi
void connect_wifi() {
  Serial.println("Connecting to Wi-Fi..");
  while (WiFi.begin(SECRET_SSID, SECRET_PASS) != WL_CONNECTED) {
    delay(2000);
    Serial.println("Retrying Wi-Fi..");
  }
  Serial.println("Wi-Fi connected.");
}

void setup () {
  // Start Serial Monitor
  Serial.begin(9600);

  // Initialize pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(grnLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  while (!Serial);
  
  connect_wifi();
  // Connect to EMQX MQTT broker
  client.setServer(host, port);
  client.setCallback(callback);

  Serial.println("Starting distance sensor..");
}

void loop () {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int distance = readDistance(); 

  // System detects wave and turns both LEDs on, then publishes a message to ES/Wave containing my name
  if (distance > 0 && distance <= movementThreshold && !sensorDetected) {
    sensorDetected = true;
    client.publish("ES/Wave", "Emily");
    lightsON();
  }
  // System detects pat and turns both LEDs off, then publishes a message to ES/Pat containing my name
  else if (distance > 0 && distance > movementThreshold && sensorDetected) {
    sensorDetected = false;
    client.publish("ES/Pat", "Emily");
    lightsOFF();
  }

  // Delay to avoid overwhelming the sensor
  delay(200);
}

int readDistance() {
  // Trigger the sensor to send out a pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the pulse
  long duration = pulseIn(echoPin, HIGH, 30000);    // LEDs have a 30 second timeout
  if (duration == 0) {
    return 0;
  }

  // Calculate distance (in cm)
  return duration * 0.034 / 2;
}

// Turns both LED lights ON
void lightsON() {
  digitalWrite(grnLedPin, HIGH);  // Turn on the green LED
  greenLedON = true;
  digitalWrite(redLedPin, HIGH);  // Turn on the green LED
  redLedON = true;
}

// Turns both LED lights OFF
void lightsOFF() {
  digitalWrite(grnLedPin, LOW);  // Turn off the green LED
  greenLedON = false;
  digitalWrite(redLedPin, LOW);  // Turn off the green LED
  redLedON = false;
}

// Publish message to MQTT to the given topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch LED on if the first character is present
  if (strcmp(topic, "ES/Wave") == 0) {
    lightsON();
  }
  else if (strcmp(topic, "ES/Pat") == 0) {
    lightsOFF();
  }
}

// Establish MQTT connection and sends message
void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection..");
    String clientId = "Arduino_Nano33-_";
    // Attempt to connect
    // Insert password
    if (client.connect(clientId.c_str(), SECRET_USER, SECRET_PWORD)) {
      Serial.println("Connected.");
      // Once connected, publish announcement
      client.subscribe("ES/Wave");
      client.subscribe("ES/Pat");
    }
    else {
      Serial.print("Failed, rc = ");
      Serial.print(client.state());
      Serial.println(" Try again in 5 seconds.");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
