# Task 1.1P - Switching ON Lights: Modular Programming Approach #


### TASK DESCRIPTION ###
Design a system by writing a program which switches ON the porch light and the hallway light when a switch/push button is pressed.

The porch light should stay ON for 30 seconds and the hallway light should stay ON for 60 seconds.  
<br/>

### SOLUTION CODE EXPLANATION ###
* I have declared int values for the button/LEDs, some time/duration variables to use later and a boolean which triggers the LEDs turning on once the button is pressed.
* The button and LED variables are assigned their respective modes within the setup function.
* The loop function first checks if the button has been pressed.
  *  If the button does get pressed, the sequence below is followed:
      1. The number of milliseconds elapsed is recorded and stored in the startMillis variable
      2. The porchLight and hallwayLight LEDs are turned on
      3. Once the porchLight timeElapsed reaches 30000 milliseconds, it is turned off due to the If statement
      4. The hallwayLight is also turned off once the timeElapsed reaches 60000 milliseconds
* The boolean variable is then set to false to reset the button.
<br/>  

#### Link to Repository:  https://github.com/s225730622/SIT210 ####
