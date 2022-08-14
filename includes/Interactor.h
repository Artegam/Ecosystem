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
      World * world;
      vector<string> path;
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
      World * getWorld ();
      void setName(char * name);
      void setLifetime (int lt);
      void setX (int x);
      void setY (int y);
      void setViewField (int v);
      void setWorld (World * w);
      void setDisplayChar (char c);
      void makeOld ();
      int random(const int min, const int max);
      void savePosition();
      bool isKnownedPosition(int posX, int posY);
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
      map<string, int> vision;
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
    //Attributes
    private:
      const int DAYS_BEFORE_STARVING = 10;
      int daysRemainingsBeforeStarvation = DAYS_BEFORE_STARVING;
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
