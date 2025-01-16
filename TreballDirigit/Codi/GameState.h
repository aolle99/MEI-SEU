#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Arduino.h>

enum class GameStatus {
    START,
    ENIGMA1,
    ENIGMA2,
    ENIGMA3,
    VICTORY,
    DEFEAT,
    FINISHED
};

class GameState {
private:
    static GameStatus currentState;


public:
    static void initialize();
    static GameStatus getState();
    static void setState(GameStatus newState);
    static bool isGameActive();
};

#endif // GAME_STATE_H
