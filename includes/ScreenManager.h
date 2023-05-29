#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Interactor.h"
#include "FileManager.h"
#include <list>

using namespace Interactor;
using namespace FileManager;

namespace ScreenManager {
  /*
#define NEW      0;
#define LOAD     1;
#define SAVE     2;
#define GAMEOVER 3;
*/

  // interface
  /// class Menu - 
  class Menu {
    // Operations
    public:
      virtual void start ();
      virtual void load ();
      virtual void save ();
      virtual void end ();
  };

  // DataStructure
  /// class ScreenViewModel - 
  class ScreenViewModel : public Loggable, public GenericModel {
    public:
      const int MAIN_MENU = 0;
      const int SAVE_MENU = 1;
      const int LOAD_MENU = 2;
      const int IN_GAME   = 3;
      const int GAME_OVER = 4;
    private:
      int currentWindow;
      // Attributes
      WorldModel worldData;
      list<string> messages = Loggable::log();
    public:
      ScreenViewModel (WorldModel worldData);
      int getWorldHeight ();
      int getWorldWidth ();
      WorldModel getWorldData ();
      bool isRunning ();
      bool getTic ();
      unsigned int getWildlifeCount ();
      list<Wildlife *> getWildlife ();
      bool isWildlife (const ClockSubscriber * ptr);
      unsigned int getTurns ();
      const unsigned int getAverageAge();
      const unsigned int getLifeExpectancy ();
      string getRawData ();
      void save (void);
      void load (void);
      void setCurrentWindow(int window);
      int getCurrentWindow();
      list<string> log();
  };

  // interface
  /// class ScreenView - 
  class ScreenView {
    // Operations
    public:
      ScreenView ();
      virtual void init (int worldHeight, int worldWidth);
      //virtual void display (ScreenViewModel * data);
      virtual void mainMenu ();
      virtual void loadMenu (list<string> files);
      virtual void infos (list<string> infos);
      virtual void gameplay (map<int, int> worldMap, list<Wildlife *> wildlifes);
      virtual void end ();
      virtual int getScreen ();
      //*****************************************//
      virtual void keyboardListener(WorldModel worldData);
  };

  /// class ScreenPresenter - 
  class ScreenPresenter {
    // Attributes
    private:
      ScreenView * view;
      ScreenViewModel * data;
      World * world;

      int status = 0;
      // Operations
    public:
      ScreenPresenter (ScreenView * sv, World * world);
      void display ();
      void setStatus (const int newStatus);
      list<string> getInfos ();
      void start ();
      void load ();
      void save ();
      void end ();
      void print (WorldModel data);
      ScreenViewModel * getData ();
  };

}

#endif
