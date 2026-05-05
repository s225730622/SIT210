// Task 4.2D - Calling Functions from the Web
#include <WiFiNINA.h>
#include <SPI.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

// WiFi credentials (assigned in the arduino_secrets tab)
char ssid[] = SECRET_SSID;    // Network SSID (name)
char pass[] = SECRET_PASS;    // Network password
WiFiServer server(80);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// MQTT Details
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* topic = "emily/lights";

// Define pins for LEDs
const int livingRoomLight = 10;
const int bathroomLight = 9;
const int closetLight = 8;

bool livingRoomState = false;
bool bathroomState = false;
bool closetState = false;

// Connect to WiFi
void connect_wifi() {
  Serial.println("Connecting to Wi-Fi..");
  while (WiFi.begin(SECRET_SSID, SECRET_PASS) != WL_CONNECTED) {
    delay(2000);
    Serial.println("Retrying Wi-Fi..");
  }
  Serial.println("Wi-Fi connected.");
  Serial.print("Remote Lights Control HTML Site: http://");
  Serial.println(WiFi.localIP());
  server.begin();
}

// MQTT Callback >> Runs when Arduino receives a message
void mqttCallback(char* receivedTopic, byte* payload, unsigned int length) {
  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("MQTT message received: ");
  Serial.println(message);

  if (message == "living room") {
    toggleLight("living room");
  }

  else if (message == "bathroom") {
    toggleLight("bathroom");
  }

  else if (message == "closet") {
    toggleLight("closet");
  }
}

// Connect to MQTT Broker
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");

    if (mqttClient.connect("ECCAr_duino#33")) {
      Serial.println("MQTT connected.");
      mqttClient.subscribe(topic);

      Serial.print("Subscribed to topic: ");
      Serial.println(topic);
    }
    else {
      Serial.print("MQTT failed, rc=");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while(!Serial);

  // Set pin modes
  pinMode(livingRoomLight, OUTPUT);
  pinMode(bathroomLight, OUTPUT);
  pinMode(closetLight, OUTPUT);

  digitalWrite(livingRoomLight, LOW);
  digitalWrite(bathroomLight, LOW);
  digitalWrite(closetLight, LOW); 

  // Connect to WiFi
  connect_wifi();

  // Setup MQTT after WiFi is connected
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);
  connectMQTT();
}

void loop() {
  // Keep MQTT connection alive
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  WiFiClient client = server.available();
  if (!client)
    return;

  String request = "";
  unsigned long timeout = millis();

  while (client.connected() && millis() - timeout < 1000) 
  {
    if (client.available()) 
    {
      char c = client.read();
      request += c;
      if (request.endsWith("\r\n\r\n"))
        break;
    }
  }

  Serial.println("Request received:");
  Serial.println(request);

  if (request.indexOf("GET /toggle?room=living%20room") >= 0)
    toggleLight("living room");
  
  if (request.indexOf("GET /toggle?room=bathroom") >= 0)
    toggleLight("bathroom");

  if (request.indexOf("GET /toggle?room=closet") >= 0)
    toggleLight("closet");


  client .println("HTTP/1.1 200 OK");
  client .println("Content-Type: text/html");
  client .println("Connection: close");
  client .println();
  client.println("<!DOCTYPE html>");
  client.println("<html><head><meta charset='UTF-8'><title>Remote Control of Lindas Lights</title></head><body>");
  client.println("<h1>Remote Control of Lindas Lights</h1>");
  client.println("<h4>Click button to switch lights on/off remotely</h4>");
  client.println("<p><a href='/toggle?room=living%20room'><button>Living Room</button></a></p>");
  client.println("<p><a href='/toggle?room=bathroom'><button>Toggle Bathroom</button></a></p>");
  client.println("<p><a href='/toggle?room=closet'><button>Toggle Closet</button></a></p>");
  client.println("<h2>Status</h2>");
  client.print("<p>Living Room: ");
  client.print(livingRoomState ? "ON" : "OFF");
  client.println("</p>");
  client.print("<p>Bathroom: ");
  client.print(bathroomState ? "ON" : "OFF");
  client.println("</p>");
  client.print("<p>Closet: ");
  client.print(closetState ? "ON" : "OFF");
  client.println("</p>");
  client.println("</body></html>");

  delay(1);
  client.stop();
}

void toggleLight(String room)
{
  if (room == "living room")
  {
    livingRoomState = !livingRoomState;
    digitalWrite(livingRoomLight, livingRoomState ? HIGH : LOW);
  }

  else if (room == "bathroom")
  {
    bathroomState = !bathroomState;
    digitalWrite(bathroomLight, bathroomState ? HIGH : LOW);
  }

  else if (room == "closet")
  {
    closetState = !closetState;
    digitalWrite(closetLight, closetState ? HIGH : LOW);
  }
}
