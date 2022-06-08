#include "Controller.h"
#include <stdio.h>

using namespace Controller;


void ClockSubscriber::setId (const string ident) {
  this->id = ident;
}

const string ClockSubscriber::getId () {
  return this->id;
}

void ClockSubscriber::update () {
  printf("update dans ClockSubscriber\n");
}

