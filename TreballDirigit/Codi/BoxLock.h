#ifndef BOX_LOCK_H
#define BOX_LOCK_H

#include <Arduino.h>
#include <Servo.h>
#include "Feedback.h"
#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

class BoxLock {
private:
    static const int SERVO_PIN = 36;
    static const int OPEN_ANGLE = 90;
    static const int CLOSED_ANGLE = 0;
    
    Servo lockServo;
    bool isOpen;

public:
    BoxLock();
    void initialize();
    void open();
    void close();
    bool isBoxOpen() const;
};

#endif // BOX_LOCK_H
