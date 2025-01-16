#include "BoxLock.h"

BoxLock::BoxLock() : isOpen(false) {}

void BoxLock::initialize() {
    lockServo.attach(SERVO_PIN);
    close(); // Ensure the box starts closed
}

void BoxLock::open() {
  Serial.println("TEST2");
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

