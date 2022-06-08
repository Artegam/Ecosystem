#include "Controller.h"
#include "Interactor.h"
#include <list>

using namespace Controller;
using namespace Interactor;

namespace Screen {

  // DataStructure
  /// class ScreenViewModel - 
  class ScreenViewModel {
    // Attributes
    World * world;
  public:
    ScreenViewModel (World * world);
    int getWorldHeight ();
    int getWorldWidth ();
    bool isRunning ();
    bool getTic ();
    unsigned int getWildlifeCount ();
    list<Wildlife *> getWildlife ();
    bool isWildlife (const ClockSubscriber * ptr);
    unsigned int getTurns ();
    const unsigned int getAverageAge();
    const unsigned int getLifeExpectancy ();
  };

  // interface
  /// class ScreenView - 
  class ScreenView {
    // Operations
  public:
    ScreenView ();
    virtual void display (ScreenViewModel * data);
  };

  /// class ScreenPresenter - 
  class ScreenPresenter {
    // Attributes
  private:
    ScreenView * view;
    ScreenViewModel * data;
    World * world;
    // Operations
  public:
    ScreenPresenter (ScreenView * sv, World * world);
    void display ();
  };

};

