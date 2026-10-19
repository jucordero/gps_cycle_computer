#include "SettingsPage.h"
#include "PageManager.h"
#include "Pages.h"
#include "DrawUtils.h"
#include "TimerController.h"

extern PageManager pageManager;
extern TimerController timer;

void SettingsPage::draw(U8G2& u8g2) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenfatguys_tf);
  u8g2.setCursor(5,20);
  u8g2.print(F("Settings"));
  String menuItems[] = {"GPS", "RTC", "SD", "GUI", "SCREEN"};
  draw_selection_list(u8g2, menuItems, itemsListLength, menuItemIndex);
  draw_info(u8g2);
  u8g2.sendBuffer();
}

void SettingsPage::handleInput(Input& input){

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
      pageManager.setPage(&gpsSenttingsPage);
      Serial.println(F("Switching to RTC settings page"));
      break;
    
    case 1:
      pageManager.setPage(&rtcSenttingsPage);
      Serial.println(F("Switching to GPS settings page"));
      break;
    
    case 2:
      pageManager.setPage(&sdSenttingsPage);
      Serial.println(F("Switching to SD settings page"));
      break;
    
    case 3:
      pageManager.setPage(&guiSenttingsPage);
      Serial.println(F("Switching to GUI settings page"));
      break;
    
    case 4:
      screenSenttingsPage.screenBrightnessSlider.setValue(timer.screenBrightness);
      screenSenttingsPage.backlightTimerSlider.setValue(timer.screenTimeout);
      pageManager.setPage(&screenSenttingsPage);
      Serial.println(F("Switching to Screen settings page"));
      break;
    
    default:
      break;
    }
    break;
  
  default:
    break;
  }
}
