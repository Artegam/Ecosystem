#include "Screen.h"
#include <iostream>

using namespace std;
using namespace Screen;

ScreenViewModel::ScreenViewModel (WorldModel worldData) {
  this->worldData = worldData;
}

int ScreenViewModel::getWorldHeight () {
  return worldData.getHeight();
}

int ScreenViewModel::getWorldWidth () {
  return worldData.getWidth();
}

WorldModel ScreenViewModel::getWorldData () {
  return worldData;
}

bool ScreenViewModel::isRunning () {
  return worldData.getClock()->isRunning();
}

bool ScreenViewModel::getTic() {
  return worldData.getClock()->getT();
}

unsigned int ScreenViewModel::getWildlifeCount() {
  return worldData.getClock()->subscribersCount();
}

list<Wildlife *> ScreenViewModel::getWildlife() {
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


bool ScreenViewModel::isWildlife(const ClockSubscriber * ptr) {
  return dynamic_cast<const ClockSubscriber *>(ptr) != nullptr;
}

unsigned int ScreenViewModel::getTurns () {
  return worldData.getClock()->getTurns();
}

const unsigned int ScreenViewModel::getAverageAge () {
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

const unsigned int ScreenViewModel::getLifeExpectancy () {
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


string ScreenViewModel::getRawData () {
  //TODO: remplir ici (a reflechir)
  return "ScreenViewModel: to be implemented";
}

void ScreenViewModel::save(void) {
  FILE* fp = fopen("saves/ecosystem.save", "w+");
  string str;
  /* TODO: Error management
     if (!fp) {
     perror("File opening failed");
     return EXIT_FAILURE;
     }
     */
  fprintf(fp, "%d\n", worldData.getHeight());
  fprintf(fp, "%d\n", worldData.getWidth());
  //Clock
  fprintf(fp, "%d\n", worldData.getClock()->isRunning());
  fprintf(fp, "%u\n", worldData.getClock()->getTurns());
  fprintf(fp, "%d\n", worldData.getClock()->getT());
  fprintf(fp, "%u\n", worldData.getClock()->interval);
  // Clock subscribers
  map<string, ClockSubscriber *> subscribers = worldData.getClock()->getSubscribers();
  map<string, ClockSubscriber *>::iterator it;
  for(it = subscribers.begin(); it != subscribers.end(); it++) {
    Wildlife * wl = (Wildlife *)it->second;
    GenericModel * data = wl->getData();
    str = data->getRawData();
    fprintf(fp, "%s\n", str.c_str());
  }
  //World map
  str = worldData.getRawData();
  fprintf(fp, "%s\n", str.c_str());
  // Screen View Model
  str = getRawData();
  fprintf(fp, "%s\n", str.c_str());
  fclose(fp);
}

void ScreenViewModel::load(void) {
  FILE* fp = fopen("saves/ecosystem.save", "r+");
  /* TODO: Error management
     if (!fp) {
     perror("File opening failed");
     return EXIT_FAILURE;
     }
     */
  const int MAX_LEN = 512;
  char line[MAX_LEN], *result;
  while(!feof(fp)) {
    result = fgets(line, MAX_LEN, fp);
    if(result != NULL) {
      // TODO: Ou et comment stocker le resultat
      /*
         printf("The line is: %s\n", line);
         printf("The result is: %s\n", result);
         */
    }
  }
  fclose(fp);

}


