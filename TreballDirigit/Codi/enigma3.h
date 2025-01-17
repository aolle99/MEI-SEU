#ifndef ENIGMA3_H
#define ENIGMA3_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#define JOYSTICK_X A0
#define JOYSTICK_Y A1

extern LiquidCrystal lcd;

void initializeEnigma3();
bool isValidMove(int x, int y);
void movePlayer();
bool checkSolution();
bool updateEnigma3();
void drawMap();

#endif
