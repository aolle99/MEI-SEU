#include "Enigma2.h"
#include "Feedback.h"

enum class Enigma2State {
  INIT,
  SHOW_SEQUENCE,
  WAIT_INPUT,
  CHECK_SEQUENCE,
  LEVEL_COMPLETE,
  FINISHED
};

const int MAX_SEQUENCE_LENGTH = 6;  // Maximum sequence length
int sequence[MAX_SEQUENCE_LENGTH];
int userSequence[MAX_SEQUENCE_LENGTH];
int currentLevel = 0;
int currentStep = 0;
int currentSequenceLength = 3;  // Start with 3 LEDs
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

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Repeteix la");
  lcd.setCursor(0, 1);
  lcd.print("sequencia");

  for (int i = 0; i < MAX_SEQUENCE_LENGTH; i++) {
    sequence[i] = random(4);  // Randomize sequence
  }
  enigma2State = Enigma2State::SHOW_SEQUENCE;
  currentLevel = 0;
  currentStep = 0;
}

void playLED(int color) {
  turnOnLED(color);
  switch (color) {
    case 0: Feedback::playSound(262, 2); break;  // DO
    case 1: Feedback::playSound(294, 2); break;  // RE
    case 2: Feedback::playSound(330, 2); break;  // MI
    case 3: Feedback::playSound(349, 2); break;  // FA
  }
  delay(300);
  turnOffLEDs();
  delay(100);
}

void turnOnLED(int color) {
  switch (color) {
    case 0: digitalWrite(LED_SIMON_GREEN, HIGH); break;
    case 1: digitalWrite(LED_SIMON_RED, HIGH); break;
    case 2: digitalWrite(LED_SIMON_BLUE, HIGH); break;
    case 3: digitalWrite(LED_SIMON_YELLOW, HIGH); break;
  }
}

void turnOnLEDs() {
  digitalWrite(LED_SIMON_GREEN, HIGH);
  digitalWrite(LED_SIMON_RED, HIGH);
  digitalWrite(LED_SIMON_BLUE, HIGH);
  digitalWrite(LED_SIMON_YELLOW, HIGH);
}

void turnOffLEDs() {
  digitalWrite(LED_SIMON_GREEN, LOW);
  digitalWrite(LED_SIMON_RED, LOW);
  digitalWrite(LED_SIMON_BLUE, LOW);
  digitalWrite(LED_SIMON_YELLOW, LOW);
}

bool isButtonPressed() {
  return digitalRead(BUTTON_SIMON_GREEN) == LOW || digitalRead(BUTTON_SIMON_RED) == LOW || digitalRead(BUTTON_SIMON_BLUE) == LOW || digitalRead(BUTTON_SIMON_YELLOW) == LOW;
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
      if (millis() - lastActionTime > 500) {
        if (currentStep < currentSequenceLength) {
          playLED(sequence[currentStep]);
          currentStep++;
        } else {
          enigma2State = Enigma2State::WAIT_INPUT;
          currentStep = 0;
        }
        lastActionTime = millis();
      }
      break;

    case Enigma2State::WAIT_INPUT:
      if (isButtonPressed()) {
        int button = getButtonPressed();
        userSequence[currentStep] = button;
        playLED(button);
        currentStep++;
        enigma2State = Enigma2State::CHECK_SEQUENCE;
      }
      break;

    case Enigma2State::CHECK_SEQUENCE:
      if (userSequence[currentStep - 1] != sequence[currentStep - 1]) {
        Feedback::failureMelody();
        turnOnLEDs();
        delay(1000);
        turnOffLEDs();
        currentLevel = 0;
        currentSequenceLength = 3;  // Reset to initial length
        currentStep=0;
        enigma2State = Enigma2State::SHOW_SEQUENCE;
      } else if (currentStep >= currentSequenceLength) {
        Feedback::successMelody();
        enigma2State = Enigma2State::LEVEL_COMPLETE;
      } else {
        enigma2State = Enigma2State::WAIT_INPUT;
      }
      break;

    case Enigma2State::LEVEL_COMPLETE:
      currentLevel++;
      if (currentLevel == MAX_SEQUENCE_LENGTH - 3) {  // When all levels are complete
        Feedback::victoryMelody();
        turnOnLEDs();
        delay(2000);
        turnOffLEDs();
        enigma2State = Enigma2State::FINISHED;
      } else {
        currentSequenceLength++;  // Increase sequence length up to MAX_SEQUENCE_LENGTH
        currentStep = 0;
        enigma2State = Enigma2State::SHOW_SEQUENCE;
      }
      break;

    case Enigma2State::FINISHED:
      return true;
  }

  return false;
}