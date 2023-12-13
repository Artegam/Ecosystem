#include "ScreenManager.h"

using namespace ScreenManager;


Node::Node (string name) {
  this->name = name;
}

void Node::add (Node * node) {
  this->children.push_back(node);
}

list<Node *> Node::getChildren () {
  return this->children;
}

string Node::getName () {
  return this->name;
}

