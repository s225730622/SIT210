// TASK 1.1P
// Design a system to switch ON the porch light and the hallway light when Linda presses the switch/push button.
// The porch light should stay ON for 30 seconds and the hallway light on for 60 seconds. 

int button = 2;
int porchLight = 11;
int hallwayLight = 12;

void setup() {
  pinMode(button, INPUT);
  pinMode(porchLight, OUTPUT);
  pinMode(hallwayLight, OUTPUT);
}

void loop() {
  // If button is pressed ON 
  if (digitalRead(button) == LOW)
  {
    // Turn porchLightPin ON for 30 seconds, then turn OFF
    digitalWrite(porchLight, HIGH);
    delay(30000);
    digitalWrite(porchLight, LOW);

    // Turn hallwayLightPin on for 60 seconds, then turn OFF
    digitalWrite(hallwayLight, HIGH);
    delay(60000);
    digitalWrite(hallwayLight, LOW);
  }
}
