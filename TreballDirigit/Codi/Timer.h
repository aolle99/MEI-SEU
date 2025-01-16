#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include <TM1637Display.h>
#include <LiquidCrystal.h>

// 7-segment display configuration


#define INITIAL_TIME 10

extern TM1637Display display;

extern LiquidCrystal lcd;

class Timer {
private:
    int minutes;
    int seconds;
    unsigned long previousTime;
    const long interval;

public:
    Timer();
    void initialize();
    void update();
    bool hasFinished() const;
    void displayTime() const;
    int getMinutes() const;
    int getSeconds() const;
};

#endif // TIMER_H
