/*
  momentary_deluxe.h - Library for defining and reading momentary button/switch states, including: presses, releases, holds and double presses.
  Created by Joseph Elias [Funnytron Labs], April 2, 2024.
  Released into the public domain.
*/

#ifndef momentary_deluxe_h
#define momentary_deluxe_h


// === INCLUDES === //
#include "Arduino.h"


// === DEFINES === //
#define CONFIGURATION_PULL_UP          0
#define CONFIGURATION_PULL_DOWN        1
#define CONFIGURATION_INTERNAL_PULL_UP 2

#define PULL_UP                        CONFIGURATION_PULL_UP
#define PULL_DOWN                      CONFIGURATION_PULL_DOWN
#define INTERNAL_PULL_UP               CONFIGURATION_INTERNAL_PULL_UP

#define PU                             CONFIGURATION_PULL_UP
#define PD                             CONFIGURATION_PULL_DOWN
#define IPU                            CONFIGURATION_INTERNAL_PULL_UP

#define CONFIGURATION_NORMALLY_OPEN   0
#define CONFIGURATION_NORMALLY_CLOSED 1

#define NORMALLY_OPEN                 CONFIGURATION_NORMALLY_OPEN
#define NORMALLY_CLOSED               CONFIGURATION_NORMALLY_CLOSED

#define NO                            CONFIGURATION_NORMALLY_OPEN
#define NC                            CONFIGURATION_NORMALLY_CLOSED

#define INTERACTION_ON_HIGH HIGH
#define INTERACTION_ON_LOW  LOW

#define BUTTON_NO_ACTION 0 // NULL POINTER

#define BUTTON_TOUCHED     1
#define BUTTON_NOT_TOUCHED 0

#define BUTTON_UNPRESSED      0 // NO INTERACTION
#define BUTTON_PRESSED        1 // BUTTON IS PRESSED
#define BUTTON_SINGLE_PRESSED 1 // SAME AS BUTTON_PRESSED
#define BUTTON_DOUBLE_PRESSED 2 // "DOUBLE CLICK"
#define BUTTON_PRESSED_WAKE   3 // BUTTON HAS BEEN PRESSED BUT NOT LONG ENOUGH TO BE HELD
#define BUTTON_HELD           4 // BUTTON IS HELD
#define BUTTON_RELEASED       5 // BUTTON WAS RELEASED
#define BUTTON_DEBOUNCING     6 // BUTTON IS DEBOUNCING

#define DEFAULT_DEBOUNCE_PERIOD 100                     // IN MILLISECONDS
#define DEFAULT_RELEASE_PERIOD  DEFAULT_DEBOUNCE_PERIOD // IN MILLISECONDS

#define DEFAULT_PRESS_HOLD_THRESHOLD   (DEFAULT_DEBOUNCE_PERIOD + 10)  // IN MILLISECONDS
#define DEFAULT_DOUBLE_PRESS_THRESHOLD (DEFAULT_DEBOUNCE_PERIOD + 600) // IN MILLISECONDS


// === CLASSES === //
class momentary_deluxe
{
  private:
    uint8_t pinNumber;

    bool configurationType;
    
    unsigned int previousInteraction;    // NO INTERACTION [UNPRESSED/RELEASED] = 0, INTERACTION [PRESSED/DOUBLE PRESSED/HELD] > 0
    unsigned int previousScanResult;     // BUTTON_UNPRESSED, BUTTON_PRESSED, ... , BUTTON_SWIPED

    unsigned long pressTime;             // IN MILLISECONDS
    unsigned long releaseTime;           // IN MILLISECONDS

    unsigned long pressDebouncePeriod;   // DEBOUNCE PERIOD ON PRESS, IN MILLISECONDS
    unsigned long releaseDebouncePeriod; // DEBOUNCE PERIOD ON RELEASE, IN MILLISECONDS

    unsigned long pressHoldThreshold;    // IN MILLISECONDS
    unsigned long doublePressThreshold;  // IN MILLISECONDS

  public:
    void (*action_press)(void);
    void (*action_double_press)(void);
    void (*action_hold)(void);
    void (*action_release_press)(void);
    void (*action_release_double_press)(void);
    void (*action_release_hold)(void);
    
  public:
    momentary_deluxe(uint8_t pinNumber, uint8_t pull_up_or_down, uint8_t normally_open_or_closed);
    unsigned int read(void);
    unsigned long holdTime(void);
    void set_pressDebounceTime(unsigned long newPressDebouncePeriod);
    void set_releaseDebounceTime(unsigned long newReleaseDebouncePeriod);
    void set_pressHoldTime(unsigned long newPressHoldThreshold);
    void set_doublePressTime(unsigned long newDoublePressThreshold);
};

#endif


/*
 * TIMING RELATIONSHIPS
 * --------------------
 *                                          --
 *                                         |    _____DEBOUNCE_____ ______PRESS WAKE_______ ____________HOLD____________
 *                                         |   |                  |                       |
 *                                         |   v                  v                       v
 * if (interactingNow && wasInteracting)  <    |------------------|-----------------------|----------------------------> time
 *                                         | PRESS              PRESS                   PRESS
 *                                         | TIME               DEBOUNCE                HOLD
 *                                         |                    PERIOD                  THRESHOLD
 *                                          --
 *
 *
 *                                          --
 *                                         |    _____DEBOUNCE_____ _____DOUBLE PRESS______ ____________PRESS___________
 *                                         |   |                  |                       |
 *                                         |   v                  v                       v
 * if (interactingNow && !wasInteracting) <    |------------------|-----------------------|----------------------------> time
 *                                         | RELEASE            RELEASE                 DOUBLE
 *                                         | TIME               DEBOUNCE                PRESS
 *                                         |                    PERIOD                  THRESHOLD
 *                                          --
 *
 */
