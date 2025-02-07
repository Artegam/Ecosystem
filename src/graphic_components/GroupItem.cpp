#include "GraphicComponents.h"

using namespace GraphicComponents;

GroupItem::GroupItem (string name) : Node (name) {
}

GroupItem::GroupItem (Node * parent, string name) : Node (parent, name) {
}

void GroupItem::setDefault (string name) {
  defaultItem = name;
  selectedItem = name;
  this->display();
}

void GroupItem::selectItem (string name) {
  selectedItem = name;
}

void GroupItem::selectItem (const int index) {
  list<Node *>::iterator it;
  for(it = children.begin(); it != children.end(); it++) {
    (*it)->clear();
  }
  it = children.begin();
  advance(it, index);
  selectedItem = (*it)->getName();
  Item * item = dynamic_cast<Item*>(*it);
  item->validate();
}

Item * GroupItem::getSelectedItem () {
  return (Item *)this->getNode(selectedItem);
}

void GroupItem::display () {
  Item * item = (Item *)this->getNode(selectedItem);
  item->select();
}

const int GroupItem::size() {
  return children.size();
}
