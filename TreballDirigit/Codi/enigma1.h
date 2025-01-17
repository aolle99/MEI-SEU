#ifndef ENIGMA1_H
#define ENIGMA1_H
#include <Arduino.h>
#include <LiquidCrystal.h>

#define LCD_BUTTONS A2

#define LCD_BTN_NONE 0
#define LCD_BTN_RIGHT 1
#define LCD_BTN_UP 2
#define LCD_BTN_DOWN 3
#define LCD_BTN_LEFT 4
#define LCD_BTN_SELECT 5

enum class Enigma1State {
    INIT,
    WAIT_INPUT,
    CHECK_CODE,
    DISPLAY_RESULT,
    FINISHED
};

extern LiquidCrystal lcd;

const int CORRECT_CODE[] = {4, 8, 5, 7};
const int CODE_LENGTH = 4;

bool updateEnigma1();
int readLCDButtons();
void updateDisplay();

#endif
