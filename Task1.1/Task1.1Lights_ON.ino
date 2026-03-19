// TASK 1.1P
// Design a system to switch ON the porch light and the hallway light when Linda presses the switch/push button.
// The porch light should stay ON for 30 seconds and the hallway light on for 60 seconds. 

// Initialize any required variables and their respective pin locations
int button = 2;
int porchLight = 11;
int hallwayLight = 12;

void setup() {
  // Initialize button and pin modes as input/outputs
  pinMode(button, INPUT_PULLUP);
  pinMode(porchLight, OUTPUT);
  pinMode(hallwayLight, OUTPUT);
}

void loop() {
  // Check button state
  if (checkButtonStatus()) {
    // If button is pressed, call method to turn both lights on 
    lightsON();
    // Then call method to turn lights off
    lightsOFF();
  }
}

// Checks state of button and returns boolean depending on status
bool checkButtonStatus() {
  // Initialize a variable for if the button is pressed or not
  int state = digitalRead(button);
  if(state == LOW) {
    // Return true if button is pressed
    return true;
  }
  else {
    // Otherwise return if button has not been pressed
    return false;
  }
}

// Turns porch and hallway lights on
void lightsON() {
  digitalWrite(porchLight, HIGH);
  digitalWrite(hallwayLight, HIGH);
}

// Turns porch and hallway lights off
void lightsOFF() {
  // Turn porch light off after 30 seconds 
  delay(30000);
  digitalWrite(porchLight, LOW);

  // Turn hallway light off after anolther 30 (totaling 60) seconds
  delay(30000);
  digitalWrite(hallwayLight, LOW);
}
