#include "Arduino.h"
#include "TimerController.h"
#include "ArduinoLowPower.h"
#include "config.h"
#include "SDUtils.h"
#include "PageManager.h"

extern PageManager pageManager;

// Static pointer to current instance for interrupt handler
static TimerController* timerInstance = nullptr;

// Static wrapper function for interrupt
static void wakeUpISR() {
    if (timerInstance) {
        timerInstance->wakeUp();
    }
}

void rtcWakeISR() {}

TimerController::TimerController(int screenPin, int wakeUpPin1, int wakeUpPin2):
screenPin(screenPin),
wakeUpPin1(wakeUpPin1),
wakeUpPin2(wakeUpPin2) {}

void TimerController::enterDeepSleep(U8G2& u8g2, RTCZero& rtc){

    // Write current time to the screen
    u8g2.clearBuffer();
    String hh = String(rtc.getHours());
    String mm = String(rtc.getMinutes());
    if (rtc.getMinutes() < 10) mm = '0' + mm;
    if (rtc.getHours() < 10) hh = '0' + hh;

    u8g2.setFont(u8g2_font_fub49_tn);
    u8g2.setCursor(20, 60);
    u8g2.print(hh);

    u8g2.setCursor(20, 120);
    u8g2.print(mm);

    int batLevel = map(constrain(analogRead(A7), MIN_BAT_LEVEL, MAX_BAT_LEVEL), MIN_BAT_LEVEL, MAX_BAT_LEVEL, 0, 100);

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(55, 230);
    u8g2.print(batLevel);
    u8g2.print("%");

    u8g2.sendBuffer();

    // Set sleep period and rtc alarm
    rtc.setAlarmEpoch(rtc.getEpoch() + 20);
    rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);

    LowPower.deepSleep();
    rtc.detachInterrupt();
    rtc.disableAlarm();
}

void TimerController::wakeUp(){
    externalWake = true;
}

void TimerController::begin(RTCZero& rtc){

    Serial.println(SLEEP_TIMER_FILE);
    sleepTimeout = readVal(SLEEP_TIMER_FILE);
    if (sleepTimeout == INVALID_OPTION_VAL){
        sleepTimeout = DEFAULT_SLEEP_TIMEOUT;
    }

    Serial.println(SCREEN_TIMER_FILE);
    screenTimeout = readVal(SCREEN_TIMER_FILE);
    if (screenTimeout == INVALID_OPTION_VAL){
        screenTimeout = DEFAULT_SCREEN_TIMEOUT;
    }

    Serial.println(SCREEN_BRIGHTNESS_FILE);
    screenBrightness = readVal(SCREEN_BRIGHTNESS_FILE);
    if (screenBrightness == INVALID_OPTION_VAL){
        screenBrightness = DEFAULT_SCREEN_BRIGHTNESS;
    }

    pinMode(screenPin, OUTPUT);
    tLastAction = millis();

    timerInstance = this;
    LowPower.attachInterruptWakeup(
        wakeUpPin1,
        wakeUpISR,
        FALLING
    );

    LowPower.attachInterruptWakeup(
        wakeUpPin2,
        wakeUpISR,
        FALLING
    );

    rtc.attachInterrupt(rtcWakeISR);
}

void TimerController::update(InputAction input, U8G2& u8g2, RTCZero& rtc){

    // Send to sleep
    if (millis() - tLastAction > sleepTimeout && !pageManager.pageBlocksSleep()){
        Serial.println(F("Entering deep sleep"));
        
        if (externalWake){
            GPSSerial.print(F("$PMTK161,0*28\r\n"));
            GPSSerial.flush();
            delay(5);
        }
        
        externalWake = false;
        
        while (!externalWake)
            enterDeepSleep(u8g2, rtc);

        // waking up from pin interrupt
        if (externalWake){
            GPSSerial.print(F("\r\n"));
            tLastAction = millis();
            analogWrite(screenPin, screenBrightness);
            screenOn = true;
            Serial.println(F("Turning screen on"));
        }
    }

    // Turn backlight on after interaction
    if (
        input == BUTTON_LEFT_LONG ||
        input == BUTTON_LEFT_SHORT ||
        input == BUTTON_RIGHT_LONG ||
        input == BUTTON_RIGHT_SHORT
    ) {
        tLastAction = millis();
        if (!screenOn){
            analogWrite(screenPin, screenBrightness);
            Serial.println(F("Turning screen on"));
        }
        screenOn = true;
    }


    // Turn backlight off after inactivity
    if (millis() - tLastAction > screenTimeout){
        if (screenOn){
            analogWrite(screenPin, 0);
            Serial.println(F("Turning screen off"));
        }
        screenOn = false;
    }
}

void TimerController::refreshBacklight(){
    analogWrite(screenPin, screenBrightness);
}