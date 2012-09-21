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
// 21.09.2012 Balázs Lécz: added support for repeat; optimized default timings; readability cleanups
// -----

#include "OneButton.h"

// Possible states
#define WAIT_PRESS 0
#define WAIT_RELEASE 1
#define WAIT_SECOND_CLICK 2
#define WAIT_SECOND_RELEASE 3
#define IN_LONG_PRESS 4

// Timeout for detecting a single click [milliseconds]
#define DEFAULT_CLICK_TIMEOUT 80 

// Timeout for detecting a long-press [milliseconds]
#define DEFAULT_PRESS_TIMEOUT 750

// Delay between two repetitions [milliseconds]
#define DEFAULT_REPEAT_DELAY 60

OneButton::OneButton(int pin, boolean activeLow) {
  pinMode(pin, INPUT); // sets pin as input
  _pin = pin;

  _clickTicks = DEFAULT_CLICK_TIMEOUT;
  _pressTicks = DEFAULT_PRESS_TIMEOUT;
  _repeatTicks = DEFAULT_REPEAT_DELAY;

  _state = WAIT_PRESS; // starting state: waiting for button to be pressed

  if (activeLow) {
    // Button connects ground to the pin when pressed.
    _buttonReleased = HIGH;
    _buttonPressed = LOW;
    digitalWrite(pin, HIGH);  // turn on pulldown resistor
  } else {
    // Button connects Vcc to the pin when pressed.
    _buttonReleased = LOW;
    _buttonPressed = HIGH;
  }
}


void OneButton::setClickTicks(int ticks) { 
  _clickTicks = ticks;
} 


void OneButton::setPressTicks(int ticks) {
  _pressTicks = ticks;
}


void OneButton::setRepeatTicks(int ticks) {
  _repeatTicks = ticks;
}


void OneButton::setRepeat(boolean repeat) {
  _repeat = repeat;
}


void OneButton::attachClick(callbackFunction newFunction) {
  _clickFunc = newFunction;
}


void OneButton::attachDoubleClick(callbackFunction newFunction) {
  _doubleClickFunc = newFunction;
}


void OneButton::attachPress(callbackFunction newFunction, boolean repeat) {
  _pressFunc = newFunction;
  _repeat = repeat;
}


void OneButton::tick() {
  int buttonLevel = digitalRead(_pin);
  unsigned long now = millis();

  switch (_state) {
    case WAIT_PRESS: 
      if (buttonLevel == _buttonPressed) {
        _state = WAIT_RELEASE;
        _startTime = now;
      }
      break;

     case WAIT_RELEASE:
       if (buttonLevel == _buttonReleased) {
         _state = WAIT_SECOND_CLICK;
       } else if ((buttonLevel == _buttonPressed)
             && (now > _startTime + _pressTicks)) {
         _startTime = now;
         if (_pressFunc) _pressFunc();
         _state = IN_LONG_PRESS;
       }
       break;

     case WAIT_SECOND_CLICK:
       if (now > _startTime + _clickTicks) {
         // this was only a single short click
         if (_clickFunc) _clickFunc();
         _state = WAIT_PRESS;
       } else if (buttonLevel == _buttonPressed) {
         _state = WAIT_SECOND_RELEASE;
       }
       break;

     case WAIT_SECOND_RELEASE:
       if (buttonLevel == _buttonReleased) {
         // this was a 2 click sequence.
         if (_doubleClickFunc) _doubleClickFunc();
         _state = WAIT_PRESS;
       }
       break;

     case IN_LONG_PRESS:
       if (buttonLevel == _buttonReleased) {
         _state = WAIT_PRESS;
       } else if (_repeat && (now > _startTime + _repeatTicks)) {
         _startTime = now;
         if (_pressFunc) _pressFunc();
       }  
       break;
  }
}
