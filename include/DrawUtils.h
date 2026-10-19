#pragma once
#include "U8g2lib.h"

void draw_selection_list(U8G2& u8g2, const String* items, int itemCount, int selectedIndex);
void draw_info(U8G2& u8g2);
void draw_map(U8G2& u8g2);
void draw_slider(U8G2& u8g2, int y, int min, int max, int value, int step, bool selected);
void draw_button(U8G2& u8g2, int x, int y, const String& text, bool selected);