#include "Interactor.h"
#include <unistd.h>
#include <cstdio>
#include "uuid.h"

using namespace std;
using namespace Interactor;
using namespace uuid;

bool Clock::t = false;
unsigned int Clock::interval = 1000;
bool Clock::running = true;
map<string, ClockSubscriber *> Clock::subscribers;
map<string, ClockSubscriber *>::iterator Clock::it;
thread Clock::t1(Clock::tick);
unsigned int Clock::turns = 0;

Clock::Clock (unsigned int interv) {
}

void Clock::run () {
  //TODO: On peux s'en servir pour lancer l'horloge et ajouter un niveau ou booleen start/stop
  running = true;
}

void Clock::stop () {
  running = false;
}

void Clock::tick () {
  while(1) {
    if(running) {
      for(it = subscribers.begin(); it != subscribers.end(); it++) {
        it->second->update();
      }
      t = !t;
      turns++;
    }
    usleep(interval * 1000);
  }
}

void Clock::subscribe (ClockSubscriber * s) {
  string id = generate_uuid_v4();
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

map<string, ClockSubscriber *> Clock::getSubscribers() {
  return subscribers;
}

unsigned int Clock::getTurns() {
  return turns;
}
