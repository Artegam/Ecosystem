#include "Controller.h"

using namespace Controller;

World::World (int height, int width) {
  data.setHeight(height);
  data.setWidth(width);
	//this->map = list<Wildlife> map[width][height];
  data.generateMap();
}

WorldModel World::getData() {
  return data;
}

void World::run(void) {
  data.getClock()->run();
}

vector<vector<ClockSubscriber*>> World::getMap() {
  vector<vector<ClockSubscriber*>> myMap;

  map<string, ClockSubscriber *> subscribers;
  subscribers = data.getClock()->getSubscribers();
  map<string, ClockSubscriber *>::iterator it;

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    myMap[it->second->getX()][it->second->getY()] = it->second;
  }

	return myMap;
}

vector<vector<ClockSubscriber*>> World::getMap(int minX, int maxX, int minY, int maxY) {
  vector<vector<ClockSubscriber*>> myMap;

  map<string, ClockSubscriber *> subscribers;
  subscribers = data.getClock()->getSubscribers();
  map<string, ClockSubscriber *>::iterator it;

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(minX >= it->second->getX() && it->second->getX() <= maxX
        && minY >= it->second->getY() && it->second->getY() <= maxY) {
      myMap[it->second->getX()][it->second->getY()] = it->second;
    }
  }

	return myMap;
}
