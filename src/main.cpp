#include <Arduino.h>
#include <U8g2lib.h>
#include "PageManager.h"
#include "Pages.h"
#include "Input.h"
#include <TinyGPS++.h>
#include <RTCZero.h>
#include "Tracking.h"
#include <SD.h>
#include "SDUtils.h"
#include "TimerController.h"
#include "config.h"

RTCZero rtc;
TinyGPSPlus gps;

U8G2_UC1608_ERC240120_F_4W_HW_SPI u8g2(U8G2_R3, CS_SCREEN, DC_SCREEN, RESET_SCREEN);

PageManager pageManager;
Input input(LEFT_BUTTON_PIN, RIGHT_BUTTON_PIN);

// Tile buffers

uint8_t tileBuffer[TILE_BYTES];
uint8_t side_tileBuffer[TILE_BYTES];
uint8_t updn_tileBuffer[TILE_BYTES];
uint8_t diag_tileBuffer[TILE_BYTES];
TileCache mainCache;
TileCache sideCache;
TileCache diagCache;
TileCache updnCache;

// Backligt controller
TimerController timer(BACKLIGHT_PIN, LEFT_BUTTON_PIN, RIGHT_BUTTON_PIN);

// Deep Sleep timer
unsigned long int deepSleepTimer; 

void setup() {

    // Initialize serial communication 
    Serial.begin(115200);
    GPSSerial.begin(9600);

    delay(500);

    // Set position interval to 2000 miliseconds
    Serial.println(F("Setting position interval"));
    
    GPSSerial.print(F("$PMTK220,2000*1C\r\n"));

    // Set to visible only RMC, GGA messages every update and every 5 updates
    // respectively: Position, speed and used visible satellites
    Serial.println(F("Setting RMC and GGA messages"));
    GPSSerial.print(F("$PMTK314,0,1,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*2C\r\n"));
    
    // Initialize input controller
    input.begin();
    
    // Initializer real time clock
    rtc.begin();
    
    // SD card
    SD.begin(4);
    
    // Initialize display
    u8g2.begin();
    u8g2.setDrawColor(1);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setFontPosTop();
    u8g2.setFont(u8g2_font_6x10_tf);

    // Initialize page
    pageManager.setPage(&mainPage);

    // Initialize backlight
    timer.begin(rtc);
}

void loop() {
    tracking(rtc, gps);
    input.update();
    timer.update(input.input.action, u8g2, rtc);
    pageManager.handleInput(input);
    pageManager.draw(u8g2);
}