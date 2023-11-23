#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "Model.h"

#include <map>
#include <thread>
#include <list>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <deque>

#include <iostream>
#include <fstream>

using namespace std;
using namespace Model;

namespace Interactor {
  // interface
  /// class Loggable - 
  class Loggable {
  private:
    // Operations
  public:
    virtual list<string> log ();
  };

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
    virtual GenericModel * getData ();
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
  class WorldModel : public Loggable, public GenericModel {
    private:
      // Attributes
      int height;
      int width;
      Clock * clock;
      map<int, int> worldMap;
      // Operations
    public:
      WorldModel ();
      WorldModel (int width, int height);
      string getRawData ();
      void init ();
      void setWidth (int w);
      void setHeight (int h);
      void setRefresh (unsigned int interval);
      int getWidth (void);
      int getHeight (void);
      Clock * getClock ();
      map<int, int> getWorldMap ();
      string getStringWorldMap ();
      void generateMap ();
      pair<int, int> calculateCoordinates (int index);
      unsigned int calculateIndex (pair<int, int> position);
      unsigned int calculateIndex (int x, int y);
      int random (const int min, const int max);
      list<string> log ();
  };

  // interface
  /// class Loggable - 
  class OutputDevice {
  private:
    // Operations
  public:
    virtual void print (WorldModel data);
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
		vector<vector<ClockSubscriber*>> getMap ();
    vector<vector<ClockSubscriber*>> getMap (int minX, int maxX, int minY, int maxY);
  };

  class WildlifeModel : public Loggable, public GenericModel {
    // Attributes
    protected:
      char * name;
      int lifetime;
      unsigned int age = 0;
      unsigned int maturityAge = 4;
      char displayChar;
      int viewField;
      int XPosition;
      int YPosition;
      int defaultTurnsNumberBeforeStarving = 10;
      int turnsNumberBeforeStarving;
      World * world;
      deque<pair<int, int>> path;
      int fieldOfView = 1;
			map<int, list<ClockSubscriber *>> vision;
      int movingTerrainType;

    public:
      // TODO: Mettre Terrain types dans une structure...
      // Terrain types
			unsigned int const OCEAN = 0;
			unsigned int const PLAIN = 1;

    // Operations
    public:
      WildlifeModel ();
      WildlifeModel (const WildlifeModel &wm);
      string getRawData ();
      char * getName ();
      const unsigned int getLifetimeRemaining ();
      const unsigned int getAge ();
      const unsigned int getMaturityAge ();
      char getDisplayChar ();
      int getViewField ();
      int getX ();
      int getY ();
			int getFieldOfView();
      World * getWorld ();
      int getTurnsNumberBeforeStarving ();
      void getHungry ();
			map<int, list<ClockSubscriber *>> getVision ();
      int getMovingTerrainType ();
      void happyBirthday ();
      void setMaturityAge(unsigned int age);
      void setName(char * name);
      void setLifetime (int min, int max);
      void setX (int x);
      void setY (int y);
      void setViewField (int v);
      void setWorld (World * w);
      void setDisplayChar (char c);
      void setDefaultTurnsNumberBeforeStarving (int turns);
			void setFieldOfView(int distance);
			void setVision (map<int, list<ClockSubscriber *>> v);
      void setMovingTerrainType (int terrainType);
      int random(const int min, const int max);
      void savePosition();
      bool isKnownedPosition(int posX, int posY);
      map<int, list<ClockSubscriber *>> openYourEyes();
			pair<int, int> calculateCoordinates (int index);
			unsigned int calculateIndex (pair<int, int> position);
			unsigned int calculateIndex (int x, int y);
			list<string> log ();
  };

  // interface
  /// class Behavior - 
  class Behavior {
    // Operations
    public:
      Behavior ();
      virtual void compute (WildlifeModel * wm);
  };


  // virtual
  /// class Wildlife - 
  class Wildlife : public ClockSubscriber {
    // Attributes
    protected:
      WildlifeModel * data;
      Behavior * action;
      // Operations
    public:
      Wildlife ();
      //Wildlife (const Wildlife * w); // standby
      Wildlife (World * world);
      Wildlife (WildlifeModel * parentData);
      ~Wildlife ();
      virtual void execute ();
      virtual void update ();
      const unsigned int getAge ();
      const unsigned int getLifetimeRemaining ();
      int getX ();
      int getY ();
      char getDisplayChar ();
      void addWildlife(string wildlifeName, int number);
      template <class T> bool cmp(pair<T, T>& x1, pair<T, T>& x2);
      GenericModel * getData ();
		protected:
			void happyBirthday ();
			bool isStarving ();
			bool isDead ();
  };

  /// class Fish - 
  class Fish : public Wildlife {
    // Operations
    public:
      Fish ();
      Fish (World * world);
      Fish (WildlifeModel * parentData);
      void init();
      void execute ();
      void update ();
  };

  /// class Shark - 
  class Shark : public Wildlife {
    // Operations
    public:
      Shark ();
      Shark (World * world);
      Shark (WildlifeModel * parentData);
      void init();
      void execute ();
      void update ();
  };

  /// class Default - Default Behavior for wildlife
  class Default : public Behavior {
    // Operations
    public:
      Default ();
      void compute (WildlifeModel * data);
      vector<int> getNewPosition(WildlifeModel * data);
  };

  /// class Aggressive - 
  class Aggressive : public Behavior {
    // Operations
    public:
      Aggressive ();
  };

  /// class Survival - 
  class Survival : public Behavior {
    // Operations
    public:
      Survival ();
      void compute (WildlifeModel * data);
      vector<int> getNewPosition(WildlifeModel * data);
  };

  /// class Explorer - 
  class Explorer : public Behavior {
    // Operations
    public:
      Explorer ();
      void compute (WildlifeModel * data);
      vector<int> getNewPosition(WildlifeModel * data);
  };



  /// class Logger - 
  class Logger {
  private:
		string filename;
    // Operations
  public:
    Logger (const string filename);
    void log (Loggable * l);
  };


};

#endif
