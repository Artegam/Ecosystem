#include "Controller.h"
#include <stdio.h>

using namespace Controller;


void ClockSubscriber::setId (const unsigned int ident) {
  this->id = ident;
}

const unsigned int ClockSubscriber::getId () {
  return this->id;
}

void ClockSubscriber::update () {
  printf("update dans ClockSubscriber\n");
}

