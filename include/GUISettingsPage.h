#pragma once
#include "Page.h"

class GUISettingsPage : public Page {
  public:
    void draw(U8G2& u8g2) override;
    void handleInput(Input& input) override;

  private:
    int menuItemIndex = 0;
    const int itemsListLength = 5;
};