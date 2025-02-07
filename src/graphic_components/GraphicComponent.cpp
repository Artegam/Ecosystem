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


