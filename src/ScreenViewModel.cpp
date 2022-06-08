#include "Screen.h"
#include <iostream>

using namespace std;
using namespace Screen;

ScreenViewModel::ScreenViewModel (World * world) {
  this->world = world;
}

int ScreenViewModel::getWorldHeight () {
  return this->world->getHeight();
}

int ScreenViewModel::getWorldWidth () {
  return this->world->getWidth();
}

bool ScreenViewModel::isRunning () {
  return this->world->getClock()->isRunning();
}

bool ScreenViewModel::getTic() {
  return this->world->getClock()->getT();
}

unsigned int ScreenViewModel::getWildlifeCount() {
  return this->world->getClock()->subscribersCount();
}

list<Wildlife *> ScreenViewModel::getWildlife() {
  list<Wildlife *> lst;

  map<unsigned int, ClockSubscriber *>::iterator it;
  map<unsigned int, ClockSubscriber *> subscribers = this->world->getClock()->getSubscribers();

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
  return this->world->getClock()->getTurns();
}

const unsigned int ScreenViewModel::getAverageAge () {
  unsigned int averageAge = 0;

  map<unsigned int, ClockSubscriber *>::iterator it;
  map<unsigned int, ClockSubscriber *> subscribers = this->world->getClock()->getSubscribers();

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

  map<unsigned int, ClockSubscriber *>::iterator it;
  map<unsigned int, ClockSubscriber *> subscribers = this->world->getClock()->getSubscribers();

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
