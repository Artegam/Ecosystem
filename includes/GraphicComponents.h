#ifndef GRAPHICCOMPONENTS_H
#define GRAPHICCOMPONENTS_H

#include <string>
#include <map>
#include <list>
#include <functional>

using namespace std;

namespace GraphicComponents {

  class Node {
    private:
      string name;
      //TODO: ici mettre le pointeur sur fonction ?
      //void (*fct)();
      Node * parent;

    protected:
      list<Node *> children;
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

    public:
      GroupItem (string name);
      GroupItem (Node * parent, string name);
      void setDefault (string name);
      Item * getSelectedItem ();
      void display ();
      void selectItem (string name);
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
      bool selected = false;
      bool selectable = false;
      int _window;
      int _selectedIndex = 0;
      int _selectSize;
      map<int, GraphicComponent *> _components;

    public:
      GraphicComponent();
      GraphicComponent(const int window, const int x, const int y);
      virtual void select(const int idComponent);
      void setSelectSize(const int size);
      const int getSelectSize();
      virtual void resize(const int height, const int width);
      const int id();
      const int window();
      const int x();
      const int y();
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
      const int selected ();
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
      list<Node *> items();
      list<string> textItems();
      void select(const int index);
  };

  class Text : public Leaf {
    private:
      string _label;
    public:
      Text (const int window, const int x, const int y, const string label);
      const string label();
  };

  class Button : public Leaf {
    private:
  };

  class Image : public Leaf {
    private:
  };

};

#endif
