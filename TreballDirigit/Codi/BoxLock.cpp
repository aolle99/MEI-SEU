#include "Arduino.h"
#include "BoxLock.h"

BoxLock::BoxLock() : isOpen(true) {}

void BoxLock::initialize() {
    lockServo.attach(SERVO_PIN);
    lockServo.write(OPEN_ANGLE);
    delay(4000); // Give the servo time to move
    close(); // Ensure the box starts closed
}

void BoxLock::open() {
    if (!isOpen) {
        lockServo.write(OPEN_ANGLE);
        lcd.clear();
        lcd.print("Caixa oberta!");
        Feedback::victoryMelody();
        delay(500); // Give the servo time to move
        isOpen = true;
    }
}

void BoxLock::close() {
    if (isOpen) {
        lockServo.write(CLOSED_ANGLE);
        delay(500); // Give the servo time to move
        isOpen = false;
    }
}

bool BoxLock::isBoxOpen() const {
    return isOpen;
}

