#include "Interactor.h"
#include <stdio.h>

using namespace Interactor;
using namespace Controller;

Fish::Fish () : Wildlife () {
  init();
}

Fish::Fish (World * world) : Wildlife (world) {
  init();
}

void Fish::init() {
  data->setName((char *)"Nemo");
  data->setLifetime(data->random(10, 15));
  data->setDisplayChar('F');
  data->setViewField(2);
  this->action = new Explorer ();
}

void Fish::execute () {
  printf("je m'execute en Fish...\n");
}

void Fish::update () {
  Wildlife::update();
  // Ecrire ici tous les cas particuliers pour un poisson
}

