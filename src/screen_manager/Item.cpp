#include "ScreenManager.h"

using namespace ScreenManager;

Item::Item (string name) : Node (name) {
  fct = [] (Item * i) {
    i->select();
  };
}

void Item::select () {
  this->selected = !this->selected;
}

bool Item::isSelected () {
  return this->selected;
}

string Item::getName () {
  return Node::getName();
}

void Item::validate () {
  fct(this);
}
