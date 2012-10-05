// -----
// OneButton.h - Library for detecting clicks, double-clicks and long presses on a single button.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 02.10.2010 created by Matthias Hertel
// 21.04.2011 transformed into a library
// 01.12.2011 include file changed to work with the Arduino 1.0 environment
// 21.09.2012 Balázs Lécz: added support for repeat; optimized default timings; general cleanups
// -----

#ifndef OneButton_h
#define OneButton_h

#include "Arduino.h"

#define NO_REPEAT false
#define REPEAT true

extern "C" {
  typedef void (*callbackFunction)(void);
}


class OneButton {
public:
  OneButton(int pin, boolean activeLow, boolean repeat);

  void setClickTicks(int ticks);
  void setPressTicks(int ticks);
  void setRepeatTicks(int ticks);
  void setRepeat(boolean repeat);

  void attachClick(callbackFunction newFunction);
  void attachDoubleClick(callbackFunction newFunction);
  void attachLongPress(callbackFunction newFunction);

  // call this function every few milliseconds for handling button events.
  void tick();

private:
  int _pin; // hardware pin number
  int _clickTicks;
  int _longPressTicks;
  int _repeatTicks;
  
  int _buttonReleased;
  int _buttonPressed;
  boolean _repeat;

  // Function pointers to event handlers
  callbackFunction _clickFunc;
  callbackFunction _doubleClickFunc;
  callbackFunction _longPressFunc;

  int _state;
  unsigned long _startTime;
};
#endif
