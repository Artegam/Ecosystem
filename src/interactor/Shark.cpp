#include "Interactor.h"
#include <stdio.h>

using namespace Interactor;
using namespace Controller;

Shark::Shark () : Wildlife () {
  this->init();
}

Shark::Shark (World * world) : Wildlife (world) {
  this->init();
  data->setLifetime(data->random(5, 10));
}

Shark::Shark (WildlifeModel * parentData) : Wildlife (parentData) {
  this->init();
  data->setLifetime(data->random(5, 10));
}

void Shark::init () {
  data->setName((char *)"Sharko");
  data->setLifetime(5);
  data->setDisplayChar('S');
  data->setViewField(1);
  this->action = new Default ();
  this->daysRemainingsBeforeStarvation = DAYS_BEFORE_STARVING;
}

void Shark::execute () {
  printf("je m'execute en Shark...\n");
}

void Shark::update () {
  // code specifique ici
	this->action->compute(data);

  if(this->daysRemainingsBeforeStarvation == 0) {
    this->die = true;
    data->getWorld()->getClock()->unsubscribe(this);
  }

  this->daysRemainingsBeforeStarvation--;
}

