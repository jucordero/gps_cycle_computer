#include "PageManager.h"

void PageManager::setPage(Page* newPage) {
  current = newPage;
}

void PageManager::draw(U8G2& u8g2) {
  current->draw(u8g2);
}

void PageManager::handleInput(Input& input) {
  current->handleInput(input);
}
