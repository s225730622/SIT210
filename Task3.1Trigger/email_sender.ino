#include "arduino_secrets.h"
#include <WiFiNINA.h>
#include <BH1750.h>

//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;        // Network SSID (name)
char pass[] = SECRET_PASS;        // Network password (use for WPA, or use as key for WEP)

WiFiClient client;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/sketch_received/with/key/BDdfOQOwAwKA_e-QR5Kgr";      // Change your EVENT-NAME and YOUR-KEY
String queryString = "?value1=57&value2=25";
BH1750 lightMeter;

void setup() 
{
  // Initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial);

  // Connect to web server on port 80:
  if (client.connect(HOST_NAME, 80))
  {
    // If connected:
    Serial.println("Connected to server");
  }
  else 
  {
    // If not connected:
    Serial.println("Connection failed");
  }

  lightMeter.begin();

}

void loop() 
{
  if (Serial.read() == 's')
  {
    // Make a HTTP request:
    // Send HTTP header
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();  // End HTTP header

    while (client.connected())
    {
      if (client.available())
      {
        // Read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }
    // The server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("Disconnected");
  }
}




