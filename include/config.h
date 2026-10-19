#pragma once
#include "Arduino.h"

#define GPSSerial Serial1

#define TILE_W 120
#define TILE_H 120
#define TILE_BYTES (TILE_W * TILE_H / 8)

#define BACKLIGHT_PIN 13
#define LEFT_BUTTON_PIN 6
#define RIGHT_BUTTON_PIN 14

#define CS_SCREEN 10
#define DC_SCREEN 12
#define RESET_SCREEN 11

#define DEFAULT_BUTTON_HOLD_TIME 750

#define DEFAULT_SCREEN_TIMEOUT 10000
#define DEFAULT_SLEEP_TIMEOUT 30000
#define DEFAULT_SCREEN_BRIGHTNESS 150
#define SLEEP_PERIOD 60000

#define MAX_BAT_LEVEL 660
#define MIN_BAT_LEVEL 550

constexpr char SCREEN_BRIGHTNESS_FILE[] = "conf/scr_bt.txt";
constexpr char SCREEN_TIMER_FILE[]      = "conf/scr_tmr.txt";
constexpr char SLEEP_TIMER_FILE[]       = "conf/slp_tmr.txt";
constexpr char HOLD_BUTTON_FILE[]       = "conf/hld_but.txt";
constexpr char TIMEZONE_FILE[]          = "conf/tzone.txt";

#define INVALID_OPTION_VAL 665