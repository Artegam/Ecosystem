#include "ScreenManager.h"

using namespace std;
using namespace ScreenManager;

ScreenViewModel::ScreenViewModel (WorldModel worldData) {
  this->worldData = worldData;

  function<void(Node *)> fct = [] (Node * n) {
  };

  loadMenu();
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

void ScreenViewModel::validate(int position) {
  list<Node *> lst = getMenu();
  list<Node *>::iterator it = lst.begin();
  advance(it, position);

  if(!(*it)->validate()) {
    // Go one step down into the menu hierarchy
    parentNodes.push_front(this->currentNode);
    this->currentNode = (*it);
  }
}

void ScreenViewModel::back() {
  this->currentNode = parentNodes.front();
  parentNodes.pop_front();
}

list<Node *> ScreenViewModel::getMenu () {
  list<Node *>::iterator it;
  list<Node *> opts = currentNode->getChildren();
  list<Node *> menu;

  for(it = opts.begin(); it != opts.end(); it++) {
    if (GroupItem* grp = dynamic_cast<GroupItem*>(*it); grp != nullptr) {
      list<Node *> menugroup = grp->getChildren();
      menu.insert(menu.end(), menugroup.begin(), menugroup.end());
    } else {
      menu.insert(menu.end(), *it);
    }
  }

  return menu;
}

list<Node *> ScreenViewModel::getParents () {
  return parentNodes;
}

string ScreenViewModel::getTitle () {
  return "Ecosystem V0.1";
}

void ScreenViewModel::loadMenu () {
  // Construction du menu de l'application
  this->root = new Node("root");
  this->root->add("New");
  this->root->add("Options");
  Node * options = this->root->getNode("Options");
  options->add("Languages");
  options->add("Video");
  options->add("");
  options->add("Back");
  Node * languages = options->getNode("Languages");
  languages->addGroup("languagesgroup");
  GroupItem * languagesgroup = (GroupItem *)languages->getNode("languagesgroup");

  list<string> lst_languages = dictionary.getLanguages();
  list<string>::iterator it;
  for(it = lst_languages.begin(); it != lst_languages.end(); it++)
    languagesgroup->addItem((*it));
  languages->add("");
  languages->add("Back");
  languagesgroup->setDefault("English");
  Node * video = options->getNode("Video");
  video->addGroup("videogroup");
  GroupItem * videogroup = (GroupItem *)video->getNode("videogroup");
  videogroup->addItem("NCurses");
  videogroup->addItem("OpenGL");
  video->add("");
  video->add("Back");
  videogroup->setDefault("NCurses");
  this->root->add("Save");
  this->root->add("Load");
  this->root->add("Quit");
}

string ScreenViewModel::translate(string key) {
  return dictionary.translate(key);
}

void ScreenViewModel::setLanguage (unsigned int lang) {
  dictionary.setLanguage(lang);
}
