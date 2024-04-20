/*
  MOMENTARY DELUXE DEMO
  ---------------------
  BY: JOSEPH ELIAS [FUNNYTRON LABS]
  APRIL 2024

  Hardware Connections
  ====================
  Push Button to GPIO 4, pulled up with a 10K Ohm resistor.

  This example demonstrates the use of the function pointer members, of the momentary_deluxe class.
*/

// === INCLUDES === //
#include "momentary_deluxe.h"


// === DEFINES === //
#define BUTTON_PIN 4


// === GLOBALS === //
momentary_deluxe test_button(BUTTON_PIN, PULL_UP, NORMALLY_OPEN);


// === PROTOTYPES === //
void action_press_function(void);
void action_double_press_function(void);
void action_hold_function(void);
void action_release_press_function(void);
void action_release_double_press_function(void);
void action_release_hold_function(void);


// === MAIN FUNCTIONS === //
void setup(){
  Serial.begin(115200);
  delay(1000); // GIVE SERIAL MONITOR TIME TO OPEN

  test_button.action_press = action_press_function;
  test_button.action_double_press = action_double_press_function;
  test_button.action_hold = action_hold_function;
  test_button.action_release_press = action_release_press_function;
  test_button.action_release_double_press = action_release_double_press_function;
  test_button.action_release_hold = action_release_hold_function;

  Serial.println("--- SETUP COMPLETE ---");
}

void loop(){
  static unsigned long waitHere = 0;
  
  if (millis() - waitHere > 1000){
    waitHere = millis();
    Serial.println("--- LOOPING ---");
  }

  test_button.read(); // MUST READ BUTTON AT EVERY NEW CYCLE
}


// === OTHER FUNCTIONS === //
void action_press_function(void)
{
  Serial.println("*** BUTTON PRESSED ***");
}

void action_double_press_function(void)
{
  Serial.println("### BUTTON DOUBLE PRESSED ###");
}

void action_hold_function(void)
{
  Serial.printf("!!! BUTTON HELD FOR %u MILLISECONDS \n !!!", test_button.holdTime());
}

void action_release_press_function(void)
{
  Serial.println("^^^ BUTTON PRESS RELEASED ^^^");
}

void action_release_double_press_function(void)
{
  Serial.println("^^^ BUTTON DOUBLE RELEASED ^^^");
}

void action_release_hold_function(void)
{
  Serial.println("^^^ BUTTON HOLD RELEASED ^^^");
}
