#include "Arduino.h"
#include "Enigma1.h"

Enigma1State enigma1State = Enigma1State::INIT;
int currentCode[CODE_LENGTH] = {0, 0, 0, 0};
int currentPosition = 0;
unsigned long lastInputTime = 0;
const unsigned long inputDelay = 200;
unsigned long resultDisplayTime = 0;
const unsigned long resultDisplayDuration = 2000;
bool isCorrect = true;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500; // 500ms para el intervalo de parpadeo
bool isDigitVisible = true;

bool updateEnigma1() {
    switch (enigma1State) {
        case Enigma1State::INIT:
            lcd.clear()
            lcd.setCursor(0, 0);
            lcd.print("Codi:");
            lcd.setCursor(0, 1);
            lcd.print("0 0 0 0");
            enigma1State = Enigma1State::WAIT_INPUT;
            break;

        case Enigma1State::WAIT_INPUT:
            if (millis() - lastInputTime > inputDelay) {
                int button = readLCDButtons();
                switch (button) {
                    case LCD_BTN_RIGHT:
                        currentPosition = (currentPosition + 1) % CODE_LENGTH;
                        break;
                    case LCD_BTN_LEFT:
                        currentPosition = (currentPosition - 1 + CODE_LENGTH) % CODE_LENGTH;
                        break;
                    case LCD_BTN_UP:
                        currentCode[currentPosition] = (currentCode[currentPosition] + 1) % 10;
                        break;
                    case LCD_BTN_DOWN:
                        currentCode[currentPosition] = (currentCode[currentPosition] - 1 + 10) % 10;
                        break;
                    case LCD_BTN_SELECT:
                        enigma1State = Enigma1State::CHECK_CODE;
                        break;
                }
                lastInputTime = millis();
            }
            updateDisplay();
            break;

        case Enigma1State::CHECK_CODE:
            for (int i = 0; i < CODE_LENGTH; i++) {
                if (currentCode[i] != CORRECT_CODE[i]) {
                    isCorrect = false;
                    break;
                }
            }
            lcd.clear();
            lcd.print(isCorrect ? "Codi Correcte!" : "codi Incorrecte");
            resultDisplayTime = millis();
            enigma1State = Enigma1State::DISPLAY_RESULT;
            break; 

        case Enigma1State::DISPLAY_RESULT:
            if (millis() - resultDisplayTime > resultDisplayDuration) {
              if(isCorrect) enigma1State = Enigma1State::FINISHED;
              else enigma1State = Enigma1State::INIT;
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
        if (i == currentPosition && millis() - lastBlinkTime > blinkInterval) {
            isDigitVisible = !isDigitVisible;
            lastBlinkTime = millis();
        }
        
        if (i == currentPosition && !isDigitVisible) {
            lcd.print("  ");
        } else {
            lcd.print(String(currentCode[i]));
            lcd.print(" ");
        }
    }
}

int readLCDButtons() {
    int adc_key_in = analogRead(LCD_BUTTONS);
    Serial.println(adc_key_in);
    if (adc_key_in >= 1020) return LCD_BTN_NONE;
    if (adc_key_in < 100) return LCD_BTN_RIGHT;
    if (adc_key_in < 300) return LCD_BTN_UP;
    if (adc_key_in < 550) return LCD_BTN_DOWN;
    if (adc_key_in < 800) return LCD_BTN_LEFT;
    if (adc_key_in < 1020) return LCD_BTN_SELECT;
}
