#include "Controller.h"

using namespace Controller;

WorldModel::WorldModel () {
  init();
}

WorldModel::WorldModel (int width, int height) {
  init();
  this->width = width;
	this->height = height;
  //TODO: Generate wolrdMap here ???
}

void WorldModel::init() {
  this->clock = new Clock();
}

void WorldModel::setWidth(int w) {
  width = w;
}

void WorldModel::setHeight(int h) {
  height = h;
}

void WorldModel::setRefresh(unsigned int interval) {
  this->clock = new Clock(interval);
}

int WorldModel::getWidth (void) {
  return width;
}

int WorldModel::getHeight (void) {
  return height;
}

Clock * WorldModel::getClock () {
  return clock;
}

map<int, int> WorldModel::getWorldMap () {
  return worldMap; 
}

