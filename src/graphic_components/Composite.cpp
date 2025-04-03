#include "GraphicComponents.h"

using namespace GraphicComponents;

Composite::Composite () {
}

Composite::Composite (const int window, const int x, const int y) : GraphicComponent(window, x, y) {
}

void Composite::add (GraphicComponent * gc) {
  _components[_components.size()] = gc;
  if(gc->getSelectSize() > 0)
    _selectSize = gc->getSelectSize();
}

map<int, GraphicComponent *> Composite::components() {
  return _components;
}

void Composite::select(const int index) {
  GraphicComponent::select(index);
}

void Composite::resize(const int height, const int width) {
  GraphicComponent::resize(height, width);
}

