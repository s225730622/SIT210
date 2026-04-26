// Task 4.1P - Handling Interrupts
#include <Wire.h>
#include <BH1750.h> 

// Initialize light meter
BH1750 lightMeter;
// Initialize dark environment threshold, leave a tiny bit of light so Linda is not in complete darkness
const float darkness = 0.5;

// Define pins for LEDs, switch and PIR sensor
const int switchPin = 3;
const int pirPin = 2;
const int led1 = 7;   // White LED
const int led2 = 8;   // Yellow LED

// Shared variables updated by interrupts
volatile uint8_t switchState = HIGH;    // HIGH = switch OFF, LOW = switch ON
volatile uint8_t sensorState = LOW;     // LOW = NO motion, HIGH = motion

// Initialize booleans for printing message to Serial that lights have turned on/off 
volatile bool switchEvent = false;
volatile bool sensorEvent = false;     

void setup() {
  // Set pin modes
  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);  
  pinMode(pirPin, INPUT);               
  pinMode(switchPin, INPUT_PULLUP);     // Inverts ON/OFF values

  Wire.begin();

  if (lightMeter.begin()) {
  Serial.println("BH1750 started");
  } else {
  Serial.println("BH1750 failed to start");
  } 

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(switchPin), switchInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pirPin), sensorInterrupt, CHANGE);
}

void loop() {
  float lightLvl = readLight(); 
  bool isDark = (lightLvl <= darkness);

  // Interrupt events to print respective messages
  if (sensorEvent) {
    // Reset the boolean trigger 
    sensorEvent = false;

    // Print appropriate message
    if (sensorState == HIGH && isDark) {
      Serial.println("Motion detected, lights ON");
    }
    else if ((sensorState == LOW && isDark) || !isDark) {
      Serial.println("No motion detected, lights OFF");
    }
  }

  if (switchEvent) {
    // Reset the boolean trigger 
    switchEvent = false;

    if (switchState == LOW) {
      Serial.println("Lights switched ON");
    }
    else {
      Serial.println("Lights switched OFF");
    }
  }

  // Turn lights on automatically if it is dark
  if ((isDark && sensorState == HIGH) || switchState == LOW) {
    lightsOn();     // Turn LED lights ON
  }
  else {
    lightsOff();    // Turn LED lights OFF
  } 

  delay(100);
}

// Interrupt called when switch changes state
void switchInterrupt() {
  switchState = digitalRead(switchPin);
  switchEvent = true;
}

// Interrupt called when PIR sensor changes state
void sensorInterrupt() {
  sensorState = digitalRead(pirPin);
  sensorEvent = true;
}

// Turn both lights on
void lightsOn() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
}

// Turn both lights off
void lightsOff() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}

float readLight() {
  float lux = lightMeter.readLightLevel();
  return lux;
}
