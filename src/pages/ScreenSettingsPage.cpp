#include "ScreenSettingsPage.h"
#include "PageManager.h"
#include "Pages.h"
#include "DrawUtils.h"
#include "config.h"
#include "SDUtils.h"
#include "TimerController.h"

extern PageManager pageManager;
extern TimerController timer;

ScreenSettingsPage::ScreenSettingsPage():
  backlightTimerSlider("Backlight timer", 10000, 60000, 10000, timer.screenTimeout),
  screenBrightnessSlider("Screen Brightness", 0, 250, 50, timer.screenBrightness),
  sleepTimerSlider("Sleep timer", 30000, 120000, 10000, timer.sleepTimeout)
{
  sliders[0] = &backlightTimerSlider;
  sliders[1] = &screenBrightnessSlider;
  sliders[2] = &sleepTimerSlider;
}

Slider* ScreenSettingsPage::activeSlider(){
  if (menuItemIndex < itemsListLength) return sliders[menuItemIndex];
  return nullptr;
}

void ScreenSettingsPage::draw(U8G2& u8g2) {

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenfatguys_tf);
  u8g2.setCursor(5,20);
  u8g2.print(F("Screen Settings"));

  backlightTimerSlider.draw(u8g2, 40);
  screenBrightnessSlider.draw(u8g2, 90);
  sleepTimerSlider.draw(u8g2, 140);

  draw_info(u8g2);
  u8g2.sendBuffer();
}

void ScreenSettingsPage::handleInput(Input& input)
{
    Slider* current = activeSlider();

    switch (input.input.action)
    {
    case BUTTON_LEFT_SHORT:
        if (current && current->selected) {
            current->decrease();
            Serial.println(current->getValue());
        } else {
            menuItemIndex--;
            if (menuItemIndex < 0) menuItemIndex = itemsListLength - 1;
        }
        break;

    case BUTTON_RIGHT_SHORT:
        if (current && current->selected) {
            current->increase();
            Serial.println(current->getValue());
        } else {
            menuItemIndex++;
            if (menuItemIndex >= itemsListLength) menuItemIndex = 0;
        }
        break;

    case BUTTON_RIGHT_LONG:
        if (current) {
            current->selected = true;
        }
        break;

    case BUTTON_LEFT_LONG:
        if (current && current->selected) {
            current->selected = false;

            // Commit value (example)
            if (menuItemIndex == 0) {
                timer.screenTimeout = current->getValue();
                writeVal(SCREEN_TIMER_FILE, current->getValue());
            }
            else if (menuItemIndex == 1) {
                timer.screenBrightness = current->getValue();
                writeVal(SCREEN_BRIGHTNESS_FILE, current->getValue());
                timer.refreshBacklight();
            }
        }
        else {
            pageManager.setPage(&settingsPage);
        }
        break;

    default:
        break;
    }
}
