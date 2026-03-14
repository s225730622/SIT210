#include <BH1750.h>
#include <Wire.h>
#include <WiFiNINA.h>
#include "Secret.h"

// Sensitive data is kept in Secret.h
char ssid[] = SECRET_SSID;  // Network SSID (name)
char pass[] = SECRET_PASS;  // Network password (use for WPA, or use as key for WEP)
WiFiClient client;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME_INSL = "/trigger/terrarium_in_sunlight/with/key/BDdfOQOwAwKA_e-QR5Kgr";
String PATH_NAME_NOTINSL = "/trigger/terrarium_notin_sunlight/with/key/BDdfOQOwAwKA_e-QR5Kgr";
String queryString = "?value1=57&value2=25";

// Initialize BH15720 light meter
BH1750 lightMeter;
// Initialize lux threshold level which detects sunlight
float luxSunlightLvl = 400;
// Set some boolean variables for in/out of sunlight which trigger a notification
bool inSunlight = false;
bool sunlightStops = false;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();

  // If Wifi doesn't connect, keep trying every 5 seconds
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) 
  {
    Serial.println("WiFi connection failed..");
    Serial.println("Trying again in 5 seconds..");
    delay(5000);
  }
  // Otherwise print connection success message
  Serial.println("WiFi connected ✔️");
}

void loop() 
{
  // Call readLight method to request light sensor reading and print to serial monitor
  float lx = readLight();
  Serial.print("Light(lx) level: ");
  Serial.print(lx);
  Serial.println(" lx");

  // If lx is at or above the threshold level that detects sunlight, trigger HTTP request is sent
  if (lx >= luxSunlightLvl && inSunlight == false)
  {
    isInSunlight();
  }
  // Once the lx level drops back below the sunlight threshold level, another trigger HTTP request is sent
  if (lx < luxSunlightLvl && sunlightStops == true)
  {
    isOutOfSunlight();
  }

  // Wait 5 seconds for next reading
  delay(5000);
}

// Method to read light sensor lx data
float readLight() 
{
  // Read light meter and return value
  return lightMeter.readLightLevel();
}

// Method that is called if lx level is equal to or above the sunlight threshold
void isInSunlight()
{
  // If light level is over threshold number, send HTTP request
  Serial.println("Terrarium light sensor has detected sunlight!");

  // Connect to web server
  if (client.connect(HOST_NAME, 80)) 
  {
    Serial.println("Connected to server ✔️");

    // Create and send Maker service a HTTP request notifying them of sunlight event
    client.println("GET " + PATH_NAME_INSL + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    Serial.println("You'll be notified when sunlight is no longer detected by sensor.");

    while (client.connected()) 
    {
      if (client.available()) 
      {
        // Read an incoming byte from the server and print it to serial monitor
        char c = client.read();
      }
    }

    // The server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("Server disconnected.. ");
  }
  else
  {
    Serial.println("Failed to connect to server!");
  }
  inSunlight = true;
  sunlightStops = true;
}

// Method that is called if lx level drops below the sunlight threshold
void isOutOfSunlight()
{
  // If light level is below threshold number, send HTTP request
  Serial.println("Terrarium light sensor is NO LONGER detecting sunlight.");

  // Connect to web server
  if (client.connect(HOST_NAME, 80)) 
  {
    Serial.println("Connected to server ✔️");

    // Create and send Maker service a HTTP request notifying them of sunlight event
    client.println("GET " + PATH_NAME_NOTINSL + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    Serial.println("You'll be notified when sunlight is detected by sensor.");

    while (client.connected()) 
    {
      if (client.available()) 
      {
        // Read an incoming byte from the server and print it to serial monitor
        char c = client.read();
      }
    }

    // The server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("Server disconnected.. ");
  }
  else
  {
    Serial.println("Failed to connect to server!");
  }
  sunlightStops = false;
  inSunlight = false;
}
