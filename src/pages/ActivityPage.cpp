#include "ActivityPage.h"
#include "PageManager.h"
#include "Pages.h"
#include "DrawUtils.h"

extern PageManager pageManager;

void ActivityPage::draw(U8G2& u8g2) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenfatguys_tf);
  u8g2.setCursor(5,20);
  u8g2.print(F("Activity"));
  draw_info(u8g2);
  draw_map(u8g2);
  u8g2.sendBuffer();
}

void ActivityPage::handleInput(Input& input){

  switch (input.input.action)
  {
  case BUTTON_LEFT_SHORT:
    break;

  case BUTTON_RIGHT_SHORT:
    break;

  case BUTTON_LEFT_LONG:
    pageManager.setPage(&mainPage);
    Serial.println(F("Switching to main page"));
    break;

  case BUTTON_RIGHT_LONG:
    break;
  
  default:
    break;
  }
}
