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

Node * Node::getParent () {
  return this->parent;
}

void Node::validate () {
  try {
    //fct();
  } catch (std::bad_function_call& e) {
    // TODO: Needs to put the logger here !!!
    //std::cout << "ERROR: Bad function call\n";
  }
}

void Node::clear () {
}


