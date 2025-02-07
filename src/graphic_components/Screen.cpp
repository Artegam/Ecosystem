#include "GraphicComponents.h"

using namespace GraphicComponents;


Screen::Screen () : Composite () {
}

Screen::Screen (const int window, const int x, const int y) : Composite (window, x, y) {
}

void Screen::select(const int index) {
  Composite::select(index);


  map<int, GraphicComponent *> lst;
  lst = components();

  for(map<int, GraphicComponent *>::iterator it = lst.begin(); it != lst.cend(); it++) {
    if (Menu * menu = dynamic_cast<Menu*>(it->second); menu != nullptr) {
      menu->select(index);
    }
  }
}


