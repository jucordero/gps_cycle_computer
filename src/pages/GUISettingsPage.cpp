#include "GUISettingsPage.h"
#include "PageManager.h"
#include "Pages.h"
#include "DrawUtils.h"

extern PageManager pageManager;

void GUISettingsPage::draw(U8G2& u8g2) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenfatguys_tf);
  u8g2.setCursor(5,20);
  u8g2.print(F("GUI Settings"));
  draw_info(u8g2);
  u8g2.sendBuffer();
}

void GUISettingsPage::handleInput(Input& input){

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
    pageManager.setPage(&settingsPage);
    Serial.println(F("Switching to settings page"));
    break;

  case BUTTON_RIGHT_LONG:
    break;
  
  default:
    break;
  }
}
