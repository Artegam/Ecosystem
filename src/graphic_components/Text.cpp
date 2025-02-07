#include "GraphicComponents.h"

using namespace GraphicComponents;


Text::Text (const int window, const int x, const int y, const string label): Leaf(window, x, y) {
  _label = label;
}

const string Text::label() {
  return _label;
}
