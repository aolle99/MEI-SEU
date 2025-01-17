#include "Timer.h"

Timer::Timer()
    : minutes(INITIAL_TIME), seconds(0), previousTime(0), interval(1000) {}

void Timer::initialize() {
    display.setBrightness(0x0f);
}

void Timer::update() {
    unsigned long currentTime = millis();
  
    if (currentTime - previousTime >= interval) {
        previousTime = currentTime;
    
        seconds--;
    
        if (seconds < 0) {
            minutes--;
            seconds = 59;
        }
    
        displayTime();
        if(hasFinished()) {
          GameState::setState(GameStatus::DEFEAT);
        }
    }
}

bool Timer::hasFinished() const {
    return (minutes == 0 && seconds == 0);
}

void Timer::displayTime() const {
    int timeDisplay = minutes * 100 + seconds;
    display.showNumberDecEx(timeDisplay, 0b01000000, true);
}

int Timer::getMinutes() const {
    return minutes;
}

int Timer::getSeconds() const {
    return seconds;
}
