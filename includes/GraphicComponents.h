#ifndef GRAPHICCOMPONENTS_H
#define GRAPHICCOMPONENTS_H

#include <string>
#include <map>
#include <list>
#include <functional>

#include "Translations.h"

using namespace std;
using namespace Translations;

namespace GraphicComponents {

  struct basic {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
  };

  struct tab {
    unsigned int rows;
    unsigned int cols;
    unsigned int colmaxsize = 0;
    unsigned int rowmaxsize = 0;
    list<unsigned int> colssizes;
    list<unsigned int> rowssizes;
  };

  struct cursor {
    unsigned int row = 0;
    unsigned int col = 0;
  };

  class Utils {
    public:
      const unsigned int occurences(const string str, char c);
  };

  class Node {
    private:
      string _key;
      string _name;
      //TODO: ici mettre le pointeur sur fonction ?
      //void (*fct)();
      Node * _parent;

    protected:
      list<Node *> children;
    public:
      Node (string key);
      Node (Node * parent, string key);
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
      void translate(Dictionary dict);
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
      unsigned int defaultItem;
      unsigned int selectedItem;

    public:
      GroupItem (string name);
      GroupItem (Node * parent, string name);
      void setDefault (const int index);
      Item * getSelectedItem ();
      void display ();
      void selectItem (const int index);
      const int size();
  };

  // interface
  /// class View - 
  class GraphicComponent {
    protected:
      int _x;
      int _y;
      int _height;
      int _width;
      int _id;
      bool _validated = false;
      bool selectable = false;
      int _window;
      int _selectedIndex = 0;
      int _selectSize;
      map<int, GraphicComponent *> _components;
      cursor _cursor;

    public:
      GraphicComponent();
      GraphicComponent(const int window, const int x, const int y);
      virtual void select(const int idComponent);
      virtual const int selected ();
      const bool isValidated ();
      void setSelectSize(const int size);
      const int getSelectSize();
      virtual void resize(const int height, const int width);
      const int id();
      const int window();
      const int x();
      const int y();
      const unsigned int height ();
      const unsigned int width ();
      const basic getBasic();
      list<string> nodesToString (list<Node *> items);
      cursor getCursor ();
  };

  class Leaf : public GraphicComponent {
    private:
    public:
      Leaf(const int window, const int x, const int y);
      virtual void select(const int idComponent);
  };

  class Composite : public GraphicComponent {
    private:
    public:
      Composite ();
      Composite (const int window, const int x, const int y);
      void add (GraphicComponent * gc);
      map<int, GraphicComponent *> components();
      void select(const int index);
      void resize(const int height, const int width);
  };

  class Screen : public Composite {
    private:
      map<int, GraphicComponent *> _components;
    public:
      Screen ();
      Screen (const int window, const int x, const int y);
      void select(const int index);
  };

  class Menu : public Composite {
    private:
      list<Node *> _items;
    public:
      Menu (const int window, const int x, const int y, list<string> items);
      Menu (const int window, const int x, const int y, list<Node *> items);
      list<string> items();
      void select(const int index);
  };

  class Text : public Leaf {
    private:
      string _label;
    public:
      Text (const int window, const int x, const int y, const string label);
      const string label();
  };

  class Selector : public Text {
    public:
      Selector (const int window, const int x, const int y, const string label);
  };

  class Button : public Leaf {
    private:
  };

  class Image : public Leaf {
    private:
  };

  class Cell : public Leaf {
    private:
      string _value;
      bool _select = false;
    public:
      Cell (const int window, const int x, const int y, const string value);
      void setWidth(const unsigned int w);
      void setHeight(const unsigned int h);
      const string value();
      void setValue(const string value);
      void select();
      bool isSelected();
  };

  class Table : public Leaf {
    private:
      tab _t;
      list<Cell*> cells;
    public:
      Table (const int window, const int x, const int y, const unsigned int rows, const unsigned int cols, string defaultval = "-");
      list<Cell*> getCells ();
      list<Cell*> getRow (unsigned int n);
      Cell getCell (const unsigned int pos);
      Cell * getCell(unsigned int row, unsigned int col);
      const tab getTab();
      void setValue(unsigned int row, unsigned int col, string value);
      void setValue(unsigned int row, unsigned int col, const char * value);
      void updateLengths ();
      void select(unsigned int row, unsigned int col);
      void setCursor(unsigned int row, unsigned int col);
      Cell * getCursor ();
  };

  class ScrollBar : public Leaf {

  };

  class Calendar : public Leaf {
    private:
      Table * _daily;
      Selector * _month;
      Selector * _year;
    public:
      Calendar (const int window, const int x, const int y);
      Selector getMonth ();
      Selector getYear ();
      Table getDaily ();
  };

  class Agenda : public Leaf {
    private:
      Table * _hourly;
      Selector * _day;
      Selector * _month;
      Selector * _year;
      Text * _name;
    public:
      Agenda (const int window, const int x, const int y);
      Selector getDay ();
      Selector getMonth ();
      Selector getYear ();
      Table getHourly ();
      Text getName ();
  };

};

#endif
