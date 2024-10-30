#include "ScreenManager.h"

using namespace ScreenManager;

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

Item * GroupItem::getSelectedItem () {
  return (Item *)this->getNode(selectedItem);
}

void GroupItem::display () {
  grp.clear();
  Item * item = (Item *)this->getNode(selectedItem);
  item->select();
}
