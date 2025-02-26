#include "GraphicComponents.h"

using namespace GraphicComponents;

GroupItem::GroupItem (string name) : Node (name) {
}

GroupItem::GroupItem (Node * parent, string name) : Node (parent, name) {
}

void GroupItem::setDefault (const int index) {
  defaultItem = index;
  selectedItem = index;
  this->display();
}

void GroupItem::selectItem (const int index) {
  list<Node *>::iterator it;
  for(it = children.begin(); it != children.end(); it++) {
    (*it)->clear();
  }

  if((long unsigned int)index < children.size()) {
    it = children.begin();
    advance(it, index);
    selectedItem = index;
    Item * item = dynamic_cast<Item*>(*it);
    item->validate();
  }
}

Item * GroupItem::getSelectedItem () {
  list<Node *>::iterator it = children.begin();
  advance(it, selectedItem);

  return dynamic_cast<Item*>(*it);
}

void GroupItem::display () {
  Item * item = getSelectedItem();
  item->select();
}

const int GroupItem::size() {
  return children.size();
}
