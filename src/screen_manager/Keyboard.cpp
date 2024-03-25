#include "ScreenManager.h"

using namespace ScreenManager;


void Keyboard::keyPressed (int key) {
  switch(key) {
    case KEYB_UP:
      break;
    case KEYB_DOWN:
      break;
    case KEYB_SPACE:
      break;
    case KEYB_ENTER:
      break;
    case KEYB_ESCAPE:
      break;
  }
}

void Keyboard::defaultPosition () {
  keybPosition = keybPosition_default;
}

void Keyboard::setPositionsCount (const unsigned int count) {
  this->positionsCount = count;
}

/*
void Keyboard::setDefaultPosition (const unsigned int position) {
  keybPosition = position;
}
*/

int Keyboard::getPosition () {
  return keybPosition;
}

void Keyboard::onUp () {
  if(keybPosition > 0)
    keybPosition--;
}

void Keyboard::onDown () {
  if(keybPosition < this->positionsCount - 1)
    keybPosition++;
}

void Keyboard::onEnter () {
  valid = true;
}

void Keyboard::resetValid () {
  valid = false;
}

bool Keyboard::isValid () {
  return valid;
}

void Keyboard::listen (unsigned int character) {
}
