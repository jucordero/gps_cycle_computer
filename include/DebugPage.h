#pragma once
#include "Page.h"

enum ShownData {
  NO_DATA,
  GPS_DATA,
  SD_DATA,
  RTC_DATA,
  SYSTEM_DATA
};

class DebugPage : public Page {
  public:
    void draw(U8G2& u8g2) override;
    void handleInput(Input& input) override;

  private:
    int menuItemIndex = 0;
    const int itemsListLength = 4;
    ShownData shownData = NO_DATA; 
};