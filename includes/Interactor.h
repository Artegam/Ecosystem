#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "Controller.h"
#include <algorithm>

using namespace Controller;

namespace Interactor {

  class WildlifeModel {
    // Attributes
    protected:
      char * name;
      int lifetime;
      unsigned int age = 0;
      char displayChar;
      int viewField;
      int XPosition;
      int YPosition;
      int defaultTurnsNumberBeforeStarving = 10;
      int turnsNumberBeforeStarving;
      World * world;
      vector<string> path;
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
      char * getName ();
      const unsigned int getLifetimeRemaining ();
      const unsigned int getAge ();
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
      void makeOld ();
      int random(const int min, const int max);
      void savePosition();
      bool isKnownedPosition(int posX, int posY);
      map<int, list<ClockSubscriber *>> openYourEyes();
			pair<int, int> calculateCoordinates (int index);
			unsigned int calculateIndex (pair<int, int> position);
			unsigned int calculateIndex (int x, int y);
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
		protected:
			void makeOld ();
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

};

#endif
