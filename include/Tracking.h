#include <Arduino.h>
#include <TinyGPS++.h>
#include <RTCZero.h>
#include "config.h"

void tracking(RTCZero& rtc, TinyGPSPlus& gps){
  while (GPSSerial.available() > 0) {
    char c = GPSSerial.read();
    // Serial.write(c);
    if (gps.encode(c)){

      // Update rtc
      rtc.setHours(gps.time.hour());
      rtc.setMinutes(gps.time.minute());
      rtc.setSeconds(gps.time.second());

      rtc.setDay(gps.date.day());
      rtc.setMonth(gps.date.month());
      rtc.setYear(gps.date.year() - 2000);
    //   rtc.setEpoch(rtc.getEpoch()+3600);
      }
    }
  }