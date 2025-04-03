#include "GraphicComponents.h"
#include <iostream>
using namespace std;

using namespace GraphicComponents;

GraphicComponent::GraphicComponent () {
}

GraphicComponent::GraphicComponent(const int window, const int x, const int y) {
  //this->_id = id; // ici il faut donner le numéro du composant. SOn index dans une liste ?
  this->_window = window;
  this->_x = x;
  this->_y = y;
  _selectSize = 0;

}

void GraphicComponent::resize(const int height, const int width) {
  _height = height;
  _width = width;
}

void GraphicComponent::setSelectSize (const int size) {
  _selectSize = size;
}

const int GraphicComponent::getSelectSize() {
  return _selectSize;
}

void GraphicComponent::select (const int index) {
  cout << "taille max confirmée : " << _selectSize << endl;
  cout << "la selection : " << index << endl;
  if (index < 0)
    _selectedIndex = 0;
  else if(index > _selectSize)
    _selectedIndex = _selectSize;
  else
    _selectedIndex = index;
}

const int GraphicComponent::selected () {
  return _selectedIndex;
}

const bool GraphicComponent::isValidated () {
  return _validated;
}

const int GraphicComponent::id() {
  return _id;
}

const int GraphicComponent::window() {
  return _window;
}

const int GraphicComponent::x() {
  return _x;
}

const int GraphicComponent::y() {
  return _y;
}

list<string> GraphicComponent::nodesToString (list<Node *> items) {
  list<Node *>::iterator it;
  list<string> lst;

  for(it = items.begin(); it != items.end(); it++) {
    if (GroupItem* grp = dynamic_cast<GroupItem*>(*it); grp != nullptr) {
      list<Node *> menugroup = grp->getChildren();
      list<string> lst_children = nodesToString(menugroup);
      lst.insert(lst.end(), lst_children.begin(), lst_children.end());
    } else {
      //TODO: Group Item est il un GraphicComponent ?
      string prefix = "";
      if (Item* item = dynamic_cast<Item*>(*it)) {
        if(item->isSelected()) {
          //prefix = dict->translate("selectedItem");
          prefix = "selectedItem ";
        } else {
          //prefix = data->translate("unselectedItem");
          prefix = "[ ] ";
        }
      }
      lst.push_back(prefix + (*it)->getName());
    }
  }
  return lst;
}
