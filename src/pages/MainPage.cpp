#include "MainPage.h"
#include "PageManager.h"
#include "Pages.h"
#include "DrawUtils.h"

extern PageManager pageManager;

void MainPage::draw(U8G2& u8g2) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenfatguys_tf);
  u8g2.setCursor(5,20);
  u8g2.print(F("Main menu"));
  String menuItems[] = {"ACTIVITY", "HISTORY", "OPTIONS", "DEBUG"};
  draw_selection_list(u8g2, menuItems, 4, menuItemIndex);
  draw_info(u8g2);
  u8g2.sendBuffer();
}

void MainPage::handleInput(Input& input){
  
  switch (input.input.action)
  {
  case BUTTON_LEFT_SHORT:
    menuItemIndex--;
    if (menuItemIndex < 0) menuItemIndex = 3;
    break;

  case BUTTON_RIGHT_SHORT:
    menuItemIndex++;
    if (menuItemIndex > 3) menuItemIndex = 0;
    break;

  case BUTTON_RIGHT_LONG:
    switch (menuItemIndex)
    {
    case 0:
      pageManager.setPage(&activityPage);
      Serial.println(F("Switching to the activity page"));
      break;
    
    case 1:
      /* code */
      break;
    
    case 2:
      pageManager.setPage(&settingsPage);
      Serial.println(F("Switching to the settings page"));
      break;
    
    case 3:
      pageManager.setPage(&debugPage);
      Serial.println(F("Switching to the debug page"));
      break;
    
    default:
      break;
    }
    break;
  
  default:
    break;
  }

}
