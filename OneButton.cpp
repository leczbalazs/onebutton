// -----
// OneButton.cpp - Library for detecting clicks, double-clicks and long presses on a single button.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 02.10.2010 created by Matthias Hertel
// 21.04.2011 support of active LOW and active HIGH button signal input.
// 01.12.2011 include file changed to work with the Arduino 1.0 environment
// 21.09.2012 Balázs Lécz: added support for repeat; optimized default timings
// -----

#include "OneButton.h"

// ----- Initialization and Default Values -----

OneButton::OneButton(int pin, int activeLow)
{
  pinMode(pin, INPUT); // sets pin as input
  _pin = pin;

  _clickTicks = 80; // timeout for detecting a single click [milliseconds]
  _pressTicks = 750; // timeout for detecting a long-press [milliseconds]
  _repeatTicks = 60; // delay between two repetitions [milliseconds]

  _state = 0; // starting with state 0: waiting for button to be pressed

  if (activeLow) {
    // button connects ground to the pin when pressed.
    _buttonReleased = HIGH; // notPressed
    _buttonPressed = LOW;
    digitalWrite(pin, HIGH);   // turn on pulldown resistor
  } 
  else {
    // button connects Vcc to the pin when pressed.
    _buttonReleased = LOW;
    _buttonPressed = HIGH;
  }

} // OneButton


// explicitely set the number of millisec that have to pass by before a click is detected.
void OneButton::setClickTicks(int ticks) { 
  _clickTicks = ticks;
} // setClickTicks


// explicitely set the number of millisec that have to pass by before a lonn button press is detected.
void OneButton::setPressTicks(int ticks) {
  _pressTicks = ticks;
} // setPressTicks


void OneButton::setRepeatTicks(int ticks) {
  _repeatTicks = ticks;
}

void OneButton::setRepeat(boolean repeat) {
  _repeat = repeat;
}

// save function for click event
void OneButton::attachClick(callbackFunction newFunction)
{
  _clickFunc = newFunction;
} // attachClick


// save function for doubleClick event
void OneButton::attachDoubleClick(callbackFunction newFunction)
{
  _doubleClickFunc = newFunction;
} // attachDoubleClick


// save function for press event
void OneButton::attachPress(callbackFunction newFunction, boolean repeat)
{
  _pressFunc = newFunction;
  _repeat = repeat;
} // attachPress


void OneButton::tick(void)
{
  // Detect the input information 
  int buttonLevel = digitalRead(_pin); // current button signal.
  unsigned long now = millis(); // current (relative) time in msecs.

  // Implementation of the state machine
  if (_state == 0) { // waiting for menu pin being pressed.
    if (buttonLevel == _buttonPressed) {
      _state = 1; // step to state 1
      _startTime = now; // remember starting time
    } // if

  } else if (_state == 1) { // waiting for menu pin being released.
    if (buttonLevel == _buttonReleased) {
      _state = 2; // step to state 2

    } else if ((buttonLevel == _buttonPressed) && (now > _startTime + _pressTicks)) {
      _startTime = now;
      if (_pressFunc) _pressFunc();
      _state = 6; // step to state 6
    } else {
      // wait. Stay in this state.
    } // if

  } else if (_state == 2) { // waiting for menu pin being pressed the second time or timeout.
    if (now > _startTime + _clickTicks) {
      // this was only a single short click
      if (_clickFunc) _clickFunc();
      _state = 0; // restart.

    } else if (buttonLevel == _buttonPressed) {
      _state = 3; // step to state 3
    } // if

  } else if (_state == 3) { // waiting for menu pin being released finally.
    if (buttonLevel == _buttonReleased) {
      // this was a 2 click sequence.
      if (_doubleClickFunc) _doubleClickFunc();
      _state = 0; // restart.
    } // if

  } else if (_state == 6) { // waiting for menu pin being release after long press.
    if (buttonLevel == _buttonReleased) {
      _state = 0; // restart.
    } else if (_repeat && (now > _startTime + _repeatTicks)) {
      _startTime = now;
      if (_pressFunc) _pressFunc();
    }  

  } // if  
} // OneButton.tick()


// end.


