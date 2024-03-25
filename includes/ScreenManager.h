#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Interactor.h"
#include "FileManager.h"
#include <list>

using namespace Interactor;
using namespace FileManager;

namespace ScreenManager {

  // Keyboard keys
  const int KEYB_UP     = 0;
  const int KEYB_DOWN   = 1;
  const int KEYB_SPACE  = 2;
  const int KEYB_ENTER  = 3;
  const int KEYB_ESCAPE = 4;

  // Screens
  const int MAIN        = 0;
  const int OPTIONS     = 1;
  const int SAVE        = 2;
  const int LOAD        = 3;
  const int IN_GAME     = 4;
  const int GAME_OVER   = 5;

  // interface
  /// class Menu - 
  class Menu {
    // Operations
    public:
      virtual void start ();
      virtual void options ();
      virtual void load ();
      virtual void save ();
      virtual void end ();
  };

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

  class Node {
    private:
      string name;
      list<Node *> children;

    public:
      Node (string name);
      void add (Node * node);
      list<Node *> getChildren ();
      virtual string getName ();
      virtual void validate ();
  };

  class Item : public Node {
    private:
      bool selected = false;
      function<void(Item *)> fct;

    public:
      Item (string name);
      void select ();
      bool isSelected ();
      string getName ();
      void validate ();
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

  // DataStructure
  /// class ScreenViewModel - 
  class ScreenViewModel : public Loggable, public GenericModel {
    public:
    private:
      int currentWindow;
      // Attributes
      WorldModel worldData;
      list<string> messages = Loggable::log();
      Node * root;
      Keyboard * keyboard;

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
      Node * getMenu ();
  };

  // interface
  /// class ScreenView - 
  class ScreenView {
    private:
      ScreenViewModel * model;

    protected:
      bool toClear = false;

    // Operations
    public:
      ScreenView ();
      virtual void init (ScreenViewModel * data);
      virtual void mainMenu ();
      virtual void options (list<string> options);
      virtual void load (list<string> files);
      virtual void infos (list<string> infos);
      virtual void gameplay ();
      virtual void end ();
      virtual int getChar ();
      void clearScreen ();
      //*****************************************//
      virtual void keyboardListener(WorldModel worldData);
  };

  /// class ScreenPresenter - 
  class ScreenPresenter : public OutputDevice {
    // Attributes
    private:
      ScreenView * view;
      ScreenViewModel * data;
      World * world;
      Keyboard * keyb;

      int screen = MAIN;
      // Operations
    public:
      ScreenPresenter (World * world, ScreenViewModel * svm, Keyboard * kb, ScreenView * sv);
      void display ();
      void setStatus (const int newStatus);
      list<string> getInfos ();
      void start ();
      void load ();
      void save ();
      void end ();
      void print (WorldModel data);
      int getScreen ();
      void changeScreen (const int nextScreen);
      ScreenViewModel * getData ();
  };

}

#endif
