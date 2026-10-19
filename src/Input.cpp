#include "Input.h"
#include "config.h"
#include "SDUtils.h"

Input::Input(int pinButtonLeft, int pinButtonRight):
    pinButtonLeft(pinButtonLeft),
    pinButtonRight(pinButtonRight) {}

void Input::begin(){

    const char* BTN_TIMER_FILE = "conf/hld_btn.txt";

    buttonHoldTime = readVal(BTN_TIMER_FILE);
    if (buttonHoldTime == INVALID_OPTION_VAL){
        buttonHoldTime = DEFAULT_BUTTON_HOLD_TIME;
        Serial.println(F("Using default value"));
    }

    buttonRight.attach(pinButtonRight, INPUT_PULLUP);
    buttonLeft.attach(pinButtonLeft, INPUT_PULLUP);

    buttonLeft.interval(5);
    buttonRight.interval(5);
}

void Input::update(){
    buttonLeft.update();
    buttonRight.update();

    if (buttonRight.read() == LOW && buttonRightReady){
        buttonRightDuration = buttonRight.currentDuration();
        if (buttonRightDuration > buttonHoldTime){
            buttonRightReady = false;
            input = button_status{BUTTON_RIGHT_LONG, 0};
            Serial.println(F("Long right press"));
            return;
        }
        else {
            input = button_status{BUTTON_RIGHT_HOLD, buttonRightDuration};
            return;
            }    
        }

    if (buttonLeft.read() == LOW && buttonLeftReady == true){
        buttonLeftDuration = buttonLeft.currentDuration(); 
        if (buttonLeftDuration > buttonHoldTime){
            buttonLeftReady = false;
            input = button_status{BUTTON_LEFT_LONG, 0};
            Serial.println(F("Long left press"));
            return;
        }
        else {
            input = button_status{BUTTON_LEFT_HOLD, buttonLeftDuration};
            return;
            }
        }

    if (buttonRight.rose()){
        buttonRightReady = true;
        buttonRightDuration = buttonRight.previousDuration();
        if (buttonRightDuration < buttonHoldTime){
            input = button_status{BUTTON_RIGHT_SHORT, 0};
            Serial.println(F("Short right press"));
            return;
            }
        }

    if (buttonLeft.rose()){
        buttonLeftReady = true;
        buttonLeftDuration = buttonLeft.previousDuration();
        if (buttonLeftDuration < buttonHoldTime){
            input = button_status{BUTTON_LEFT_SHORT, 0};
            Serial.println(F("Short left press"));
            return;
            }
        }

    input = button_status{NO_ACTION, 0};

}