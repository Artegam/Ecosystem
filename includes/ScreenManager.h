#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Interactor.h"
#include "FileManager.h"
#include "Sound.h"
#include <functional>
#include <list>

using namespace Interactor;
using namespace FileManager;
using namespace sound;

namespace ScreenManager {
  // Keyboard keys
  const int KEYB_UP     = 0;
  const int KEYB_DOWN   = 1;
  const int KEYB_SPACE  = 2;
  const int KEYB_ENTER  = 3;
  const int KEYB_ESCAPE = 4;

  // Screens
  const int ROOT       = 0;
  const int MAIN       = 1;
  const int OPTIONS    = 2;
  const int LANGUAGES  = 3;
  const int VIDEO      = 4;
  const int SAVE       = 5;
  const int LOAD       = 6;
  const int IN_GAME    = 7;
  const int GAME_OVER  = 8;

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

  class Node {
    private:
      string name;
      //TODO: ici mettre le pointeur sur fonction ?
      //void (*fct)();
      list<Node *> children;
      Node * parent;

    public:
      Node (string name);
      Node (Node * parent, string name);
      void add (Node * node);
      void add (string name);
      void addItem (string name);
      void addGroup (string name);
      void erase (unsigned int position);
      list<Node *> getChildren ();
      virtual string getName ();
      Node * getParent ();
      Node * getNode(string name);
      virtual bool validate ();
      virtual void clear ();
  };

  class Item : public Node {
    private:
      bool selected = false;
      std::function<void(Item *)> fct;

    public:
      Item (string name);
      Item (Node * parent, string name);
      void select ();
      bool isSelected ();
      string getName ();
      bool validate ();
      void clear ();
  };

  class GroupItem : public Node {
    private:
      string defaultItem;
      string selectedItem;
      list<Item *> grp;

    public:
      GroupItem (string name);
      GroupItem (Node * parent, string name);
      void setDefault (string name);
      Item * getSelectedItem ();
      void display ();
      void selectItem (string name);
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
      list<string> languages;
      unsigned int language = 0;
      map<string, string> dict;

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
      void validate(int position);
      void back();
      list<Node *> getMenu ();
      list<Node *> getParents ();
      string getTitle ();
      map<string, string> getDictionary();
      list<list<string>> getLanguages ();
      map<string, string> getTheme ();
      list<list<string>> getFile (string filename);
      unsigned int getLanguage ();
      void setLanguage (unsigned int lang = 0);
      void loadMenu ();
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
      virtual void mainMenu (int keybPosition = -1);
      virtual void options (int keybPosition = -1);
      virtual void languages (int keybPosition = -1);
      virtual void video (int keybPosition = -1);
      virtual void load (list<string> files, const unsigned int menuSize, int keybPosition = -1);
      virtual void save (list<string> files, const unsigned int menuSize, int keybPosition = -1);
      //virtual void save (string filename);
      virtual void infos (list<string> infos);
      virtual void gameplay ();
      virtual void end ();
      virtual int getChar ();
      void clearScreen ();
      void clearOptions (list<Node *> options);
      //*****************************************//
      virtual void keyboardListener(WorldModel worldData);
      virtual void validateOption (int optionNumber);
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
