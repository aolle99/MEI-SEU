#include "Enigma3.h"

enum class Enigma3State {
    INIT,
    MOVE_PLAYER,
    CHECK_SOLUTION,
    SOLVED,
    FINISHED
};

byte playerChar[8] = {
    B00100, B01110, B00100, B01010,
    B00100, B00100, B01010, B01010
};

int posX = 0, posY = 0;
int targetX = 15, targetY = 1;
Enigma3State enigma3State = Enigma3State::INIT;
unsigned long lastMoveTime = 0;
const unsigned long moveDelay = 100;

void initializeEnigma3() {
    lcd.clear();
    lcd.createChar(0, playerChar);
    lcd.setCursor(targetX, targetY);
    lcd.write('X');
    posX = 0;
    posY = 0;
    enigma3State = Enigma3State::MOVE_PLAYER;
}

void movePlayer() {
    int x = analogRead(JOYSTICK_X);
    int y = analogRead(JOYSTICK_Y);
    
    lcd.setCursor(posX, posY);
    lcd.print(' ');

    Serial.println(x);
    
    if (x < 300 && posX > 0) posX--;
    if (x > 700 && posX < 15) posX++;
    if (y < 300 && posY > 0) posY--;
    if (y > 700 && posY < 1) posY++;
    
    lcd.setCursor(posX, posY);
    lcd.write(byte(0));
}

bool checkSolution() {
    return (posX == targetX && posY == targetY);
}

bool updateEnigma3() {
    switch (enigma3State) {
        case Enigma3State::INIT:
            initializeEnigma3();
            break;
        
        case Enigma3State::MOVE_PLAYER:
            if (millis() - lastMoveTime >= moveDelay) {
                movePlayer();
                lastMoveTime = millis();
                enigma3State = Enigma3State::CHECK_SOLUTION;
            }
            break;
        
        case Enigma3State::CHECK_SOLUTION:
            if (checkSolution()) {
                enigma3State = Enigma3State::SOLVED;
            } else {
                enigma3State = Enigma3State::MOVE_PLAYER;
            }
            break;
        
        case Enigma3State::SOLVED:
            lcd.clear();
            lcd.print("Enigma solved!");
            enigma3State = Enigma3State::FINISHED;
            break;
        
        case Enigma3State::FINISHED:
            return true;
    }
    
    return false;
}
