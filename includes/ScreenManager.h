#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Interactor.h"
#include "FileManager.h"
#include "Sound.h"
#include "Translations.h"
#include "GraphicComponents.h"
#include "Views.h"
#include <list>
#include <map>

using namespace Interactor;
using namespace FileManager;
using namespace sound;
using namespace Translations;
using namespace GraphicComponents;
using namespace Views;

namespace ScreenManager {
  // Keyboard keys
  const int KEYB_UP     = 0;
  const int KEYB_DOWN   = 1;
  const int KEYB_SPACE  = 2;
  const int KEYB_ENTER  = 3;
  const int KEYB_ESCAPE = 4;

  // Screens
  const int MAIN      = 0;
  const int OPTIONS   = 1;
  const int LANGUAGES = 2;
  const int VIDEO     = 3;
  const int SAVE      = 4;
  const int LOAD      = 5;
  const int IN_GAME   = 6;
  const int GAME_OVER = 7;


  class InputDevice {
  };

  class Keyboard : public InputDevice {
    private:
      const int keybPosition_default = 0;
      int keybPosition = keybPosition_default;
      int positionsCount = 1;
      bool valid = false;

    public:
      virtual void keyPressed (int key);
      virtual void defaultPosition ();
      virtual void setPositionsCount (const unsigned int count);
      //virtual void setDefaultPosition (const unsigned int position);
      virtual int getPosition ();
      virtual void listen (unsigned int character);
      void onUp ();
      void onDown ();
      void onEnter ();
      void resetValid ();
      bool isValid ();
  };

  // interface
  class Function {
    public:
      virtual void execute();
  };

  class MenuFunction : public Function {
    public:
      void execute ();
  };

  // interface
  /// class Sound -
  class Sound {
    private:

    public:
      Sound();
  };

  // DataStructure
  /// class ScreenViewModel - 
  class ScreenViewModel : public Loggable, public GenericModel {
    private:
      // Attributes
      WorldModel worldData;
      list<string> messages = Loggable::log();
      list<Node *> parentNodes;
      Node * currentNode;
      list<Node *> root_lst;
      Node * root;
      Keyboard * keyboard;
      Sound * snd;
      map<int, Screen *> screens;

      /*
         mode meanings:
         1 - NCurses
         2 - OpenGL
       */
      int mode = 1;

    public:
      ScreenViewModel ();
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
      void save (void);
      void load (void);
      void setCurrentWindow(int window);
      int getCurrentWindow();
      list<string> log();
      void validate(int position);
      void back();
      list<Node *> getMenu ();
      list<string> getMenuText ();
      list<string> getMenuText (Node * node);
      string getRawData ();
      list<Node *> getParents ();
      string getTitle ();
      void loadMenu ();
      void setLanguage (const int lang);
      void setMode (int m);
      int getMode ();
      list<pair<pair<int, int>, string>> getDataScreen (const int screenView);
      list<pair<pair<int, int>, string>> getDataMenu ();
      void createWindow (int position, int height, int width);
      Screen getScreen(const int screen);
  };

  /// class ScreenPresenter - 
  class ScreenPresenter : public Presenter, public OutputDevice {
    // Attributes
    private:
      View * view; // TO BE FIXED: the current view is a data => Model
      ScreenViewModel data;
      World * world;
      Keyboard * keyb;

      int screen = MAIN;
      // Operations
    public:
      ScreenPresenter (World * world, ScreenViewModel svm);
      void start ();
      void display ();
      void setStatus (const int newStatus);
      list<string> getInfos ();
      void load ();
      void save ();
      void end ();
      void print (WorldModel data);
      int getScreen ();
      void changeScreen (const int nextScreen);
      ScreenViewModel getData ();
      const unsigned int computeMaxWidth (list<string> menu);
  };


}

#endif
