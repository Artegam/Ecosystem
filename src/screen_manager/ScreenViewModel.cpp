#include "ScreenManager.h"

using namespace std;
using namespace ScreenManager;

ScreenViewModel::ScreenViewModel (WorldModel worldData) {
  this->worldData = worldData;

  function<void(Node *)> fct = [] (Node * n) {
  };

  // Construction du menu de l'application
  this->root = new Node("root");
  this->root->add("New");
  this->root->add("Options");
  Node * options = this->root->getNode("Options");
  options->add("languages");
  options->add("video");
  options->add("");
  options->add("back");
  Node * languages = options->getNode("languages");
  languages->addItem("English");
  languages->addItem("French");
  languages->add("");
  languages->add("back");
  Node * video = options->getNode("video");
  video->addItem("NCurses");
  video->addItem("OpenGL");
  video->add("");
  video->add("back");
  this->root->add("Save");
  this->root->add("Load");
  this->root->add("Quit");

  this->currentNode = root;

  // Creation et lancement de la musique
  snd = new Sound();
}

int ScreenViewModel::getWorldHeight () {
  return worldData.getHeight();
}

int ScreenViewModel::getWorldWidth () {
  return worldData.getWidth();
}

WorldModel ScreenViewModel::getWorldData () {
  return worldData;
}

bool ScreenViewModel::isRunning () {
  return worldData.getClock()->isRunning();
}

bool ScreenViewModel::getTic() {
  return worldData.getClock()->getT();
}

unsigned int ScreenViewModel::getWildlifeCount() {
  return worldData.getClock()->subscribersCount();
}

list<Wildlife *> ScreenViewModel::getWildlife() {
  list<Wildlife *> lst;

  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(this->isWildlife(it->second)) {
      lst.push_back((Wildlife *)it->second);
    }
  }

  return lst;
}


bool ScreenViewModel::isWildlife(const ClockSubscriber * ptr) {
  return dynamic_cast<const ClockSubscriber *>(ptr) != nullptr;
}

unsigned int ScreenViewModel::getTurns () {
  return worldData.getClock()->getTurns();
}

const unsigned int ScreenViewModel::getAverageAge () {
  unsigned int averageAge = 0;

  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(this->isWildlife(it->second)) {
      Wildlife * life = (Wildlife *)it->second;
      averageAge += life->getAge();
    }
  }

  if(subscribers.size() > 0) {
    averageAge = averageAge / subscribers.size();
  } else {
    averageAge = 0;
  }

  return averageAge;
}

const unsigned int ScreenViewModel::getLifeExpectancy () {
  unsigned int lifeExpectancy = 0;

  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(this->isWildlife(it->second)) {
      Wildlife * life = (Wildlife *)it->second;
      lifeExpectancy += life->getAge() + life->getLifetimeRemaining();
    }
  }

  if(subscribers.size() > 0) {
    lifeExpectancy = lifeExpectancy / subscribers.size();
  } else {
    lifeExpectancy = 0;
  }

  return lifeExpectancy;
}


string ScreenViewModel::getRawData () {
  //TODO: remplir ici (a reflechir)
  return "ScreenViewModel: to be implemented";
}

list<string> ScreenViewModel::log() {
  messages.push_back("appel a ScreenViewModel::log()");
  return messages;
}

void ScreenViewModel::validateNode(int position) {
  list<Node *> lst = getMenu();
  list<Node *>::iterator it = lst.begin();
  advance(it, position);
  // TODO: Reflechir ici avec des groupItem par exemple
  // Verifie si il s'agit d'un Item
  Item * i = dynamic_cast<Item*> ((*it));
  if( i != NULL) {
    i->select();
  } else {
    parentNodes.push_front(this->currentNode);
    this->currentNode = (*it);
  }
}

void ScreenViewModel::backNode() {
  this->currentNode = parentNodes.front();
  parentNodes.pop_front();
}

list<Node *> ScreenViewModel::getMenu () {
  return currentNode->getChildren();
}

list<Node *> ScreenViewModel::getParents () {
  return parentNodes;
}
