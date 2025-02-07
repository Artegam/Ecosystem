#include "GraphicComponents.h"

using namespace GraphicComponents;


Node::Node (string name) {
  this->name = name;
  this->parent = this;
}

Node::Node (Node * parent, string name) {
  this->name = name;
  this->parent = parent;
}

void Node::add (Node * node) {
  this->children.push_back(node);
}

void Node::add (string name) {
  Node * node = new Node(this, name);
  this->children.push_back(node);
}

void Node::addItem (string name) {
  Item * item = new Item(this, name);
  this->children.push_back(item);
}

void Node::addGroup (string name) {
  GroupItem * group = new GroupItem(this, name);
  this->children.push_back(group);
}

void Node::erase (unsigned int position) {
  list<Node *>::iterator it = this->children.begin();
  advance(it, position);
  this->children.erase(it);
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

Node * Node::getNode(string name) {
  list<Node *>::iterator it;
  for(it = children.begin(); it != children.end(); it++) {
    if((*it)->getName() == name)
      return (*it);
  }

  return NULL;
}

bool Node::validate () {
  try {
    //fct();
    return false;
  } catch (std::bad_function_call& e) {
    // Needs to put the logger here !!!
    //std::cout << "ERROR: Bad function call\n";
  }
}

void Node::clear () {
}


