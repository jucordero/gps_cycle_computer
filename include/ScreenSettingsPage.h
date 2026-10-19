#pragma once
#include "Page.h"
#include "Slider.h"

class ScreenSettingsPage : public Page {
  public:
  ScreenSettingsPage();
  void draw(U8G2& u8g2) override;
  void handleInput(Input& input) override;
  Slider backlightTimerSlider;
  Slider screenBrightnessSlider;
  Slider sleepTimerSlider;
  
  private:
    static const int itemsListLength = 3;
    int menuItemIndex = 0;
    Slider* sliders[itemsListLength];
    Slider* activeSlider();
};