#include "Controller.h"

using namespace Controller;

World::World (int height, int width) {
  this->height = height;
  this->width = width;
	//this->map = list<Wildlife> map[width][height];
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

vector<vector<ClockSubscriber*>> World::getMap() {
  vector<vector<ClockSubscriber*>> myMap;

  map<string, ClockSubscriber *> subscribers;
  subscribers = this->clock->getSubscribers();
  map<string, ClockSubscriber *>::iterator it;

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    myMap[it->second->getX()][it->second->getY()] = it->second;
  }

	return myMap;
}

vector<vector<ClockSubscriber*>> World::getMap(int minX, int maxX, int minY, int maxY) {
  vector<vector<ClockSubscriber*>> myMap;

  map<string, ClockSubscriber *> subscribers;
  subscribers = this->clock->getSubscribers();
  map<string, ClockSubscriber *>::iterator it;

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(minX >= it->second->getX() && it->second->getX() <= maxX
        && minY >= it->second->getY() && it->second->getY() <= maxY) {
      myMap[it->second->getX()][it->second->getY()] = it->second;
    }
  }

	return myMap;
}
