#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "Controller.h"

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
      ~Wildlife ();
      virtual void execute ();
      virtual void update ();
      const unsigned int getAge ();
      const unsigned int getLifetimeRemaining ();
      int getX ();
      int getY ();
      char getDisplayChar ();
    protected:
      void makeOld ();
  };

  /// class Fish - 
  class Fish : public Wildlife {
    // Operations
    public:
      Fish ();
      Fish (World * world);
      void init();
      void execute ();
      void update ();
  };

  /// class Shark - 
  class Shark : public Wildlife {
    // Operations
    public:
      Shark (World * world);
      void update ();
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
  };

};

#endif
