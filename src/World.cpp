#include "Controller.h"

using namespace Controller;

World::World (int height, int width) {
  this->height = height;
  this->width = width;
  this->clock = new Clock();
}

int World::getWidth (void) {
  return width;
}

int World::getHeight (void) {
  return height;
}

Clock * World::getClock () {
  return clock;
}

void World::setRefresh(unsigned int interval) {
  this->clock = new Clock(interval);
}

void World::run(void) {
  this->clock->run();
}
