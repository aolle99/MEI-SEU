#include "Arduino.h"
#include "Enigma1.h"

Enigma1State enigma1State = Enigma1State::INIT;
int currentCode[CODE_LENGTH] = {0, 0, 0, 0};
int currentPosition = 0;
unsigned long lastInputTime = 0;
const unsigned long inputDelay = 200;
unsigned long resultDisplayTime = 0;
const unsigned long resultDisplayDuration = 2000;

bool updateEnigma1() {
    switch (enigma1State) {
        case Enigma1State::INIT:
            lcd.setCursor(0, 0);
            lcd.print("Enter code:     ");
            lcd.setCursor(0, 1);
            lcd.print("0 0 0 0         ");
            enigma1State = Enigma1State::WAIT_INPUT;
            break;

        case Enigma1State::WAIT_INPUT:
            if (millis() - lastInputTime > inputDelay) {
                int button = readLCDButtons();
                switch (button) {
                    case LCD_BTN_RIGHT:
                        currentPosition = (currentPosition + 1) % CODE_LENGTH;
                        updateDisplay();
                        break;
                    case LCD_BTN_LEFT:
                        currentPosition = (currentPosition - 1 + CODE_LENGTH) % CODE_LENGTH;
                        updateDisplay();
                        break;
                    case LCD_BTN_UP:
                        currentCode[currentPosition] = (currentCode[currentPosition] + 1) % 10;
                        updateDisplay();
                        break;
                    case LCD_BTN_DOWN:
                        currentCode[currentPosition] = (currentCode[currentPosition] - 1 + 10) % 10;
                        updateDisplay();
                        break;
                    case LCD_BTN_SELECT:
                        enigma1State = Enigma1State::CHECK_CODE;
                        break;
                }

                lastInputTime = millis();
            }
            break;

        case Enigma1State::CHECK_CODE:
            bool codeCorrect = true;
            for (int i = 0; i < CODE_LENGTH; i++) {
                if (currentCode[i] != CORRECT_CODE[i]) {
                    codeCorrect = false;
                    break;
                }
            }
            lcd.clear();
            lcd.print(codeCorrect ? "Correct code!" : "Incorrect code");
            resultDisplayTime = millis();
            if(!codeCorrect) {
              delay(2000);
              enigma1State = Enigma1State::WAIT_INPUT;
            }
            else {
            enigma1State = Enigma1State::DISPLAY_RESULT;
            }
            break; 

        case Enigma1State::DISPLAY_RESULT:
            if (millis() - resultDisplayTime > resultDisplayDuration) {
                enigma1State = Enigma1State::FINISHED;
            }
            break;

        case Enigma1State::FINISHED:
            return true;
    }

    return false;
}

void updateDisplay() {
    lcd.setCursor(0, 1);
    for (int i = 0; i < CODE_LENGTH; i++) {
        lcd.print(String(currentCode[i]));
        lcd.print(" ");
    }
}

int readLCDButtons() {
    int adc_key_in = analogRead(LCD_BUTTONS);

    if (adc_key_in >= 600) return LCD_BTN_NONE;
    if (adc_key_in < 90) return LCD_BTN_RIGHT;
    if (adc_key_in < 200) return LCD_BTN_UP;
    if (adc_key_in < 350) return LCD_BTN_DOWN;
    if (adc_key_in < 450) return LCD_BTN_LEFT;
    if (adc_key_in < 600) return LCD_BTN_SELECT;
}
