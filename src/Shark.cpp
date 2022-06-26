#include "Interactor.h"
#include <stdio.h>

using namespace Interactor;
using namespace Controller;

Shark::Shark () {
  this->name = (char *)"Sharko";
  this->lifetime = 5;
  this->displayChar = 'S';
  this->viewField = 2;
  this->action = new Explorer ();
}

Shark::Shark (World * world) : Wildlife (world) {
  //TODO: Appeler le constructeur par defaut ici mais comment ???
  this->name = (char *)"Sharko";
  this->lifetime = this->random(5, 10);
  this->displayChar = 'S';
  this->viewField = 1;
  this->action = new Explorer ();
}

void Shark::execute () {
  printf("je m'execute en Shark...\n");
}

void Shark::update () : update () {
  // code specifique ici
}

