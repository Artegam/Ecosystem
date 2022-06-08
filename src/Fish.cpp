#include "Interactor.h"
#include <stdio.h>

using namespace Interactor;
using namespace Controller;

Fish::Fish () {
  this->name = (char *)"Nemo";
  this->lifetime = 10;
  this->displayChar = 'F';
  this->viewField = 2;
  this->action = new Explorer ();
}

Fish::Fish (World * world) : Wildlife (world) {
  //TODO: Appeler le constructeur par defaut ici mais comment ???
  this->name = (char *)"Nemo";
  this->lifetime = this->random(10, 15);
  this->displayChar = 'F';
  this->viewField = 2;
  this->action = new Explorer ();
}

void Fish::execute () {
  printf("je m'execute en Fish...\n");
}

void Fish::update () {
  //Si le poisson est encore en vie aprÃs 6 tours
  // il y a un enfant
  if(this->age >= 9) {
    new Fish(this->world);
  }

  XPosition = XPosition + this->random(-1, 1);
  YPosition = YPosition + this->random(-1, 1);

  if(XPosition < 1) {
    XPosition = 1;
  }

  if(YPosition < 1) {
    YPosition = 1;
  }

  if(XPosition > world->getWidth()) {
    XPosition = world->getWidth();
  }

  if(YPosition > world->getHeight()) {
    YPosition = world->getHeight();
  }

  this->makeOld();
}

