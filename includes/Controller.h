#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <thread>

using namespace std;

namespace Controller {

  // interface
  /// class ClockSubscriber - 
  class ClockSubscriber {
  private:
    unsigned int id = 0;
  protected:
    bool die = false;
    // Operations
  public:
    void setId (const unsigned int ident);
    const unsigned int getId ();
    virtual void update ();
    bool isDying ();
  };

  /// class Clock - 
  class Clock {
    // Attributes
  private:
    static bool running;
    static thread t1;
    static map<unsigned int, ClockSubscriber *>::iterator it;
    static unsigned int turns;
  public:
    static bool t;
    static unsigned int interval;
    static map<unsigned int, ClockSubscriber *> subscribers;
    // Operations
  public:
    Clock (unsigned int interv = 1000);
    void run ();
    void stop ();
    static void tick ();
    void subscribe (ClockSubscriber * s);
    void unsubscribe (ClockSubscriber * s);
    bool isRunning();
    unsigned int subscribersCount ();
    bool getT();
    map<unsigned int, ClockSubscriber *> getSubscribers();
    static unsigned int getTurns();
  };

  /// class World - 
  class World {
    // Attributes
  private:
    int height;
    int width;
    Clock * clock;
    // Operations
  public:
    World (int height = 20, int width = 30);
    int getWidth (void);
    int getHeight (void);
    Clock * getClock ();
    void setRefresh (unsigned int interval);
    void run (void);
  };

};

#endif
