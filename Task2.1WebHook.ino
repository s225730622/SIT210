// Task 2.1P - Sending Temperature and Light Data to the Web
#include <DHT.h>
#include <BH1750.h>
#include <Wire.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h" // Include thingspeak header file after other header files and custom macros

#define DHTPIN 2
#define DHTTYPE DHT11

char ssid[] = SECRET_SSID;   // Network SSID (name) 
char pass[] = SECRET_PASS;   // Network password
int keyIndex = 0;            // Network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize DHT sensor & BH1750 light meter
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("DHT11 & BH1750 Test!"));

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // Don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv != "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  dht.begin();
  Wire.begin();
  lightMeter.begin();
  ThingSpeak.begin(client); 
}

void loop() 
{
  // Call collectTempLightData to collect temperature and light data and send to ThingSpeak
  collectTempLightData();
}

void collectTempLightData()
{
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // Call readTemp method to request temperature reading
  float t = readTemp();
  // Call lightRead method to request light reading
  float lux = readLight();

  // Print temperature/light readings
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C"));
  Serial.print(" Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  // Set the fields with the values
  ThingSpeak.setField(1, lux);
  ThingSpeak.setField(2, t);

  // Write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(30000); // Wait 30 seconds to update the channel again
}

float readTemp()
{
  // Read Temperature Sensor
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return NAN;
  }
  // Return recorded t value
  return t;
}

float readLight()
{
  // Read Light Meter
  float lux = lightMeter.readLightLevel();
  // Return recorded lux value
  return lux;
}



