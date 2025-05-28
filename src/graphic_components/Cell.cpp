#include "GraphicComponents.h"

using namespace GraphicComponents;


Cell::Cell (const int window, const int x, const int y, const string value): Leaf(window, x, y) {
  _value = value;
  setHeight(1);
  setWidth(value.length());
}

const string Cell::value () {
  return _value;
}

void Cell::setValue (const string value) {
  _value = value;
}

void Cell::setWidth (const unsigned int w) {
  _width = w;
}

void Cell::setHeight (const unsigned int h) {
  _height = h;
}

void Cell::select () {
  _select=!_select;
}

bool Cell::isSelected () {
  return _select;
}

