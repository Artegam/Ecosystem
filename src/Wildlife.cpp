#include "Interactor.h"

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
  data->setX(data->random(1, world->getWidth()));
  data->setY(data->random(1, world->getHeight()));
  Clock * c = world->getClock();
  c->subscribe(this);
}

Wildlife::~Wildlife () {
}

void Wildlife::execute () {
  printf("Je m'execute en Wildlife...\n");
}

void Wildlife::update () {
  //printf("Je me mets a jour en Wildlife...\n");
  action->compute(data);
  //Si l animal est encore en vie aprÃs x tours
  // il y a un enfant
  if(data->getAge() >= 9) {
    new Fish(data->getWorld());
  }

  this->makeOld();
}

void Wildlife::makeOld() {
  data->makeOld();
  if(data->getLifetimeRemaining() <= 0) {
    this->die = true;
    data->getWorld()->getClock()->unsubscribe(this);
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
