#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "Controller.h"

using namespace Controller;

namespace Interactor {

  // interface
  /// class Behavior - 
  class Behavior {
    // Operations
  public:
    void compute ();
  };

  // virtual
  /// class Wildlife - 
  class Wildlife : public ClockSubscriber {
    // Attributes
  protected:
    char * name;
    int lifetime;
    unsigned int age = 0;
    char displayChar;
    int viewField;
    int XPosition;
    int YPosition;
    Behavior * action;
    World * world;
    // Operations
  public:
    Wildlife ();
    Wildlife (World * world);
    ~Wildlife ();
    char * getName ();
    virtual void execute ();
    virtual void update ();
    int getX ();
    int getY ();
    char getDisplayChar ();
    const unsigned int getLifetimeRemaining ();
    const unsigned int getAge ();
  protected:
    int random(const int min, const int max);
    void makeOld ();
  };

  /// class Fish - 
  class Fish : public Wildlife {
    // Operations
  public:
    Fish ();
    Fish (World * world);
    void execute ();
    void update ();
  };

  /// class Shark - 
  class Shark : public Wildlife {
    // Operations
  public:
    Shark (World * world);
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
  };

};

#endif
