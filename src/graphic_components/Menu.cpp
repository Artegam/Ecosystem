#include "GraphicComponents.h"

using namespace GraphicComponents;


Menu::Menu (const int window, const int x, const int y, list<string> items) : Composite (window, x, y) {
  list<string>::iterator it;

  _items.clear();
  for(it = items.begin(); it != items.end(); it++) {
    Node * node = new Node((*it));
    _items.push_back(node);
  }
  _selectSize = _items.size();
}

Menu::Menu (const int window, const int x, const int y, list<Node *> items) : Composite (window, x, y) {
  _items = items;
  list<Node *>::iterator it;

  for(it = items.begin(); it != items.end(); it++) {
    if (GroupItem* grp = dynamic_cast<GroupItem*>(*it); grp != nullptr) {
      _selectSize += grp->size();
    } else {
      _selectSize++;
    }
  }
}

list<string> Menu::items() {
  return nodesToString(_items);
}

void Menu::select (const int index) {
  for(list<Node *>::iterator it = _items.begin();it != _items.end(); it++) {
    if (GroupItem* grp = dynamic_cast<GroupItem*>(*it); grp != nullptr) {
      grp->selectItem(index);
    }
  }
}
