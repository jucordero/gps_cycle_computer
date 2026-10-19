#include "DebugPage.h"
#include "PageManager.h"
#include "Pages.h"
#include "DrawUtils.h"
#include "SD.h"
#include "TinyGPS++.h"
#include "RTCZero.h"
#include "SDUtils.h"
#include "TimerController.h"
#include "Input.h"

extern PageManager pageManager;
extern TinyGPSPlus gps;
extern RTCZero rtc;
extern TimerController timer;
extern Input input;

void DebugPage::draw(U8G2& u8g2) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenfatguys_tf);
  u8g2.setCursor(5,20);
  u8g2.print(F("Debug"));
  String menuItems[] = {"GPS", "RTC", "SD", "SYSTEM"};
  draw_selection_list(u8g2, menuItems, itemsListLength, menuItemIndex);
  draw_info(u8g2);

  switch (shownData)
  {
  case GPS_DATA:
    u8g2.setFont(u8g2_font_6x10_tf);

    u8g2.setCursor(5, 150);
    u8g2.print(F("Speed"));
    u8g2.setCursor(65, 150);
    u8g2.print(gps.speed.kmph(), 2);

    u8g2.setCursor(5, 170);
    u8g2.print(F("Lat"));
    u8g2.setCursor(65, 170);
    u8g2.print(gps.location.lat(), 2);

    u8g2.setCursor(5, 190);
    u8g2.print(F("Lon"));
    u8g2.setCursor(65, 190);
    u8g2.print(gps.location.lng(), 2);

    u8g2.setCursor(5, 210);
    u8g2.print(F("Sats"));
    u8g2.setCursor(65, 210);
    u8g2.print(gps.satellites.value());
    break;

  case RTC_DATA:
    u8g2.setFont(u8g2_font_6x10_tf);

    u8g2.setCursor(5, 150);
    u8g2.print(F("Day"));
    u8g2.setCursor(65, 150);
    u8g2.print(rtc.getDay());

    u8g2.setCursor(5, 170);
    u8g2.print(F("Month"));
    u8g2.setCursor(65, 170);
    u8g2.print(rtc.getMonth());

    u8g2.setCursor(5, 190);
    u8g2.print(F("Year"));
    u8g2.setCursor(65, 190);
    u8g2.print(rtc.getYear());

    break;

  case SD_DATA:
    break;

  case SYSTEM_DATA:
    u8g2.setFont(u8g2_font_6x10_tf);

    u8g2.setCursor(5, 130);
    u8g2.print(F("Battery"));
    u8g2.setCursor(65, 130);
    u8g2.print(analogRead(A7));

    u8g2.setCursor(5, 150);
    u8g2.print(F("SCR T_out"));
    u8g2.setCursor(65, 150);
    u8g2.print(timer.screenTimeout);

    u8g2.setCursor(5, 170);
    u8g2.print(F("SLP T_out"));
    u8g2.setCursor(65, 170);
    u8g2.print(timer.sleepTimeout);

    u8g2.setCursor(5, 190);
    u8g2.print(F("Btn Hold"));
    u8g2.setCursor(65, 190);
    u8g2.print(input.buttonHoldTime);

    u8g2.setCursor(5, 210);
    u8g2.print(F("SCR Bgt"));
    u8g2.setCursor(65, 210);
    u8g2.print(timer.screenBrightness);

    break;;
  
  default:
    break;
  }

  u8g2.sendBuffer();
}

void DebugPage::handleInput(Input& input){
  
  switch (input.input.action)
  {
  case BUTTON_LEFT_SHORT:
    menuItemIndex--;
    if (menuItemIndex < 0) menuItemIndex = itemsListLength-1;
    break;

  case BUTTON_RIGHT_SHORT:
    menuItemIndex++;
    if (menuItemIndex == itemsListLength) menuItemIndex = 0;
    break;

  case BUTTON_LEFT_LONG:
    pageManager.setPage(&mainPage);
    Serial.println(F("Switching to main page"));
    break;

  case BUTTON_RIGHT_LONG:
    switch (menuItemIndex)
    {
    case 0:
      shownData = GPS_DATA;
      break;

    case 1:
      shownData = RTC_DATA;
      break;

    case 2:{
      shownData = SD_DATA;
      File dir = SD.open("/");
      listFiles(dir, 0);
      dir.close();
      break;}

    case 3:
      shownData = SYSTEM_DATA;
      break;
    
    default:
      shownData = NO_DATA;
      break;
    }

    break;
  
  default:
    break;
  }
}
