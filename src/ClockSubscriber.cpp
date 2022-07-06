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

int ClockSubscriber::getX () {
  printf("getX() dans ClockSubscriber\n");
  return 0;
}

int ClockSubscriber::getY () {
  printf("getY() dans ClockSubscriber\n");
  return 0;
}

