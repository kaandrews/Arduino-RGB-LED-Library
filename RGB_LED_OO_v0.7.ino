/*
RGB LED Control using Object Oriented architecture
Author: Kristian Andrews
Date: 10/May/2019

Controls the colour of an RGB LED using Pulse Width Modulation
Allows instant or gradual control of the colour

To do:
Add ability to have different on and off times for flashing
*/

#include "RGBLED.h"

// Define variables and objects
RGBLED rgbLED(6,5,3);   // Create RGBLED object with pins R=6, G=5, B=3
rgb colour = {255,0,0}; // Create an RGB colour set to Red
byte curColour = 2;     // Current colour in the rainbow colour cycle
byte colIncr = 1;       // Increment for rainbow colour cycle cycle
const boolean DEBUG = true;   // Set to true to report debug over serial connection
byte state = 6;          // Initial state of the program

void setup()
{
  if (DEBUG) {
    Serial.begin(9600);
  }
  
  rgbLED.setup();
}

void debug(char str[]) {
  if (DEBUG) {
    Serial.println(str);
  }
}

void debug(double value) {
  if (DEBUG) {
    Serial.println(value);
  }
}

void debug(bool blVal) {
  if (DEBUG) {
    if(blVal){
      Serial.println("true");
    } else {
      Serial.println("false");
    }
  }
}

// main loop
void loop()
{
  rgbLED.loop();

  switch (state) {
    case 1: // Solid blue
      colour.R = 0;
      colour.G = 0;
      colour.B = 255;
      rgbLED.changeTo(colour);
      rgbLED.brightness(50);
      rgbLED.onOff(true);
      state = 5;
      debug("Solid blue, 50% brightness");
      break;
    case 2: // Pusling White
      colour.R = 255;
      colour.G = 255;
      colour.B = 255;
      rgbLED.changeTo(colour);
      rgbLED.brightness(100);
      rgbLED.pulseDelay(30);
      rgbLED.pulsing(true);
      rgbLED.onOff(true);
      debug("Pulsing White, 100% brightness");
      state = 5;
      break;
    case 3: // Flashing RED
      colour.R = 255;
      colour.G = 0;
      colour.B = 0;
      rgbLED.changeTo(colour);
      rgbLED.brightness(80);
      rgbLED.flashDelay(500);
      rgbLED.flashing(true);
      rgbLED.onOff(true);
      debug("Flashing red, 80% brightness");
      state = 5;
    case 4: // Set white colour temperature
      rgbLED.changeToWhite(4800);
      rgbLED.brightness(60);
      rgbLED.onOff(true);
      debug("White colour temperature 4800K, 60% brightness");
      state = 5;
    case 5:
      // Do nothing, continue previous state
      break;
    case 6: // Set up for rainbow colours
      colour.R = 255;
      colour.G = 0;
      colour.B = 0;
      rgbLED.changeTo(colour);
      rgbLED.brightness(70);
      rgbLED.fadeDelay(30);
      rgbLED.onOff(true);
      debug("Rainbow colour cycle, 70% brightness");
      state = 7;
      break;
    case 7: // Cycle through colours
      if (!rgbLED.fading()){
        // If the LED has stopped changing colour, set the next colour to move to
        // Loops through the colours of the rainbow
        switch (curColour) {
          case 1: // Red
            colour.R = 255;
            colour.G = 0;
            colour.B = 0;
            colIncr = -colIncr;
            curColour += colIncr;
            debug("to Red");
            break;
          case 2: // Orange
            colour.R = 255;
            colour.G = 127;
            colour.B = 0;
            curColour += colIncr;
            debug("to Orange");
            break;
          case 3: // Yellow
            colour.R = 255;
            colour.G = 255;
            colour.B = 0;
            curColour += colIncr;
            debug("to Yellow");
            break;
          case 4: // Green
            colour.R = 0;
            colour.G = 255;
            colour.B = 0;
            curColour += colIncr;
            debug("to Green");
            break;
          case 5: // Cyan
            colour.R = 0;
            colour.G = 255;
            colour.B = 255;
            curColour += colIncr;
            debug("to Cyan");
            break;
          case 6: // Blue
            colour.R = 0;
            colour.G = 0;
            colour.B = 255;
            curColour += colIncr;
            debug("to Blue");
            break;
          case 7: // Violet
            colour.R = 127;
            colour.G = 0;
            colour.B = 255;
            colIncr = -colIncr; // Reverse increment to go back through colours
            curColour += colIncr;
            debug("to Violet");
            break;
        }

        rgbLED.fadeTo(colour);
      }
  }
}
