#include "Views.h"
#include "Logs.h"
#include "Translations.h"
#include <cstring>

using namespace Views;
using namespace Logs;
using namespace Translations;

#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define PLAIN_PAIR     3
#define SHARK_PAIR     4
#define FISH_PAIR      5

Views::NCurses::NCurses (Presenter * presenter) : View (presenter) {
  // Ncurses initialization
  initscr();
  noecho();
  nodelay(stdscr, TRUE); // For the keyboard
  getmaxyx(stdscr, this->windowHeight, this->windowWidth);
}

Views::NCurses::~NCurses () {
  endwin();
}

void Views::NCurses::init (int height, int width) {
  this->worldHeight = height;
  this->worldWidth = width;

  // Creation of the windows
  windows[0] = subwin(stdscr, LINES, COLS, 0, 0);

  // Enable keyboard for first standart screen
  keypad(stdscr, true);

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(EMPTY_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(WATER_PAIR, COLOR_BLUE, COLOR_BLACK);
  init_pair(PLAIN_PAIR, COLOR_GREEN, COLOR_BLACK);
  init_pair(SHARK_PAIR, COLOR_RED, COLOR_BLACK);
  init_pair(FISH_PAIR, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(EMPTY_PAIR));
}

void Views::NCurses::createWindow (int screen, int x, int y, int height, int width) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("create window() x: " + to_string(x) + " y: " + to_string(y) + " height: " + to_string(height) + " width: " + to_string(width));

  if(windows.size() < (unsigned long)(screen + 1)) 
    windows.resize(screen+1);
  windows[screen] = subwin(stdscr, height, width, y, x);

  //initScreen();
  refresh();
}

void Views::NCurses::drawChar (WINDOW * win, int x, int y, char c, char color) {
  wattron(win, COLOR_PAIR(color));
  mvwaddch(win, y, x, c);
  wattroff(win, COLOR_PAIR(color));
}

void Views::NCurses::redraw (WINDOW * win) {
  refresh();
  wmove(windows[0], 0, 0); // repositione le curseur
  wrefresh(windows[0]);
  wrefresh(win);
  usleep(20000);
}

/*
void Views::NCurses::display (int position, list<pair<pair<int, int>, string>> rawData, int keybPosition) {
  list<pair<pair<int, int>, string>>::iterator it;
  int cursorPosition = 0;

  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("1-Nb raw data: " + to_string(rawData.size()));
  logCursorPosition(keybPosition);

  //box(windows[position], ACS_VLINE, ACS_HLINE);
  for(it = rawData.begin(); it != rawData.end(); it++) {
    if(cursorPosition == keybPosition)
      wattron(windows[position], A_REVERSE);
// Le code suivant doit etre deplace

    string prefix = "";
    if (Item* item = dynamic_cast<Item*>(*it)) {
      if(item->isSelected()) {
        prefix = data->translate("selectedItem");
      } else {
        prefix = data->translate("unselectedItem");
      }
    }
    // ici doit etre imprime juste la chaine de caracteres calculee pour le presenter

    mvwprintw(windows[position], it->first.first, it->first.second, "%s", it->second.c_str());
    wattroff(windows[position], A_REVERSE);
    cursorPosition++;
  }

  wmove(windows[0], 1, 1); // repositione le curseur
  //wrefresh(windows[0]);
  wrefresh(windows[position]);
  usleep(20000);
  display((*s));
}
*/

void Views::NCurses::displayCursorPosition (int keybPosition) {
  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
}

void Views::NCurses::logCursorPosition (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("Cursor position: " + to_string(keybPosition));
}

// Le code suivant doit etre deplace
/* 
const unsigned int Views::NCurses::computeMaxWidth (list<string> menu) {
  list<Node *>::iterator it;
  unsigned int max = 0;
  unsigned int len = 0;

  for(it = menu.begin(); it != menu.end(); it++) {
    len = data->translate((*it)->getName()).size();
    if (dynamic_cast<Item*>(*it))
      len += 4;
    if (max < len)
      max = len;
  }

  return max;
}
*/
/*
void Views::NCurses::mainMenu (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("appel de mainMenu()");

  list<Node *> menu = data->getMenu();
  const unsigned int menuMaxWidth = computeMaxWidth(menu);
  const unsigned int edges = 2;
  windows[MAIN] = subwin(stdscr, menu.size()+edges, menuMaxWidth+edges, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  initScreen();
  box(windows[MAIN], ACS_VLINE, ACS_HLINE);
  int x = (this->windowWidth - data->getTitle().length()) / 2;
  int y = this->windowHeight / 4;

  mvwprintw(windows[ROOT], y, x, "%s", data->getTitle().c_str());
  refresh();

  displayCursorPosition(keybPosition);

  // Affiche le menu principal
  std::thread t_m(&NCurses::display, windows[MAIN], data, keybPosition);
  t_m.detach();

  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[MAIN]);
  usleep(20000);
}

void Views::NCurses::options (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("appel de options()");

  initScreen();

  windows[OPTIONS] = createWindow(data->getMenu().size(), 15);
  display(windows[OPTIONS], data, keybPosition);

  displayCursorPosition(keybPosition);
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[OPTIONS]);
  usleep(20000);
}

void Views::NCurses::languages (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("appel de languages()");

  initScreen();

  windows[LANGUAGES] = createWindow(data->getMenu().size(), 15);
  display(windows[LANGUAGES], data, keybPosition);

  displayCursorPosition(keybPosition);
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[LANGUAGES]);
  usleep(20000);
}

void Views::NCurses::video (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("appel de video()");

  initScreen();

  //windows[VIDEO] = createWindow(data->getMenu().size(), 15);
  windows[VIDEO] = createWindow(4, 15);
  display(windows[VIDEO], data, keybPosition);

  displayCursorPosition(keybPosition);
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[VIDEO]);
  usleep(20000);
}
*/

