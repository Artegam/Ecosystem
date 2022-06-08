#include "Controller.h"
#include <unistd.h>
#include <cstdio>

using namespace std;
using namespace Controller;

bool Clock::t = false;
unsigned int Clock::interval = 1000;
bool Clock::running = true;
map<unsigned int, ClockSubscriber *> Clock::subscribers;
map<unsigned int, ClockSubscriber *>::iterator Clock::it;
thread Clock::t1(Clock::tick);
unsigned int Clock::turns = 0;

Clock::Clock (unsigned int interv) {
}

void Clock::run () {
  //TODO: On peux s'en servir pour lancer l'horloge et ajouter un niveau ou booleen start/stop
}

void Clock::stop () {
  running = false;
}

void Clock::tick () {
  while(running) {
    for(it = subscribers.begin(); it != subscribers.end(); it++) {
      it->second->update();
    }
    t = !t;
    turns++;

    usleep(interval * 1000);
  }
}

void Clock::subscribe (ClockSubscriber * s) {
  unsigned int id = subscribers.size() + 1;
  s->setId(id);
  subscribers.insert({id, s});
}

void Clock::unsubscribe (ClockSubscriber * s) {
  it = subscribers.find(s->getId());
  it = subscribers.erase(it);
}

bool Clock::isRunning () {
  return running;
}

unsigned int Clock::subscribersCount () {
  return this->subscribers.size();
}

bool Clock::getT() {
  return this->t;
}

map<unsigned int, ClockSubscriber *> Clock::getSubscribers() {
  return subscribers;
}

unsigned int Clock::getTurns() {
  return turns;
}
