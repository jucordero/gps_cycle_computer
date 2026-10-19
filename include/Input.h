#pragma once
#include "Bounce2.h"
#include "config.h"

enum InputAction {
    BUTTON_LEFT_LONG,
    BUTTON_LEFT_SHORT,
    BUTTON_RIGHT_LONG,
    BUTTON_RIGHT_SHORT,
    BUTTON_LEFT_HOLD,
    BUTTON_RIGHT_HOLD,
    NO_ACTION
};

struct button_status{
    InputAction action;
    unsigned long int duration;
};

class Input {
    public:
        Input(int pinButtonLeft, int pinButtonRight);
        void begin();
        void update();
        button_status input = button_status{NO_ACTION, 0};
        unsigned long int buttonHoldTime;

    private:
        Bounce buttonRight = Bounce();
        Bounce buttonLeft = Bounce();
        int pinButtonLeft;
        int pinButtonRight;
        boolean buttonLeftReady = true;
        boolean buttonRightReady = true;
        unsigned long int buttonLeftDuration;
        unsigned long int buttonRightDuration;
        
};


