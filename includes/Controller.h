#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <thread>
#include <list>
#include <vector>
#include <string>

using namespace std;

namespace Controller {

  // interface
  /// class ClockSubscriber - 
  class ClockSubscriber {
  private:
    string id = "";
  protected:
    bool die = false;
    // Operations
  public:
    void setId (const string ident);
    const string getId ();
    virtual void update ();
    bool isDying ();
    int getX ();
    int getY ();
  };

  /// class Clock - 
  class Clock {
    // Attributes
  private:
    static bool running;
    static thread t1;
    static map<string, ClockSubscriber *>::iterator it;
    static unsigned int turns;
  public:
    static bool t;
    static unsigned int interval;
    static map<string, ClockSubscriber *> subscribers;
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
    map<string, ClockSubscriber *> getSubscribers();
    static unsigned int getTurns();
  };

  /// class WorldModel - 
  class WorldModel {
    // Attributes
		int height;
    int width;
    Clock * clock;
    map<int, int> worldMap;
  private:
    // Operations
  public:
    WorldModel ();
		WorldModel (int width, int height);
    void init ();
    void setWidth(int w);
    void setHeight(int h);
    void setRefresh (unsigned int interval);
    int getWidth (void);
    int getHeight (void);
    Clock * getClock ();
    map<int, int> getWorldMap ();
  };

  /// class World - 
  class World {
    // Attributes
  private:
    WorldModel data;
    // Operations
  public:
    World (int height = 20, int width = 30);
    WorldModel getData ();
		void addWildlife(string wildlifeName, int number);
    void run (void);
		vector<vector<ClockSubscriber*>> getMap();
    vector<vector<ClockSubscriber*>> getMap(int minX, int maxX, int minY, int maxY);
  };

};

#endif
