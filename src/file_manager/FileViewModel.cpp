#include "FileManager.h"
#include <iostream>
#include <experimental/filesystem>

using namespace std;
using namespace FileManager;
using namespace std::experimental::filesystem;

FileViewModel::FileViewModel (WorldModel worldData) {
  this->worldData = worldData;
}

int FileViewModel::getWorldHeight () {
  return worldData.getHeight();
}

int FileViewModel::getWorldWidth () {
  return worldData.getWidth();
}

WorldModel FileViewModel::getWorldData () {
  return worldData;
}

bool FileViewModel::isRunning () {
  return worldData.getClock()->isRunning();
}

bool FileViewModel::getTic() {
  return worldData.getClock()->getT();
}

unsigned int FileViewModel::getWildlifeCount() {
  return worldData.getClock()->subscribersCount();
}

list<Wildlife *> FileViewModel::getWildlife() {
  list<Wildlife *> lst;

  map<string, ClockSubscriber *>::iterator it;
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();

  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    if(this->isWildlife(it->second)) {
      lst.push_back((Wildlife *)it->second);
    }
  }

  return lst;
}


bool FileViewModel::isWildlife(const ClockSubscriber * ptr) {
  return dynamic_cast<const ClockSubscriber *>(ptr) != nullptr;
}

unsigned int FileViewModel::getTurns () {
  return worldData.getClock()->getTurns();
}

const unsigned int FileViewModel::getAverageAge () {
  unsigned int averageAge = 0;

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

const unsigned int FileViewModel::getLifeExpectancy () {
  unsigned int lifeExpectancy = 0;

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


string FileViewModel::getRawData () {
  //TODO: remplir ici (a reflechir)
  return "FileViewModel: to be implemented";
}

list<string> FileViewModel::log() {
  messages.push_back("appel a FileViewModel::log()");
  return messages;
}

