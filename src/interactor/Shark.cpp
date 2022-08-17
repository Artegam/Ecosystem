#include "Interactor.h"
#include <stdio.h>

using namespace Interactor;
using namespace Controller;

Shark::Shark () : Wildlife () {
  this->init();
}

Shark::Shark (World * world) : Wildlife (world) {
  this->init();
}

Shark::Shark (WildlifeModel * parentData) : Wildlife (parentData) {
  this->init();
}

void Shark::init () {
  data->setName((char *)"Sharko");
  data->setLifetime(5, 10);
  data->setDisplayChar('S');
  data->setViewField(1);
  this->action = new Default ();
}

void Shark::execute () {
  printf("je m'execute en Shark...\n");
}

void Shark::update () {
  Wildlife::update();
  // code specifique ici
}

