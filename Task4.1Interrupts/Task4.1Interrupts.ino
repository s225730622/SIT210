// Task 4.1P - Handling Interrupts

// Define pins for LEDs, switch, and PIR sensor
const int switchPin = 3;
const int pirPin = 2;
const int led1 = 7;
const int led2 = 8;

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

  Serial.begin(9600);

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(switchPin), switchInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pirPin), sensorInterrupt, CHANGE);
}

void loop() {
  // Print lights on/lights off message depending on interrupt and state
  if (switchEvent) {
    // Reset the boolean trigger 
    switchEvent = false;

    if (switchState == LOW) {
      Serial.println("Lights switched ON");
      lightsOn();     // Turn LED lights ON
    }
    else {
      Serial.println("Lights switched OFF");
      lightsOff();    // Turn LED lights OFF
    }
  }
  if (sensorEvent) {
    // Reset the boolean trigger 
    sensorEvent = false;

    if (sensorState == HIGH) {
      Serial.println("Motion detected, lights ON");
      lightsOn();     // Turn LED lights ON
    }
    else {
      Serial.println("No motion detected, lights OFF");
      lightsOff();    // Turn LED lights OFF
    }
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
