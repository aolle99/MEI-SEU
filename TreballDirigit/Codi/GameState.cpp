#include "GameState.h"

GameStatus GameState::currentState = GameStatus::START;

void GameState::initialize() {
    currentState = GameStatus::START;
}

GameStatus GameState::getState() {
    return currentState;
}

void GameState::setState(GameStatus newState) {
    currentState = newState;
}

bool GameState::isGameActive() {
    return currentState != GameStatus::DEFEAT &&
           currentState != GameStatus::FINISHED;
}

