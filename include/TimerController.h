#pragma once
#include "Input.h"
#include "U8g2lib.h"
#include "RTCZero.h"

class TimerController {
    public:
        TimerController(int screenPin, int wakeUpPin1, int wakeUpPin2);
        void enterDeepSleep(U8G2& u8g2, RTCZero& rtc);
        void update(InputAction input, U8G2& u8g2, RTCZero& rtc);
        void begin(RTCZero& rtc);
        void wakeUp();
        void refreshBacklight();

        unsigned long int screenTimeout;
        unsigned long int sleepTimeout;
        int screenBrightness;

    private:
        int screenPin;
        int wakeUpPin1;
        int wakeUpPin2;
        unsigned long int tLastAction;
        volatile bool externalWake = true;
        bool screenOn = true;
        

};