void Views::NCurses::validateOption (int optionNumber) {
/*
  list<Node *>::iterator it;
  list<Node *> menu = data->getMenu();

  it = menu.begin();
  advance(it, optionNumber);
  clearOptions(menu);
  (*it)->validate();
*/
  scr.select(optionNumber);
}


void Views::NCurses::clearScreen() {
  toClear = true;
}

void Views::NCurses::initScreen() {
  if (toClear) {
    clear();
    toClear = false;
  }
}

/*
void Views::NCurses::save (list<string> files, const unsigned int menuSize, int keybPosition) {
  string choices[menuSize];

  windows[SAVE] = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  initScreen();
  box(windows[SAVE], ACS_VLINE, ACS_HLINE);
  refresh();

  unsigned int idx = 0;
  list<string>::iterator it;
  for(it = files.begin(); it != files.end(); it++)
    choices[idx++] = it->c_str();

  if(files.size() == 0)
    choices[0] = "No saves";
  choices[menuSize-1] = "Back";

  int i;
  for (i = 0; i < (int)menuSize; i++) {
    if(i == keybPosition)
      wattron(windows[SAVE], A_REVERSE);
    mvwprintw(windows[SAVE], 1+i, 1, "%s", choices[i].c_str());
    wattroff(windows[SAVE], A_REVERSE);
  }

  // Ecoute le clavier
  displayCursorPosition(keybPosition);
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[SAVE]);
  usleep(20000);
}

void Views::NCurses::load (list<string> files, const unsigned int menuSize, int keybPosition) {
  string choices[menuSize];

  windows[LOAD] = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  initScreen();
  box(windows[LOAD], ACS_VLINE, ACS_HLINE);
  refresh();

  unsigned int idx = 0;
  list<string>::iterator it;
  for(it = files.begin(); it != files.end(); it++)
    choices[idx++] = it->c_str();

  if(files.size() == 0)
    choices[0] = "No saves";
  choices[menuSize-1] = "Back";

  int i;
  for (i = 0; i < (int)menuSize; i++) {
    if(i == keybPosition)
      wattron(windows[LOAD], A_REVERSE);
    mvwprintw(windows[LOAD], 1+i, 1, "%s", choices[i].c_str());
    wattroff(windows[LOAD], A_REVERSE);
  }

  // Ecoute le clavier
  displayCursorPosition(keybPosition);
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[LOAD]);
  usleep(20000);
}

void Views::NCurses::infos (list<string> infos) {
  initScreen();
  wprintw(windows[ROOT], "This is the virtual world");
  int ligne = 1;
  list<string>::iterator info;
  for(info = infos.begin(); info != infos.end(); info++) {
    mvwprintw(windows[ROOT], ligne, 50, "%s", info->c_str());
    ligne++;
  }
}
*/

void Views::NCurses::gameplay (int position, map<int, int> worldMap) {

  unsigned int const OCEAN = 0;
  unsigned int const PLAIN = 1;
  unsigned int const xOffset = 1;
  unsigned int const yOffset = 1;
  unsigned int size = 100;

  initScreen();
  windows[position] = subwin(stdscr, worldHeight + 2, worldWidth + 2, 1, 0);
  // dessin du bord de la fenetre
  box(windows[position], ACS_VLINE, ACS_HLINE);
  Dictionary dict;

  // Drawing the map
  // Ce n'est pas a l'afficheur de faire ce travail la... ce code doit etre deplace plus haut
  for(unsigned int index = 0; index < size; index++) {
    int x = xOffset + (index % worldWidth);
    int y = yOffset + ((index - (x - xOffset)) / worldWidth);

    if(worldMap[index] == OCEAN) {
      drawChar (windows[position], x, y, dict.translate("Water").c_str()[0], WATER_PAIR);
    } else if (worldMap[index] == PLAIN) {
      drawChar (windows[position], x, y, dict.translate("Plain").c_str()[0], PLAIN_PAIR);
    } else {
      drawChar (windows[position], x, y, dict.translate("Empty").c_str()[0], EMPTY_PAIR);
    }
  }

  //On affiche la faune
  // Ce n'est pas a l'afficheur de faire ce travail la... ce code doit etre deplace plus haut
/*
  list<Wildlife *>::iterator it;
  list<Wildlife *> wildlifes = data->getWildlife();
  for(it = wildlifes.begin(); it != wildlifes.end(); it++) {
    char type = (*it)->getDisplayChar();
    int x = (*it)->getX()+1;
    int y = (*it)->getY()+1;

    if(type == 'F') {
      drawChar (windows[position], x, y, type, FISH_PAIR);
    } else if(type == 'S') {
      drawChar (windows[position], x, y, type, SHARK_PAIR);
    } else {
      drawChar (windows[position], x, y, type, EMPTY_PAIR);
    }
  }
*/
  refresh();
  wrefresh(windows[position]);
  wmove(windows[0], 0, 50); // repositione le curseur
  wrefresh(windows[0]);
  usleep(200000);
  // fin dessin de la fenetre
}

