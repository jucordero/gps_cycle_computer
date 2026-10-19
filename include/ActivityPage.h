#pragma once
#include "Page.h"

class ActivityPage : public Page {
  public:
    void draw(U8G2& u8g2) override;
    void handleInput(Input& input) override;
    bool blocksSleep() const override {
        return true;
    }

};