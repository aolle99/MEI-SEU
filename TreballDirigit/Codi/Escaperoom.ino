#include <LiquidCrystal.h>
#include "Timer.h"
#include "GameState.h"
#include "BoxLock.h"
#include "Feedback.h"
#include "Enigma1.h"
#include "Enigma2.h"
#include "Enigma3.h"
#include <IRremote.hpp>

#define CLK 22
#define DIO 23
#define IR_RECEIVER_PIN 38

// LCD configuration
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
TM1637Display display(CLK, DIO);

Timer timer;
BoxLock BoxLock;

void initializeGame() {
  lcd.clear();
  GameState::initialize();
  Feedback::initialize();
  timer.initialize();
  BoxLock.initialize();
}

void setup() {
  // LCD initialization
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);
  initializeGame();
}

void loop() {

  if (GameState::isGameActive()) {
    timer.update();
    switch (GameState::getState()) {
      case GameStatus::START:
        if(updateWelcomeMessage()) {
          GameState::setState(GameStatus::ENIGMA1);
        }
        break;
      case GameStatus::ENIGMA1:
        if (updateEnigma1()) {
          GameState::setState(GameStatus::ENIGMA2);
        }
        break;
      case GameStatus::ENIGMA2:
        if (updateEnigma2()) {
          GameState::setState(GameStatus::ENIGMA3);
        }
        break;
      case GameStatus::ENIGMA3:
        if (updateEnigma3()) {
          GameState::setState(GameStatus::VICTORY);
        }
        break;
      case GameStatus::VICTORY:
        openBox();
        GameState::setState(GameStatus::FINISHED);
        break;
      default:
        break;
    }
  }
  handleRemoteControl();
}

enum class WelcomeState {
    INIT,
    FIRST_LINE,
    SECOND_LINE,
    THIRD_LINE,
    LEDS_ON,
    LEDS_OFF,
    FINISHED
};

WelcomeState welcomeState = WelcomeState::INIT;
unsigned long lastStateChange = 0;

bool updateWelcomeMessage() {
    unsigned long currentTime = millis();

    switch (welcomeState) {
        case WelcomeState::INIT:
            lastStateChange = currentTime;
            welcomeState = WelcomeState::FIRST_LINE;
            break;

        case WelcomeState::FIRST_LINE:
            if (currentTime - lastStateChange >= 100) {
                lcd.setCursor(0, 0);
                lcd.print("Benvingut al");
                lcd.setCursor(0, 1);
                lcd.print("Escaperoom!");
                lastStateChange = currentTime;
                welcomeState = WelcomeState::SECOND_LINE;
            }
            break;

        case WelcomeState::SECOND_LINE:
            if (currentTime - lastStateChange >= 2000) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Supera");
                lcd.setCursor(0, 1);
                lcd.print("les proves");
                lastStateChange = currentTime;
                welcomeState = WelcomeState::THIRD_LINE;
            }
            break;
        case WelcomeState::THIRD_LINE:
          if (currentTime - lastStateChange >= 2000) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("i obriras");
              lcd.setCursor(0, 1);
              lcd.print("la caixa");
              lastStateChange = currentTime;
              welcomeState = WelcomeState::LEDS_ON;
          }
          break;

        case WelcomeState::LEDS_ON:
            Feedback::greenLedOn();
            Feedback::redLedOn();
            lastStateChange = currentTime;
            welcomeState = WelcomeState::LEDS_OFF;
            break;

        case WelcomeState::LEDS_OFF:
            if (currentTime - lastStateChange >= 2000) {
                Feedback::greenLedOff();
                Feedback::redLedOff();
                welcomeState = WelcomeState::FINISHED;
            }
            break;

        case WelcomeState::FINISHED:
            return true;
    }

    return false;
}


void openBox() {
  BoxLock.open();
  lcd.clear();
  lcd.print("Caixa oberta!");
  Feedback::greenLedOn();
  Feedback::redLedOff();
  Feedback::victoryMelody();
}

void handleRemoteControl() {
  if (IrReceiver.decode()) {
    uint16_t lastCode = IrReceiver.decodedIRData.command;
    IrReceiver.resume();
    switch (lastCode) {
      case 69:
        lcd.clear();
        lcd.print("Reiniciant...");
        delay(1000);
        initializeGame();
        break;
      case 70:
        GameState::setState(GameStatus::VICTORY);
        lcd.clear();
        lcd.print("Joc Acabat");
        Feedback::redLedOn();
        Feedback::greenLedOff();
        break;
      case 71:
        GameStatus currentState = GameState::getState();
        if (currentState != GameStatus::VICTORY && currentState != GameStatus::FINISHED) {
          GameState::setState(static_cast<GameStatus>(static_cast<int>(currentState) + 1));
          lcd.clear();
          lcd.print("Seguent nivell");
          delay(1000);
        }
        break;
    }
  }
}
