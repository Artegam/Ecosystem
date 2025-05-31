#include "ScreenManager.h"

using namespace std;
using namespace ScreenManager;

ScreenViewModel::ScreenViewModel () {
}

ScreenViewModel::ScreenViewModel (WorldModel worldData) {
  this->worldData = worldData;

  function<void(Node *)> fct = [] (Node * n) {
  };

  loadMenu();
  this->currentNode = root;

  //La taille de l ecran fait 254 * 30 (ou 51 ?) characteres avec la console
  // Creation et lancement de la musique
  // stockage de la position x et y en pourcentage
  snd = new Sound();

  Node * opts = this->root->getNode("Options");
  Node * lang = opts->getNode("Languages");
  Node * video = opts->getNode("Video");
  Node * load = root->getNode("Load");
  Node * save = root->getNode("Save");

  screens[MAIN] = new Screen(MAIN, 0, 0);
  screens[MAIN]->resize(50,50);
  screens[MAIN]->add(new Text(MAIN, 45, 20, " Ecosystem v0.2 "));
  screens[MAIN]->add(new Menu(MAIN, 47, 30, getMenuText()));
  //screens[MAIN]->add(new Calendar(MAIN, 10, 10));
  screens[MAIN]->add(new Agenda(MAIN, 10, 10));

  screens[OPTIONS] = new Screen(OPTIONS, 10, 10);
  screens[OPTIONS]->add(new Text(OPTIONS, 1, 0, " Options "));
  screens[OPTIONS]->add(new Menu(OPTIONS, 1, 1, opts->getChildren()));

  screens[LANGUAGES] = new Screen(LANGUAGES, 10, 10);
  screens[LANGUAGES]->add(new Text(LANGUAGES, 1, 0, " Languages "));
  screens[LANGUAGES]->add(new Menu(LANGUAGES, 1, 1, lang->getChildren()));

  screens[VIDEO] = new Screen(VIDEO, 10, 10);
  screens[VIDEO]->add(new Text(VIDEO, 1, 0, " Video "));
  screens[VIDEO]->add(new Menu(VIDEO, 1, 1, video->getChildren()));

  screens[SAVE] = new Screen(SAVE, 10, 10);
  screens[SAVE]->add(new Text(SAVE, 1, 0, " Save "));
  screens[SAVE]->add(new Menu(SAVE, 1, 1, save->getChildren()));

  screens[LOAD] = new Screen(LOAD, 10, 10);
  screens[LOAD]->add(new Text(LOAD, 1, 0, " Load "));
  screens[LOAD]->add(new Menu(LOAD, 1, 1, load->getChildren()));
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

list<string> ScreenViewModel::getMenuText () {
  list<Node *>::iterator it, it2;
  list<Node *> opts = currentNode->getChildren();
  list<string> menu;

  for(it = opts.begin(); it != opts.end(); it++) {
    if (GroupItem* grp = dynamic_cast<GroupItem*>(*it); grp != nullptr) {
      list<Node *> menugroup = grp->getChildren();
      for(it2 = menugroup.begin(); it2 != menugroup.end(); it2++) {
        menu.push_back((*it2)->getName());
      }
    } else {
      menu.insert(menu.end(), (*it)->getName());
    }
  }

  return menu;
}

list<string> ScreenViewModel::getMenuText (Node * node) {
  list<Node *>::iterator it, it2;
  list<Node *> opts = node->getChildren();
  list<string> menu;

  for(it = opts.begin(); it != opts.end(); it++) {
    if (GroupItem* grp = dynamic_cast<GroupItem*>(*it); grp != nullptr) {
      list<Node *> menugroup = grp->getChildren();
      for(it2 = menugroup.begin(); it2 != menugroup.end(); it2++) {
        menu.push_back((*it2)->getName());
      }
    } else {
      menu.insert(menu.end(), (*it)->getName());
    }
  }

  return menu;
}

string ScreenViewModel::getRawData () {
  //TODO: A reflechir...
  return "";
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

  Dictionary dictionary;
  list<string> lst_languages = dictionary.getLanguages();
  list<string>::iterator it;
  for(it = lst_languages.begin(); it != lst_languages.end(); it++)
    languagesgroup->addItem((*it));
  languages->add("");
  languages->add("Back");
  languagesgroup->setDefault(0); // "English"
  Node * video = options->getNode("Video");
  video->addGroup("videogroup");
  GroupItem * videogroup = (GroupItem *)video->getNode("videogroup");
  videogroup->addItem("NCurses");
  videogroup->addItem("OpenGL");
  video->add("");
  video->add("Back");
  videogroup->setDefault(0); // "NCurses"
  this->root->add("Save");
  Node * save = root->getNode("Save");
  save->add("no data yet");
  save->add("");
  save->add("Back");
  this->root->add("Load");
  Node * load = root->getNode("Load");
  load->add("no data yet");
  load->add("");
  load->add("Back");

  this->root->add("Quit");
}

void ScreenViewModel::setMode (int m) {
  mode = m;
}

int ScreenViewModel::getMode () {
  return mode;
}

//TODO: a nettoyer !!
// CA ca sert a faire un menu a partir d'une liste mais pas de faire un ecran complet
// par exemple il faut data->getTitle() pour afficher le nom de l'application
// Il va falloir faire une fonction pour chaque ecran dans le modele
// et probablement remettre cette fonction dans le modele
list<pair<pair<int, int>, string>> ScreenViewModel::getDataScreen (const int screenView) {
  list<Node *>::iterator it, it2;
  list<Node *> opts = getMenu();
  list<pair<pair<int, int>, string>> screen;
  pair<pair<int, int>, string> foo;
  //int y = 0;

  pair<int, int> position;
  pair<pair<int, int>, string> elt;
/*
  switch(screenView) {
    case ROOT:
      //computeMaxWidth(
      position = make_pair(0, 2);
      elt = make_pair(position, " " + getTitle() + " ");
      screen.push_back(elt);
      break;

    case MAIN:
      list<pair<pair<int, int>, string>> menu = getDataMenu();
      screen.insert(screen.end(), menu.begin(), menu.end());
      break;
  }
*/
  return screen;
}

list<pair<pair<int, int>, string>> ScreenViewModel::getDataMenu () {
  list<Node *>::iterator it, it2;
  list<Node *> opts = getMenu();
  list<pair<pair<int, int>, string>> menu;
  pair<pair<int, int>, string> foo;
  int y = 0;

  for(it = opts.begin(); it != opts.end(); it++) {
    if (GroupItem* grp = dynamic_cast<GroupItem*>(*it); grp != nullptr) {
      list<Node *> menugroup = grp->getChildren();
      for(it2 = menugroup.begin(); it2 != menugroup.end(); it2++) {
        foo.first.first = y;  // y
        foo.first.second = 1; // x
        foo.second = (*it2)->getName();
        menu.push_back(foo);
      }
    } else {
      foo.first.first = y + 1; //y
      foo.first.second = 1;    // x
      foo.second = (*it)->getName();
      menu.push_back(foo);
    }
    y++;
  }

  return menu;
}

void ScreenViewModel::createWindow (int position, int height, int width) {
/*
  const int edges = 2;
  const int yOffset = (this->windowHeight / 2) - 5;
  const int xOffset = ((this->windowWidth - width) / 2);

  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("create window() size: " + to_string(height) + " width: " + to_string(width));
 
  if(windows.size() < (unsigned long)(position + 1)) 
    windows.resize(position+1);
  windows[position] = subwin(stdscr, height + edges, width, yOffset, xOffset);
  initScreen();
  refresh();
  if (position == 0)
    s = new Screen(position);
*/
}


Screen ScreenViewModel::getScreen(const int screen) {
  return (*screens[screen]);
}
