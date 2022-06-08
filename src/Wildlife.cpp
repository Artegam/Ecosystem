#include "Interactor.h"
#include <random>

using namespace Interactor;
using namespace Controller;
using namespace std;

Wildlife::Wildlife () {
}

Wildlife::Wildlife (World * world) {
  this->world = world;
  this->XPosition = this->random(1, world->getWidth());
  this->YPosition = this->random(1, world->getHeight());
  Clock * c = world->getClock();
  c->subscribe(this);
}

Wildlife::~Wildlife () {
}

char* Wildlife::getName() {
  return this->name;
}

void Wildlife::execute () {
  printf("Je m'execute en Wildlife...\n");
}

void Wildlife::update () {
  printf("Je me mets a jour en Wildlife...\n");
}

int Wildlife::random (const int min, const int max) {
  random_device                  rand_dev;
  mt19937                        generator(rand_dev());
  uniform_int_distribution<int>  distr(min, max);

  return distr(generator);
}

int Wildlife::getX () {
  return XPosition;
}

int Wildlife::getY () {
  return YPosition;
}

char Wildlife::getDisplayChar () {
  return displayChar;
}

const unsigned int Wildlife::getLifetimeRemaining () {
  return this->lifetime;
}

const unsigned int Wildlife::getAge () {
  return this->age;
}

void Wildlife::makeOld() {
  this->lifetime--;
  this->age++;
  if(this->lifetime <= 0) {
    this->die = true;
    this->world->getClock()->unsubscribe(this);
  }
}

