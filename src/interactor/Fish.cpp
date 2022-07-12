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

Fish::Fish (WildlifeModel * parentData) : Wildlife (parentData) {
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

  //Si l animal est encore en vie aprÃs x tours
  // il y a un enfant

  if(data->getAge() >= 9) {
    this->addWildlife("Fish", data->random(1, 3));
  }

  //printf("je m'update en Fish...\n");
	//this->action->compute(data);
}
