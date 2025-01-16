#include "Feedback.h"

void Feedback::initialize() {
    pinMode(BUZZER, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    startMelody();
}

void Feedback::playSound(int pitch, int duration) {
  tone(BUZZER, pitch, duration);
  delay(duration);
  noTone(BUZZER);
}


void Feedback::playMelody(int* melody, int* durations, int length) {
    for (int note = 0; note < length; note++) {
        int noteDuration = 1000 / durations[note];
        tone(BUZZER, melody[note], noteDuration);
        
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        
        noTone(BUZZER);
    }
}

void Feedback::startMelody() {
    int melody[] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5};
    int durations[] = {4, 4, 4, 4};
    playMelody(melody, durations, 4);
}

void Feedback::successMelody() {
    int melody[] = {NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4};
    int durations[] = {8, 8, 8, 8};
    playMelody(melody, durations, 4);
}

void Feedback::failureMelody() {
    int melody[] = {NOTE_C4, NOTE_B4, NOTE_A4, NOTE_G4};
    int durations[] = {4, 4, 4, 4};
    playMelody(melody, durations, 4);
}

void Feedback::victoryMelody() {
    int melody[] = {
        NOTE_C5, NOTE_C5, NOTE_C5, 
        NOTE_G4, NOTE_E4, NOTE_D4, 
        NOTE_C5, NOTE_G4, NOTE_E4, NOTE_D4,
        NOTE_C5, NOTE_G4, NOTE_E4, NOTE_D4
    };
    int durations[] = {
        4, 4, 4, 
        8, 8, 8, 
        4, 8, 8, 8,
        4, 8, 8, 8
    };
    playMelody(melody, durations, 14);
}

void Feedback::greenLedOn() {
    digitalWrite(GREEN_LED, HIGH);
}

void Feedback::greenLedOff() {
    digitalWrite(GREEN_LED, LOW);
}

void Feedback::redLedOn() {
    digitalWrite(RED_LED, HIGH);
}

void Feedback::redLedOff() {
    digitalWrite(RED_LED, LOW);
}
