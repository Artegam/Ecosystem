#include "Screen.h"
#include <iostream>

using namespace std;
using namespace Screen;

ScreenViewModel::ScreenViewModel (World * world) {
  this->world = world;
}

int ScreenViewModel::getWorldHeight () {
  WorldModel worldData = this->world->getData();
  return worldData.getHeight();
}

int ScreenViewModel::getWorldWidth () {
  WorldModel worldData = this->world->getData();
  return worldData.getWidth();
}

bool ScreenViewModel::isRunning () {
  WorldModel worldData = this->world->getData();
  return worldData.getClock()->isRunning();
}

bool ScreenViewModel::getTic() {
  WorldModel worldData = this->world->getData();
  return worldData.getClock()->getT();
}

unsigned int ScreenViewModel::getWildlifeCount() {
  WorldModel worldData = this->world->getData();
  return worldData.getClock()->subscribersCount();
}

list<Wildlife *> ScreenViewModel::getWildlife() {
  list<Wildlife *> lst;

  WorldModel worldData = this->world->getData();
  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(this->isWildlife(it->second)) {
      lst.push_back((Wildlife *)it->second);
    }
  }

  return lst;
}


bool ScreenViewModel::isWildlife(const ClockSubscriber * ptr) {
   return dynamic_cast<const ClockSubscriber *>(ptr) != nullptr;
}

unsigned int ScreenViewModel::getTurns () {
  WorldModel worldData = this->world->getData();
  return worldData.getClock()->getTurns();
}

const unsigned int ScreenViewModel::getAverageAge () {
  unsigned int averageAge = 0;

  WorldModel worldData = this->world->getData();
  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(this->isWildlife(it->second)) {
      Wildlife * life = (Wildlife *)it->second;
      averageAge += life->getAge();
    }
  }

  if(subscribers.size() > 0) {
    averageAge = averageAge / subscribers.size();
  } else {
    averageAge = 0;
  }

  return averageAge;
}

const unsigned int ScreenViewModel::getLifeExpectancy () {
  unsigned int lifeExpectancy = 0;

  WorldModel worldData = this->world->getData();
  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(this->isWildlife(it->second)) {
      Wildlife * life = (Wildlife *)it->second;
      lifeExpectancy += life->getAge() + life->getLifetimeRemaining();
    }
  }

  if(subscribers.size() > 0) {
    lifeExpectancy = lifeExpectancy / subscribers.size();
  } else {
    lifeExpectancy = 0;
  }

  return lifeExpectancy;
}
