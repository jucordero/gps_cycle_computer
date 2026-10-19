#pragma once
#include "U8g2lib.h"

class Slider{
    public:
        Slider(const char* label, int min, int max, int step, int value);
        void draw(U8G2& u8g2, int y);
        int getValue();
        void setValue(int val);
        void increase();
        void decrease();
        bool selected = false;
        bool highlighted = false;
        
    private:
        const char* label;
        int min, max, step;
        int value;
        int w = 120;
        int h = 30;
};
