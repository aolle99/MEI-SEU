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

const int MAP_WIDTH = 32;
const int MAP_HEIGHT = 2;
const int SCREEN_WIDTH = 16;
const int SCREEN_HEIGHT = 2;

int posX = 0, posY = 0;
int targetX = 31, targetY = 1;
int viewX = 0;
Enigma3State enigma3State = Enigma3State::INIT;
unsigned long lastMoveTime = 0;
const unsigned long moveDelay = 200; // Cooldown de 200ms entre movimientos

// Definir el mapa más grande
char gameMap[MAP_HEIGHT][MAP_WIDTH+1] = {
    "---#---###---#------#---#---#---",
    "-----#-----#---####---#---#---#X"
};

void initializeEnigma3() {
    lcd.clear();
    lcd.createChar(0, playerChar);
    posX = 0;
    posY = 0;
    viewX = 0;
    enigma3State = Enigma3State::MOVE_PLAYER;
    drawMap();
}

void drawMap() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        lcd.setCursor(0, y);
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (x + viewX == posX && y == posY) {
                lcd.write(byte(0)); // Dibujar jugador
            } else {
              if(gameMap[y][x + viewX] =='-') {
                lcd.write(" ");
              }
              else {
                lcd.write(gameMap[y][x + viewX]);
              }
            }
        }
    }
}

void movePlayer() {
    if (millis() - lastMoveTime < moveDelay) {
        return; // Aún no ha pasado suficiente tiempo desde el último movimiento
    }

    int x = analogRead(JOYSTICK_X);
    int y = analogRead(JOYSTICK_Y);
    
    int newPosX = posX;
    int newPosY = posY;
    if (x < 300 && posX > 0) newPosX--;
    if (x > 800 && posX < MAP_WIDTH - 1) newPosX++;
    if (y < 300 && posY > 0) newPosY--;
    if (y > 800 && posY < MAP_HEIGHT - 1) newPosY++;
    
    if (gameMap[newPosY][newPosX] != '#') {
        posX = newPosX;
        posY = newPosY;
        
        // Actualizar la vista si el jugador se acerca a los bordes
        if (posX - viewX < 3 && viewX > 0) viewX--;
        if (posX - viewX > SCREEN_WIDTH - 4 && viewX < MAP_WIDTH - SCREEN_WIDTH) viewX++;
        
        drawMap();
        lastMoveTime = millis();
    }
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
            movePlayer();
            enigma3State = Enigma3State::CHECK_SOLUTION;
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
