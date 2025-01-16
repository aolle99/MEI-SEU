#ifndef ENIGMA2_H
#define ENIGMA2_H

#include <Arduino.h>

#define LED_SIMON_RED 26
#define LED_SIMON_YELLOW 28
#define LED_SIMON_GREEN 30
#define LED_SIMON_BLUE 32

#define BUTTON_SIMON_RED 27
#define BUTTON_SIMON_YELLOW 29
#define BUTTON_SIMON_GREEN 31
#define BUTTON_SIMON_BLUE 33

enum class Enigma2State {
    INIT,
    SHOW_SEQUENCE,
    WAIT_INPUT,
    CHECK_SEQUENCE,
    LEVEL_COMPLETE,
    FINISHED
};



void initializeEnigma2();
void turnOnLED(int color);
void turnOffLEDs();
bool isButtonPressed();
int getButtonPressed();
bool updateEnigma2();

#endif
