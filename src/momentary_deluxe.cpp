/*
  momentary_deluxe.cpp - Library for defining and reading momentary button/switch states, including: presses, releases, holds and double presses.
  Created by Joseph Elias [Funnytron Labs], April 2, 2024.
  Released into the public domain.
*/

// === INCLUDES === //
#include "momentary_deluxe.h"


// === METHODS === //
momentary_deluxe::momentary_deluxe(uint8_t pinNumber, uint8_t pull_up_or_down, uint8_t normally_open_or_closed)
{
  this->pinNumber = pinNumber;

  if (pull_up_or_down == CONFIGURATION_INTERNAL_PULL_UP) pinMode(pinNumber, INPUT_PULLUP);
  else pinMode(pinNumber, INPUT);
  
  if (pull_up_or_down == CONFIGURATION_PULL_UP || pull_up_or_down == CONFIGURATION_INTERNAL_PULL_UP){
    if (normally_open_or_closed == CONFIGURATION_NORMALLY_OPEN) configurationType = INTERACTION_ON_LOW;
    else configurationType = INTERACTION_ON_HIGH;
  }
  else {
    if (normally_open_or_closed == CONFIGURATION_NORMALLY_OPEN) configurationType = INTERACTION_ON_HIGH;
    else configurationType = INTERACTION_ON_LOW;
  }
  
  previousInteraction = BUTTON_NOT_TOUCHED; // NO INTERACTION
  previousScanResult  = BUTTON_UNPRESSED;

  pressTime   = 0; // IN MILLISECONDS
  releaseTime = 0; // IN MILLISECONDS
  
  pressDebouncePeriod   = DEFAULT_DEBOUNCE_PERIOD; // IN MILLISECONDS
  releaseDebouncePeriod = DEFAULT_RELEASE_PERIOD;  // IN MILLISECONDS

  pressHoldThreshold   = DEFAULT_PRESS_HOLD_THRESHOLD;   // IN MILLISECONDS
  doublePressThreshold = DEFAULT_DOUBLE_PRESS_THRESHOLD; // IN MILLISECONDS

  action_press        = BUTTON_NO_ACTION;
  action_hold         = BUTTON_NO_ACTION;
  action_double_press = BUTTON_NO_ACTION;

  action_release_press        = BUTTON_NO_ACTION;
  action_release_hold         = BUTTON_NO_ACTION;
  action_release_double_press = BUTTON_NO_ACTION;
}

unsigned int momentary_deluxe::read(void)
{
  unsigned long nowTime = millis();

  int interactingNow;
  
  if (configurationType == INTERACTION_ON_HIGH){
    interactingNow = (digitalRead(this->pinNumber))? BUTTON_TOUCHED : BUTTON_NOT_TOUCHED;
  }
  else {
    interactingNow = (digitalRead(this->pinNumber))? BUTTON_NOT_TOUCHED : BUTTON_TOUCHED;
  }

  //int interactingNow = digitalRead(this->pinNumber);

  if ((nowTime - pressTime) < pressDebouncePeriod || (nowTime - releaseTime) < releaseDebouncePeriod) {
    // --- REJECT [interactingNow] BECAUSE SCAN IS WITHIN DEBOUNCE PERIOD --- //
    return BUTTON_DEBOUNCING;
  }
  else {
    // --- ACCEPT [interactingNow] AND PROCESS INTERACTION --- //
    unsigned int wasInteracting = previousInteraction; // SAVE FOR ...
    previousInteraction = interactingNow;              // ... NEXT CYCLE
    
    if (interactingNow) {
      if (wasInteracting) {
        // +++ PRESS VS HOLD +++ //
        if ((nowTime-pressTime) < pressHoldThreshold) return BUTTON_PRESSED_WAKE; // KEEP pressTime UNCHANGED HERE
        else {
          if (action_hold != BUTTON_NO_ACTION) (*action_hold)();
          previousScanResult = BUTTON_HELD;
          return BUTTON_HELD;
        }
      }
      else {
        // +++ PRESS VS DOUBLE PRESS/CLICK +++ //
        pressTime = nowTime;
        if ((nowTime-releaseTime) < doublePressThreshold){
          if (action_double_press != BUTTON_NO_ACTION) (*action_double_press)();
          previousScanResult = BUTTON_DOUBLE_PRESSED;
          return BUTTON_DOUBLE_PRESSED;
        }
        else {
          if (action_press != BUTTON_NO_ACTION) (*action_press)();
          previousScanResult = BUTTON_PRESSED;
          return BUTTON_PRESSED;
        }
      }
    }
    else {
      if (wasInteracting) {
        // +++ RELEASE +++ //
        releaseTime = nowTime;

        switch(previousScanResult){
          case BUTTON_PRESSED:
            if (action_release_press != BUTTON_NO_ACTION) (*action_release_press)();
            break;

          case BUTTON_DOUBLE_PRESSED:
            if (action_release_double_press != BUTTON_NO_ACTION) (*action_release_double_press)();
            break;

          case BUTTON_HELD:
            if (action_release_hold != BUTTON_NO_ACTION) (*action_release_hold)();
            break;
        }

        previousScanResult = BUTTON_RELEASED;
        return BUTTON_RELEASED;
      }
      else {
        // +++ NO INTERACTION +++ //
        return BUTTON_UNPRESSED;
      }
    }
  }
}

unsigned long momentary_deluxe::holdTime(void)
{
  if (previousScanResult == BUTTON_HELD) return (millis() - pressTime);
  else return 0;
}

void momentary_deluxe::set_pressDebounceTime(unsigned long newPressDebouncePeriod)
{
  if (newPressDebouncePeriod > pressHoldThreshold) pressDebouncePeriod = pressHoldThreshold;
  else pressDebouncePeriod = newPressDebouncePeriod;
}

void momentary_deluxe::set_releaseDebounceTime(unsigned long newReleaseDebouncePeriod)
{
  if (newReleaseDebouncePeriod > doublePressThreshold) releaseDebouncePeriod = doublePressThreshold;
  else releaseDebouncePeriod = newReleaseDebouncePeriod;
}

void momentary_deluxe::set_pressHoldTime(unsigned long newPressHoldThreshold)
{
  if (newPressHoldThreshold < pressDebouncePeriod) pressHoldThreshold = pressDebouncePeriod;
  else pressHoldThreshold = newPressHoldThreshold;
}

void momentary_deluxe::set_doublePressTime(unsigned long newDoublePressThreshold)
{
  if (newDoublePressThreshold < releaseDebouncePeriod) doublePressThreshold = releaseDebouncePeriod;
  else doublePressThreshold = newDoublePressThreshold;
}
