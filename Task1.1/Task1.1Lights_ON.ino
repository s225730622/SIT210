// TASK 1.1P | Modular Programming

const int button = 2;
const int porchLight = 10;
const int hallwayLight = 12;

unsigned long startMillis;
unsigned long timeElapsed;
const unsigned long porchLightOn = 30000;
const unsigned long hallwayLightOn = 60000;

bool lightsSwitchedOn = false;

void setup() 
{
  pinMode(button, INPUT_PULLUP);
  pinMode(hallwayLight, OUTPUT);
  pinMode(porchLight, OUTPUT);
}

void loop() 
{
  int buttonState = digitalRead(button);
  
  if (buttonState == LOW && !lightsSwitchedOn)
  {
    lightsSwitchedOn = true;
    startMillis = millis();

    digitalWrite(porchLight, HIGH);
    digitalWrite(hallwayLight, HIGH);
  }
  if (lightsSwitchedOn)
  {
    timeElapsed = millis() - startMillis;

    if (timeElapsed >= porchLightOn)
    {
      digitalWrite(porchLight, LOW);
    }

    if (timeElapsed >= hallwayLightOn)
    {
      digitalWrite(hallwayLight, LOW);
      lightsSwitchedOn = false;
    }
  }
}