void Views::NCurses::end () {
  mvwprintw(stdscr, 14, 20, "******************");
  mvwprintw(stdscr, 15, 20, "This is the end...");
  mvwprintw(stdscr, 16, 20, "******************");
  refresh();
  getch();
  endwin();
}

int Views::NCurses::getChar () {
  return wgetch(stdscr);
}

void Views::NCurses::hello () {
  // pour tester
    initscr();
    printw("Hello World");
    refresh();
    getch();
    endwin();
}

void Views::NCurses::display () {
  map<int, GraphicComponent *> lst;

  while(1) {
    wclear(windows[scr.window()]);
    keyboard();
    box(windows[scr.window()], ACS_VLINE, ACS_HLINE);

    lst = scr.components();

    mvwprintw(stdscr, 1, 1, "_keyboardx: %d", _keyboardx);

    for(map<int, GraphicComponent *>::iterator it = lst.begin(); it != lst.cend(); it++) {
      if (Text * text = dynamic_cast<Text*>(it->second); text != nullptr) {
        display((*text));
      } else if (Menu * menu = dynamic_cast<Menu*>(it->second); menu != nullptr) {
        display((*menu));
      }
    }

    wrefresh(windows[scr.window()]);
    _presenter->display();
    usleep(100000);
  }
}

void Views::NCurses::display (Screen screen) {
  scr = screen;
  View::display(scr);
}


void Views::NCurses::display (Menu menu) {
  list<string>::iterator it;
  list<string> items = nodesToString(menu.items());
  int y = menu.y();
  const int x = menu.x(); 
  int cursorPosition = 0;

  it = items.begin();
  advance(it, _keyboardx);
  mvwprintw(stdscr, 2, 1, "label: %s", (*it).c_str());
  mvwprintw(stdscr, 5, 1, "_maxKeyboardx: %d", _maxKeyboardx);

  //TODO: le 10 c'est la largeur, donc le calcul de la plus longue chaine de caracteres
  WINDOW * sub = subwin(windows[menu.window()], items.size() + 2, 10,  y - 1, x - 1);
  box(sub, ACS_VLINE, ACS_HLINE);

  for(it = items.begin(); it != items.end(); it++) {
    if(cursorPosition == _keyboardx)
      wattron(windows[menu.window()], A_REVERSE);

    mvwprintw(windows[menu.window()], y, menu.x(), "%s", (*it).c_str());

    if(cursorPosition == _keyboardx)
      wattroff(windows[menu.window()], A_REVERSE);

    y++;
    cursorPosition++;
  }

}

list<string> Views::NCurses::nodesToString (list<Node *> items) {
  list<Node *>::iterator it;
  list<string> lst;

  for(it = items.begin(); it != items.end(); it++) {
    if (GroupItem* grp = dynamic_cast<GroupItem*>(*it); grp != nullptr) {
      list<Node *> menugroup = grp->getChildren();
      list<string> lst_children = nodesToString(menugroup);
      lst.insert(lst.end(), lst_children.begin(), lst_children.end());
    } else {

      //TODO: Group Item est il un GraphicComponent ?
      string prefix = "";
      if (Item* item = dynamic_cast<Item*>(*it)) {
        if(item->isSelected()) {
          //prefix = dict->translate("selectedItem");
          prefix = "[*] ";
        } else {
          //prefix = data->translate("unselectedItem");
          prefix = "[ ] ";
        }
      }
      lst.push_back(prefix + (*it)->getName());
    }
  }
  return lst;
}

void Views::NCurses::display (Text text) {
  mvwprintw(windows[text.window()], text.y(), text.x(), "%s", text.label().c_str());
}

void Views::NCurses::keyboard () {
  unsigned char key = wgetch(stdscr);
  mvwprintw(stdscr, 3, 1, "keyboard: %d", key);
  string v = "-";
  if(_valid)
    v = "true";
  else
    v = "false";
  mvwprintw(stdscr, 4, 1, "isValid ?: %s", v.c_str());
  switch(key) {
    case KEYB_UP:
      if(_keyboardx > 0)
        _keyboardx--;
      break;
    case KEYB_DOWN:
      if(_keyboardx < _maxKeyboardx - 1)
        _keyboardx++;
      break;
    case KEYB_SPACE:
      break;
    case KEYB_ENTER:
      _valid = true;
      break;
    case KEYB_ESCAPE:
      break;
  }
}

