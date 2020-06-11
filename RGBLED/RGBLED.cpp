#include "RGBLED.h"

  // Constructor
  // Assign PWM pins for each colour
  RGBLED::RGBLED(byte Rpin, byte Gpin, byte Bpin):pinR(Rpin),pinG(Gpin),pinB(Bpin) {
    
  }
  
  RGBLED::~RGBLED(){/*Nothing to destruct*/}

  void RGBLED::setup(){
    // Initialise the pins and set LED to off
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);
    digitalWrite(pinR, LOW);
    digitalWrite(pinG, LOW);
    digitalWrite(pinB, LOW);

    // No colour changing, flashing or pulsing
    fade = false;
    flash = false;
    pulse = false;

    // set default delays
    fadeMS = 1000;
    flashMS = 500;
    pulseMS = 50;

    // Record current colour (off)
    curCol.R = 0;
    curCol.G = 0;
    curCol.B = 0;

    // The LED is off
    ledOn = false;

    // Set brigtness to 100%
    bright = 1.0;
    pulseBright = 1.0;

    pulseStep = 1;
  }

  void RGBLED::onOff(bool On){
    // Turn the LED on or off
    ledOn = On;

    if(ledOn){
      analogWrite(pinR, curCol.R*bright);
      analogWrite(pinG, curCol.G*bright);
      analogWrite(pinB, curCol.B*bright);
    } else {
      digitalWrite(pinR, LOW);
      digitalWrite(pinG, LOW);
      digitalWrite(pinB, LOW);
    }
  }

  bool RGBLED::onOff(){
    // Find out if the LED is on
    return ledOn;
  }

  void RGBLED::flashing(bool On){
    if(On){
      flash = true;
      flashTime = millis();
    } else {
      flash = false;
      analogWrite(pinR, curCol.R*bright);
      analogWrite(pinG, curCol.G*bright);
      analogWrite(pinB, curCol.B*bright);
    }
  }

  boolean RGBLED::flashing(){
    return flash;
  }

  void RGBLED::flashDelay(int msDelay){
    flashMS = msDelay;
  }

  int RGBLED::flashDelay(){
    return flashMS;
  }

  void RGBLED::fadeTo(rgb colour){
    
    // Start a gradual move to the new colour
    newCol.R = colour.R;
    newCol.G = colour.G;
    newCol.B = colour.B;
  }

  void RGBLED::fadeDelay(int msDelay){
    fadeMS = msDelay;
  }

  int RGBLED::fadeDelay(){
    return fadeMS;
  }

  bool RGBLED::fading(){
    return fade;
  }

  void RGBLED::pulsing(bool On) {
    if(On){
      pulse = true;
      pulseTime = millis();
      pulseBright = 1.0;
      pulseStep = -1;
    } else {
      pulse = false;
      pulseBright = 1.0;
      analogWrite(pinR, curCol.R*bright*pulseBright);
      analogWrite(pinG, curCol.G*bright*pulseBright);
      analogWrite(pinB, curCol.B*bright*pulseBright);
    }
  }

  bool RGBLED::pulsing() {
    return pulse;
  }

  void RGBLED::pulseDelay(int msDelay){
    pulseMS = msDelay;
  }

  int RGBLED::pulseDelay(){
    return pulseMS;
  }

  void RGBLED::changeTo(rgb colour){
    
    // Change colour instantly
    newCol.R = colour.R;
    newCol.G = colour.G;
    newCol.B = colour.B;

    curCol.R = colour.R;
    curCol.G = colour.G;
    curCol.B = colour.B;

    if (ledOn){
      analogWrite(pinR, curCol.R*bright);
      analogWrite(pinG, curCol.G*bright);
      analogWrite(pinB, curCol.B*bright);
    }
  }

  void RGBLED::changeToWhite(int temperature){
    // Set the LED to white with the given colour temperature
    // Algorithm thanks to http://www.tannerhelland.com/4435/

    int red;
    int green;
    int blue;

    // Temperature must be between 1000 and 40000
    if (temperature < 1000) {
      temperature = 1000;
    }
    if (temperature > 40000) {
      temperature = 40000;
    }

    temperature = temperature / 100;

    // Calculate Red
    if(temperature <= 66) {
      red = 255;
    } else {
      red = temperature - 60;
      red = 329.698727446 * pow(red,-0.1332047592);
      if (red < 0) {
        red = 0;
      }
      if (red > 255) {
        red = 255;
      }
    }

    // Calculate Green
    if(temperature <= 66){
      green = temperature;
      green = 99.4708025861 * log(green) - 161.1195681661;
    } else {
      green = temperature - 60;
      green = 288.1221695283 * pow(green,-0.0755148492);
    }
    if (green < 0){
      green = 0;
    }
    if (green > 255){
      green = 255;
    }

    // Calculate Blue
    if (temperature >= 66){
      blue = 255;
    } else {
      if (temperature <= 19) {
        blue = 0;
      } else {
        blue = temperature - 10;
        blue = 138.5177312231 * log(blue) - 305.0447927307;
      }
      if (blue < 0){
        blue = 0;
      }
      if (blue > 255){
        blue = 255;
      }
    }

    // Change colour instantly
    newCol.R = red;
    newCol.G = green;
    newCol.B = blue;

    curCol.R = red;
    curCol.G = green;
    curCol.B = blue;

    if (ledOn){
      analogWrite(pinR, curCol.R*bright);
      analogWrite(pinG, curCol.G*bright);
      analogWrite(pinB, curCol.B*bright);
    }
  }

  void RGBLED::brightness(int percent){
    bright = percent / 100.0;
  }

  int RGBLED::brightness(){
    return bright * 100.0;
  }

  void RGBLED::loop(){

    // Should be called as often as possible in the main loop of the sketch
    // Check if the LED is flashing
    if(flash){
      if(millis()-flashTime > flashMS){
        ledOn = !ledOn;
        
        // Change state of LED
        if(ledOn){
          analogWrite(pinR, curCol.R*bright*pulseBright);
          analogWrite(pinG, curCol.G*bright*pulseBright);
          analogWrite(pinB, curCol.B*bright*pulseBright);
        } else {
          digitalWrite(pinR, LOW);
          digitalWrite(pinG, LOW);
          digitalWrite(pinB, LOW);
        }

        flashTime = millis();
      }
    }

    if(pulse){
      if(millis()-pulseTime > pulseMS){
        if(pulseStep == 1){
          pulseBright += 0.01;
          if(pulseBright > 1) {
            pulseBright = 1;
          }
          if(pulseBright >= 1){
            pulseStep = -1;
          }
        } else {
          pulseBright -= 0.01;

          if(pulseBright <0){
            pulseBright = 0;
          }
          
          if(pulseBright == 0){
            pulseStep = 1;
          }
        }

        analogWrite(pinR, curCol.R*bright*pulseBright);
        analogWrite(pinG, curCol.G*bright*pulseBright);
        analogWrite(pinB, curCol.B*bright*pulseBright);

        pulseTime = millis();
      }
    }

    // Check if the LED is On
    if(ledOn){
      // Check if the current colour is different to the required colour, if yes change gradually
      if (newCol.R == curCol.R && newCol.G == curCol.G && newCol.B == curCol.B){
        // If the current and new colours match then do nothing
        fade = false;
      } else {
        fade = true;
  
        if (millis() - fadeTime > fadeMS){
          // If they don't match then move towards the new colour
          if (newCol.R != curCol.R){
            if(newCol.R > curCol.R){
              curCol.R++;
            } else {
              curCol.R--;
            }

            analogWrite(pinR, curCol.R*bright*pulseBright);
          }
    
          if (newCol.G != curCol.G){
            if(newCol.G > curCol.G){
              curCol.G++;
            } else {
              curCol.G--;
            }

            analogWrite(pinG, curCol.G*bright*pulseBright);
          }
    
          if (newCol.B != curCol.B){
            if(newCol.B > curCol.B){
              curCol.B++;
            } else {
              curCol.B--;
            }

            analogWrite(pinB, curCol.B*bright*pulseBright);
          }
  
          fadeTime = millis();
        }
      }
    }
  }