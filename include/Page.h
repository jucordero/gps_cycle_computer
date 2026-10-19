#pragma once

#include <U8g2lib.h>
#include "Input.h"

class Page {
public:
  virtual void onEnter() {}        // optional
  virtual void onExit() {}         // optional
  virtual void draw(U8G2& u8g2) = 0;
  virtual void handleInput(Input& input) = 0;
  virtual bool blocksSleep() const {
        return false;   // default behaviour
    }

  virtual ~Page() {}
};