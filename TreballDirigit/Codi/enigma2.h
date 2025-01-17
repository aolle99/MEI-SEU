#ifndef ENIGMA2_H
#define ENIGMA2_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#define LED_SIMON_RED 26
#define LED_SIMON_YELLOW 28
#define LED_SIMON_GREEN 30
#define LED_SIMON_BLUE 32

#define BUTTON_SIMON_RED 27
#define BUTTON_SIMON_YELLOW 29
#define BUTTON_SIMON_GREEN 31
#define BUTTON_SIMON_BLUE 33

extern LiquidCrystal lcd;

void initializeEnigma2();
void playLED(int color);
void turnOnLED(int color);
void turnOffLEDs();
bool isButtonPressed();
int getButtonPressed();
bool updateEnigma2();

#endif
