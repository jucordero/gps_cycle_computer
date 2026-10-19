#include "Arduino.h"
#include "DrawUtils.h"
#include <TinyGPS++.h>
#include <RTCZero.h>
#include "SDUtils.h"
#include "config.h"

extern RTCZero rtc;
extern TinyGPSPlus gps;
extern uint8_t
  tileBuffer[],
  side_tileBuffer[],
  updn_tileBuffer[],
  diag_tileBuffer[];

extern TileCache
  mainCache,
  sideCache,
  diagCache,
  updnCache;

void draw_selection_list(U8G2& u8g2, const String* items, int itemCount, int selectedIndex){
  const int frameHeight = 20;
  const int frameWidth = 120;
  const int startY = 40;
  const int frameSeparation = 1;

  u8g2.setFont(u8g2_font_tenfatguys_tf);

  for (int i = 0; i < itemCount; i++) {
    int y = startY + i * (frameHeight + frameSeparation);
    
    if (i == selectedIndex) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, y, frameWidth, frameHeight);
      u8g2.setDrawColor(0);
    } else {
      u8g2.setDrawColor(1);
    }

    u8g2.drawFrame(0, y, frameWidth, frameHeight);
    
    u8g2.setCursor(5, y + 5);
    u8g2.print(items[i]);

    u8g2.setDrawColor(1);
  }
}

void draw_info(U8G2& u8g2){
  u8g2.setFont(u8g2_font_crox2cb_mn);

  String hh = String(rtc.getHours());
  String mm = String(rtc.getMinutes());
  if (rtc.getMinutes() < 10) mm = '0' + mm;
  String hora = hh + ':' + mm;
  char buf[9];
  hora.toCharArray(buf, 9);
  u8g2.drawStr(1, -1, buf);

  int bat_level = analogRead(A7);
  bat_level = constrain(bat_level, MIN_BAT_LEVEL, MAX_BAT_LEVEL);
  int bat_level_pctg = map(bat_level, MIN_BAT_LEVEL, MAX_BAT_LEVEL, 0, 100);

  u8g2.setFont(u8g2_font_6x10_tn);

  u8g2.setCursor(93, 2);
  u8g2.print(bat_level_pctg);

  if (gps.satellites.value() > 0){
    for (int i = 0; i < (int)gps.satellites.value(); i++){
      u8g2.drawVLine(55 + i*3, 13-i, i);
      u8g2.drawVLine(55 + i*3+1, 13-i, i);
    }
    // If no updates received in the last 10 seconds
    if (gps.speed.age() > 10000){
      u8g2.setCursor(55+(gps.satellites.value()+1)*3, -1);
      u8g2.print(F("?"));
    }
  }
  else {
    u8g2.setCursor(55,10);
    u8g2.print(F("No signal"));
  }
}

void draw_map(U8G2& u8g2){

    int dy = 20;
    //Romford
    // double lat = 51.5775;
    // double lng = 0.1786;

    double lat = gps.location.lat();
    double lng = gps.location.lng();
    
    // Get main tile
    int tilex = lng2tilex(lng);
    int tiley = lat2tiley(lat);
    
    // Get main tile diplay positions
    int px = 60 - (int)( (lng2tilex(lng) - tilex) * TILE_W);
    int py = 60 - (int)( (lat2tiley(lat) - tiley) * TILE_H);
    
    // Get adjacent tile data
    int dtx = px > 0 ? -1 : 1;
    int dty = py > 0 ? -1 : 1;
    
    loadBMP(tilex,     tiley,     tileBuffer, mainCache);
    loadBMP(tilex+dtx, tiley,     side_tileBuffer, sideCache);
    loadBMP(tilex,     tiley+dty, updn_tileBuffer, updnCache);
    loadBMP(tilex+dtx, tiley+dty, diag_tileBuffer, diagCache);
    
    u8g2.setClipWindow(0, dy, 120, 120 + dy);

    // Draw main tile inside a frame
    u8g2.drawXBM(px, dy + py, TILE_W, TILE_H, tileBuffer);
    u8g2.drawXBM(px + dtx*TILE_W, dy + py, TILE_W, TILE_H, side_tileBuffer);
    u8g2.drawXBM(px, dy + py + dty*TILE_H, TILE_W, TILE_H, updn_tileBuffer);
    u8g2.drawXBM(px + dtx*TILE_W, dy + py + dty*TILE_H, TILE_W, TILE_H, diag_tileBuffer);

    // Draw cental position

    u8g2.setDrawColor(0);
    u8g2.drawDisc(60, dy+60, 5);
    u8g2.setDrawColor(1);
    u8g2.drawDisc(60, dy+60, 3);    

    u8g2.setMaxClipWindow();
}

void draw_slider(
  U8G2& u8g2,
  const char* label,
  int y,
  int min,
  int max,
  int value,
  int step,
  bool selected){

    if (selected){
      u8g2.drawBox(0, y, 120, 20);
    }
}

void draw_toggle_switch(U8G2& u8g2, const char* label, int y, bool state, bool selected) {
  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.setCursor(5, y);
  u8g2.print(label);

  int switchX = 90;
  int switchY = y;
  int switchWidth = 20;
  int switchHeight = 10;
  int circleRadius = 4;

  u8g2.drawFrame(switchX, switchY, switchWidth, switchHeight);
  if (state) {
    u8g2.drawDisc(switchX + switchWidth - circleRadius - 1, switchY + switchHeight / 2, circleRadius);
  } else {
    u8g2.drawDisc(switchX + circleRadius + 1, switchY + switchHeight / 2, circleRadius);
  }
}
