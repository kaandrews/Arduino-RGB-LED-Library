#ifndef RGBLED_H
#define RGBLED_H

#include <Arduino.h>

typedef struct {
  byte R;
  byte G;
  byte B;
} rgb;

class RGBLED {
public:
	RGBLED(byte Rpin, byte Gpin, byte Bpin);
	~RGBLED();
	void setup();
	void onOff(bool On);
	bool onOff();
	void flashing(bool On);
	boolean flashing();
	void flashDelay(int msDelay);
	int flashDelay();
	void fadeTo(rgb colour);
	void fadeDelay(int msDelay);
	int fadeDelay();
	bool fading();
	void pulsing(bool on);
	bool pulsing();
	void pulseDelay(int msDelay);
	int pulseDelay();
	void changeTo(rgb colour);
	void changeToWhite(int temperature);
	void brightness(int percent);
	int brightness();
	void loop();
private:
	// Pins to use to control RGB LED
	const byte pinR;
	const byte pinG;
	const byte pinB;
	
	// Status variables
	bool ledOn;     // indicates if the LED is on or not
	bool fade;      // indicates if the colour is currently changing
	bool flash;     // indicates if the LED is currently flashing
	bool pulse;     // indicates if the LED is currently pulsing
	
	// Speed variables
	int fadeMS;
	int flashMS;
	int pulseMS;
	
	// Previous run time stores
	unsigned long fadeTime;
	unsigned long flashTime;
	unsigned long pulseTime;
	
	// Brightness of LED
	float bright;
	
	// Tracking pulsing
	int pulseStep;
	float pulseBright;
	
	// Store current and target colour
	rgb curCol = {0,0,0};
	rgb newCol = {0,0,0};
};

#endif