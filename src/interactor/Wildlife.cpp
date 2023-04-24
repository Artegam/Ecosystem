#include "Interactor.h"
#include <cstring>
#include <math.h>

using namespace Interactor;
using namespace Controller;
using namespace std;

Wildlife::Wildlife () {
  data = new WildlifeModel();
}

/*
Wildlife::Wildlife (const Wildlife * w) {
  name        = w->name;
  lifetime    = w->lifetime;
  age         = w->age;
  displayChar = w->displayChar;
  viewField   = w->viewField;
  XPosition   = w->XPosition;
  YPosition   = w->YPosition;
  action      = w->action;
  world       = w->world;
}
*/

Wildlife::Wildlife (World * world) {
  data = new WildlifeModel();
  data->setWorld(world);
  WorldModel worldData = world->getData();
  data->setX(data->random(1, worldData.getWidth()));
  data->setY(data->random(1, worldData.getHeight()));
  Clock * c = worldData.getClock();
  c->subscribe(this);
}

Wildlife::Wildlife (WildlifeModel * parentData) {
  World * world = parentData->getWorld();
  //data = new WildlifeModel(*parentData);
  data = new WildlifeModel();
  data->setWorld(world);
  data->setX(parentData->getX());
  data->setY(parentData->getY());
  WorldModel worldData = world->getData();
  Clock * c = worldData.getClock();
  c->subscribe(this);
}

Wildlife::~Wildlife () {
}

void Wildlife::execute () {
  printf("Je m'execute en Wildlife...\n");
}

void Wildlife::update () {
  data->openYourEyes();
  action->compute(data);

  Logger * log = new Logger("bin/mon_fichier.log");
  log->log(data);

  this->happyBirthday();
}

template <class T> bool Wildlife::cmp(pair<T, T>& x1, pair<T, T>& x2) {
  // return type is bool
  return x1.second <= x2.second;
}

void Wildlife::happyBirthday() {
  data->happyBirthday();
  data->getHungry();

  if(isDead() || isStarving()) {
    this->die = true;
    WorldModel worldData = data->getWorld()->getData();
    worldData.getClock()->unsubscribe(this);
  }
}

const unsigned int Wildlife::getAge () {
  return data->getAge();
}

const unsigned int Wildlife::getLifetimeRemaining () {
  return data->getLifetimeRemaining();
}

int Wildlife::getX () {
  return data->getX();
}

int Wildlife::getY () {
  return data->getY();
}

char Wildlife::getDisplayChar () {
  return data->getDisplayChar();
}

void Wildlife::addWildlife(string wildlifeName, int number) {
	for(int i = 1; i < number; i++) {
    if(!strcmp(wildlifeName.c_str(), "Fish")) {
      new Fish(data);
      data->setMaturityAge(2000);
    } else if (!strcmp(wildlifeName.c_str(), "Shark")) {
      new Shark(data);
    }
	}
}

bool Wildlife::isStarving () {
  return (data->getTurnsNumberBeforeStarving() == 0);
}

bool Wildlife::isDead () {
  return (data->getLifetimeRemaining() <= 0);
}

GenericModel * Wildlife::getData () {
  return data;
}

