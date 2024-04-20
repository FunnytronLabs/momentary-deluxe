/*
  MOMENTARY DELUXE DEMO
  ---------------------
  BY: JOSEPH ELIAS [FUNNYTRON LABS]
  APRIL 2024

  Hardware Connections
  ====================
  Push Button to GPIO 4, pulled up with a 10K Ohm resistor.

  This example demonstrates the use of a switch statement together with the read() member function, of the momentary_deluxe class.
*/

// === INCLUDES === //
#include "momentary_deluxe.h"


// === DEFINES === //
#define BUTTON_PIN 4


// === GLOBALS === //
momentary_deluxe test_button(BUTTON_PIN, PULL_UP, NORMALLY_OPEN);


// === MAIN FUNCTIONS === //
void setup(){
  Serial.begin(115200);
  delay(1000); // GIVE SERIAL MONITOR TIME TO OPEN

  Serial.println("--- SETUP COMPLETE ---");
}

void loop(){
  static unsigned long waitHere = 0;
  
  if (millis() - waitHere > 1000){
    waitHere = millis();
    Serial.println("--- LOOPING ---");
  }

  unsigned int buttonState = test_button.read(); // MUST READ BUTTON AT EVERY NEW CYCLE

  if (buttonState != BUTTON_UNPRESSED){
    switch(buttonState){
      case BUTTON_PRESSED:
        Serial.println("*** BUTTON PRESSED ***");
        break;
        
      case BUTTON_DOUBLE_PRESSED:
        Serial.println("### BUTTON DOUBLE PRESSED ###");
        break;
        
      case BUTTON_HELD:
        Serial.printf("!!! BUTTON HELD FOR %u MILLISECONDS \n !!!", test_button.holdTime());
        break;
        
      case BUTTON_RELEASED:
        Serial.println("^^^ BUTTON RELEASED ^^^");
    }
  }
}
