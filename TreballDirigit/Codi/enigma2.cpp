#include "Enigma2.h"

const int SEQUENCE_LENGTH = 5;
int sequence[SEQUENCE_LENGTH];
int userSequence[SEQUENCE_LENGTH];
int currentLevel = 0;
int currentStep = 0;
Enigma2State enigma2State = Enigma2State::INIT;
unsigned long lastActionTime = 0;

void initializeEnigma2() {
    pinMode(LED_SIMON_GREEN, OUTPUT);
    pinMode(LED_SIMON_RED, OUTPUT);
    pinMode(LED_SIMON_BLUE, OUTPUT);
    pinMode(LED_SIMON_YELLOW, OUTPUT);
    
    pinMode(BUTTON_SIMON_GREEN, INPUT_PULLUP);
    pinMode(BUTTON_SIMON_RED, INPUT_PULLUP);
    pinMode(BUTTON_SIMON_BLUE, INPUT_PULLUP);
    pinMode(BUTTON_SIMON_YELLOW, INPUT_PULLUP);
    
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        sequence[i] = random(4);
    }
    enigma2State = Enigma2State::SHOW_SEQUENCE;
    currentLevel = 0;
    currentStep = 0;
}

void turnOnLED(int color) {
  switch(color) {
    case 0: digitalWrite(LED_SIMON_GREEN, HIGH); break;
    case 1: digitalWrite(LED_SIMON_RED, HIGH); break;
    case 2: digitalWrite(LED_SIMON_BLUE, HIGH); break;
    case 3: digitalWrite(LED_SIMON_YELLOW, HIGH); break;
  }
}

void turnOffLEDs() {
  digitalWrite(LED_SIMON_GREEN, LOW);
  digitalWrite(LED_SIMON_RED, LOW);
  digitalWrite(LED_SIMON_BLUE, LOW);
  digitalWrite(LED_SIMON_YELLOW, LOW);
}

bool isButtonPressed() {
  return digitalRead(BUTTON_SIMON_GREEN) == LOW ||
         digitalRead(BUTTON_SIMON_RED) == LOW ||
         digitalRead(BUTTON_SIMON_BLUE) == LOW ||
         digitalRead(BUTTON_SIMON_YELLOW) == LOW;
}

int getButtonPressed() {
  while (true) {
    if (digitalRead(BUTTON_SIMON_GREEN) == LOW) return 0;
    if (digitalRead(BUTTON_SIMON_RED) == LOW) return 1;
    if (digitalRead(BUTTON_SIMON_BLUE) == LOW) return 2;
    if (digitalRead(BUTTON_SIMON_YELLOW) == LOW) return 3;
  }
}

void showSequence() {
  for (int i = 0; i <= currentLevel; i++) {
    turnOnLED(sequence[i]);
    delay(800);
    turnOffLEDs();
    delay(250);
  }
}

bool checkSequence() {
  for (int i = 0; i <= currentLevel; i++) {
    if (userSequence[i] != sequence[i]) {
      return false;
    }
  }
  return true;
}

bool updateEnigma2() {
    switch (enigma2State) {
        case Enigma2State::INIT:
            initializeEnigma2();
            break;

        case Enigma2State::SHOW_SEQUENCE:
            if (millis() - lastActionTime > 250) {
                if (currentStep <= currentLevel) {
                    if (currentStep % 2 == 0) {
                        turnOnLED(sequence[currentStep / 2]);
                    } else {
                        turnOffLEDs();
                    }
                    currentStep++;
                    lastActionTime = millis();
                } else {
                    enigma2State = Enigma2State::WAIT_INPUT;
                    currentStep = 0;
                }
            }
            break;

        case Enigma2State::WAIT_INPUT:
            if (isButtonPressed()) {
                int button = getButtonPressed();
                userSequence[currentStep] = button;
                turnOnLED(button);
                lastActionTime = millis();
                enigma2State = Enigma2State::CHECK_SEQUENCE;
            }
            break;

        case Enigma2State::CHECK_SEQUENCE:
            if (millis() - lastActionTime > 200) {
                turnOffLEDs();
                if (userSequence[currentStep] != sequence[currentStep]) {
                    currentLevel = 0;
                    currentStep = 0;
                    enigma2State = Enigma2State::SHOW_SEQUENCE;
                } else {
                    currentStep++;
                    if (currentStep > currentLevel) {
                        enigma2State = Enigma2State::LEVEL_COMPLETE;
                    } else {
                        enigma2State = Enigma2State::WAIT_INPUT;
                    }
                }
            }
            break;

        case Enigma2State::LEVEL_COMPLETE:
            currentLevel++;
            if (currentLevel == SEQUENCE_LENGTH) {
                enigma2State = Enigma2State::FINISHED;
            } else {
                currentStep = 0;
                enigma2State = Enigma2State::SHOW_SEQUENCE;
            }
            break;

        case Enigma2State::FINISHED:
            return true;
    }

    return false;
}
