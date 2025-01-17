#ifndef BOX_LOCK_H
#define BOX_LOCK_H

#include <Arduino.h>
#include <Servo.h>
#include "Feedback.h"
#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

class BoxLock {
private:
    static const int SERVO_PIN = 2;
    static const int OPEN_ANGLE = 180;
    static const int CLOSED_ANGLE = 90;
    
    Servo lockServo;
    bool isOpen=true;

public:
    BoxLock();
    void initialize();
    void open();
    void close();
    bool isBoxOpen() const;
};

#endif // BOX_LOCK_H
