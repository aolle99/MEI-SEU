#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "pitches.h"
#include <Arduino.h>

class Feedback {
private:
    static const int BUZZER = 37;
    static const int GREEN_LED = 25;
    static const int RED_LED = 24;

    static void playMelody(int* melody, int* durations, int length);

public:
    static void initialize();
    static void playSound(int pitch, int duration);
    static void startMelody();
    static void successMelody();
    static void failureMelody();
    static void victoryMelody();
    
    static void greenLedOn();
    static void greenLedOff();
    static void redLedOn();
    static void redLedOff();
};

#endif
