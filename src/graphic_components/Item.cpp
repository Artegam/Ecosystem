#include "GraphicComponents.h"

using namespace GraphicComponents;

Item::Item (string name) : Node (name) {
  fct = [] (Item * i) {
    i->select();
  };
}

Item::Item (Node * parent, string name) : Node (parent, name) {
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

bool Item::validate () {
  fct(this);
  return true;
}

void Item::clear () {
  this->selected = false;
}
