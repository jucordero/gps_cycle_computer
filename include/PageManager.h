#pragma once
#include "Page.h"

class PageManager {
public:
  void setPage(Page* newPage);
  void handleInput(Input& input);
  void draw(U8G2& u8g2);
  bool pageBlocksSleep() const {
    return current && current->blocksSleep();
}

private:
  Page* current = nullptr;
};