/*
 This is a sample sketch to show how to use the OneButton library
 to detect double-click events on a button.
 The library internals are explained at
 http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
  
 Set up a test circuit:
 * Connect a pushbutton to pin A1 (ButtonPin) and ground.
 * Pin 13 (StatusPin) is used for output. Attach an LED and a resistor to ground
   or see the built-in led on the standard Arduino board.
   
 The Sketch shows how to set up the library and bind function to the doubleclick event.
 In the loop function, the button.tick() function has to be called as often as you like.
*/
 
// 03.03.2011 created by Matthias Hertel
// 01.12.2011 extension changed to work with the Arduino 1.0 environment
// 21.09.2012 Balázs Lécz: style cleanups

#include "OneButton.h"

// Set up a new OneButton on pin A1.
OneButton button(A1, true);


// Setup code (run once).
void setup() {
  // Enable the standard LED on pin 13.
  pinMode(13, OUTPUT); // Sets the digital pin as output.
  
  // Link the doubleclickHandler() function to the double-click event.   
  button.attachDoubleClick(doubleclickHandler);
}
  

// main code here, to run repeatedly: 
void loop() {
  // keep watching the push button:
  button.tick();

  // You can implement other code in here or just wait a while 
  delay(10);
}


// This function will be called when the button has been pressed twice in a short timeframe.
void doubleclickHandler() {
  static int m = LOW;
  // reverse the LED 
  m = !m;
  digitalWrite(13, m);
}

