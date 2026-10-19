#include "Arduino.h"
#include "Slider.h"

Slider::Slider(const char* label, int min, int max, int step, int value):
    label(label),
    min(min),
    max(max),
    step(step),
    value(value)
{}

int Slider::getValue(){
    return value;
}

void Slider::setValue(int val){
    value = val;
}

void Slider::increase(){
    value = value + step;
    if (value > max) value = max;
}

void Slider::decrease(){
    value = value - step;
    if (value < min) value = min;
}

void Slider::draw(U8G2& u8g2, int y){
    u8g2.drawFrame(0, y+8, w, h);

    u8g2.setCursor(5, y);
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.print(label);

    int nSteps = (max - min) / step;
    int dw = w / nSteps;

    for (int i = 0; i < nSteps; i++)
    {
        u8g2.drawVLine(dw + i*dw, y+8, h);
    }
    u8g2.setDrawColor(0);
    u8g2.drawBox(1, y+5+8, w-2, h-10);
    u8g2.setDrawColor(1);

    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.setCursor(60-u8g2.getStrWidth((const char*)String(value).c_str())/2, y+5+8);
    u8g2.print(value);

    if (selected){
        u8g2.setDrawColor(2);
        u8g2.drawBox(1, y+1+8, dw*(value-min)/step, h-2);
        u8g2.setDrawColor(1);
    }
    else {
        u8g2.drawVLine(dw*(value-min)/step, y+8, h);
    }

    if (highlighted){
        u8g2.setDrawColor(2);
        u8g2.drawBox(0, y, w, 8);
        u8g2.setDrawColor(1);
    }


}