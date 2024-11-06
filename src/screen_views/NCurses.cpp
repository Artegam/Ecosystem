#include "ScreenViews.h"
#include <cstring>

using namespace ScreenViews;

#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define PLAIN_PAIR     3
#define SHARK_PAIR     4
#define FISH_PAIR      5

ScreenViews::NCurses::NCurses (Keyboard * keyb) {
  keyb = keyb;

  // Ncurses initialization
  initscr();
  noecho();
  nodelay(stdscr, TRUE); // For the keyboard

  getmaxyx(stdscr, this->windowHeight, this->windowWidth);
}

ScreenViews::NCurses::~NCurses () {
  endwin();
}

void ScreenViews::NCurses::init (ScreenViewModel * data) {
  this->data = data;
  this->worldHeight = data->getWorldData().getHeight();
  this->worldWidth = data->getWorldData().getWidth();

  // Creation of the windows
  windows[ROOT] = subwin(stdscr, LINES, COLS, 0, 0);
  windows[IN_GAME] = subwin(stdscr, worldHeight + 2, worldWidth + 2, 1, 0);
  box(windows[IN_GAME], ACS_VLINE, ACS_HLINE);

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
  clear();
}

WINDOW * ScreenViews::NCurses::createWindow (int height, int width) {
  const int edges = 2;
  const int yOffset = (this->windowHeight / 2) - 5;
  const int xOffset = ((this->windowWidth - width) / 2);

  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("create window() size: " + to_string(height) + " width: " + to_string(width));

  WINDOW * window = subwin(stdscr, height + edges, width, yOffset, xOffset);
  box(window, ACS_VLINE, ACS_HLINE);
  refresh();
  return window;
}

WINDOW * ScreenViews::NCurses::getWindow () {
  return windows[currentWindow];
}

void ScreenViews::NCurses::drawChar (WINDOW * win, int x, int y, char c, char color) {
  wattron(win, COLOR_PAIR(color));
  mvwaddch(win, y, x, c);
  wattroff(win, COLOR_PAIR(color));
}

void ScreenViews::NCurses::redraw (WINDOW * win) {
  refresh();
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(win);
  usleep(20000);
}

void ScreenViews::NCurses::display (WINDOW * win, list<Node *> menu, map<string, string> dictionary, int keybPosition) {
  list<Node *>::iterator it;
  int i = 0;

  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("1-Nb menu: " + to_string(menu.size()));
  logCursorPosition(keybPosition);

  for(it = menu.begin(); it != menu.end(); it++) {
    if(i == keybPosition)
      wattron(win, A_REVERSE);
    string prefix = "";
    if (Item* item = dynamic_cast<Item*>(*it)) {
      if(item->isSelected()) {
        prefix = dictionary["selectedItem"];
      } else {
        prefix = dictionary["unselectedItem"];
      }
    }
    mvwprintw(win, 1+i, 1, "%s%s", prefix.c_str(), dictionary[(*it)->getName()].c_str());
    wattroff(win, A_REVERSE);
    i++;
  }
}

void ScreenViews::NCurses::displayCursorPosition (int keybPosition) {
  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
}

void ScreenViews::NCurses::logCursorPosition (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("Cursor position: " + to_string(keybPosition));
}

void ScreenViews::NCurses::mainMenu (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("appel de mainMenu()");
//TODO: dimentionner automatiquement la largeur de la fenêtre en focntion du menu
  const unsigned int menuSize = 5;
  windows[MAIN] = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
  box(windows[MAIN], ACS_VLINE, ACS_HLINE);
  int x = (this->windowWidth - data->getTitle().length()) / 2;
  int y = this->windowHeight / 4;

  mvwprintw(windows[ROOT], y, x, "%s", data->getTitle().c_str());
  refresh();

  displayCursorPosition(keybPosition);

  // Affiche le menu principal
  std::thread t_m(&NCurses::display, windows[MAIN], data->getMenu(), data->getDictionary(), keybPosition);
  t_m.detach();

  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[MAIN]);
  usleep(20000);
}

void ScreenViews::NCurses::options (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("appel de options()");

  if (toClear) {
    clear();
    toClear = false;
  }

  windows[OPTIONS] = createWindow(data->getMenu().size(), 15);
  display(windows[OPTIONS], data->getMenu(), data->getDictionary(), keybPosition);

  displayCursorPosition(keybPosition);
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[OPTIONS]);
  usleep(20000);
}

void ScreenViews::NCurses::languages (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("appel de languages()");

  if (toClear) {
    clear();
    toClear = false;
  }

  windows[LANGUAGES] = createWindow(data->getMenu().size(), 15);
  display(windows[LANGUAGES], data->getMenu(), data->getDictionary(), keybPosition);

  displayCursorPosition(keybPosition);
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[LANGUAGES]);
  usleep(20000);
}

void ScreenViews::NCurses::video (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("appel de video()");

  if (toClear) {
    clear();
    toClear = false;
  }

  //windows[VIDEO] = createWindow(data->getMenu().size(), 15);
  windows[VIDEO] = createWindow(4, 15);
  display(windows[VIDEO], data->getMenu(), data->getDictionary(), keybPosition);

  displayCursorPosition(keybPosition);
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[VIDEO]);
  usleep(20000);
}

void ScreenViews::NCurses::validateOption (int optionNumber) {
  list<Node *>::iterator it;
  list<Node *> menu = data->getMenu();

  it = menu.begin();
  advance(it, optionNumber);
  clearOptions(menu);
  (*it)->validate();
}

void ScreenViews::NCurses::save (list<string> files, const unsigned int menuSize, int keybPosition) {
  string choices[menuSize];

  windows[SAVE] = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
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

void ScreenViews::NCurses::load (list<string> files, const unsigned int menuSize, int keybPosition) {
  string choices[menuSize];

  windows[LOAD] = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
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

void ScreenViews::NCurses::infos (list<string> infos) {
  if (toClear) {
    clear();
    toClear = false;
  }
  wprintw(windows[ROOT], "This is the virtual world");
  int ligne = 1;
  list<string>::iterator info;
  for(info = infos.begin(); info != infos.end(); info++) {
    mvwprintw(windows[ROOT], ligne, 50, "%s", info->c_str());
    ligne++;
  }
}

void ScreenViews::NCurses::gameplay () {

  unsigned int const OCEAN = 0;
  unsigned int const PLAIN = 1;
  unsigned int const xOffset = 1;
  unsigned int const yOffset = 1;
  map<int, int> worldMap = data->getWorldData().getWorldMap();
  unsigned int size = worldMap.size();
  map<string, string> dict = data->getDictionary();

  if (toClear) {
    clear();
    toClear = false;
  }

  windows[IN_GAME] = subwin(stdscr, worldHeight + 2, worldWidth + 2, 1, 0);
  // dessin du bord de la fenetre
  box(windows[IN_GAME], ACS_VLINE, ACS_HLINE);

  // Drawing the map
  for(unsigned int index = 0; index < size; index++) {
    int x = xOffset + (index % worldWidth);
    int y = yOffset + ((index - (x - xOffset)) / worldWidth);

    if(worldMap[index] == OCEAN) {
      drawChar (windows[IN_GAME], x, y, dict["Water"].c_str()[0], WATER_PAIR);
    } else if (worldMap[index] == PLAIN) {
      drawChar (windows[IN_GAME], x, y, dict["Plain"].c_str()[0], PLAIN_PAIR);
    } else {
      drawChar (windows[IN_GAME], x, y, dict["Empty"].c_str()[0], EMPTY_PAIR);
    }
  }

  //On affiche la faune
  list<Wildlife *>::iterator it;
  list<Wildlife *> wildlifes = data->getWildlife();
  for(it = wildlifes.begin(); it != wildlifes.end(); it++) {
    char type = (*it)->getDisplayChar();
    int x = (*it)->getX()+1;
    int y = (*it)->getY()+1;

    if(type == 'F') {
      drawChar (windows[IN_GAME], x, y, type, FISH_PAIR);
    } else if(type == 'S') {
      drawChar (windows[IN_GAME], x, y, type, SHARK_PAIR);
    } else {
      drawChar (windows[IN_GAME], x, y, type, EMPTY_PAIR);
    }
  }

  refresh();
  wrefresh(windows[IN_GAME]);
  wmove(windows[ROOT], 0, 50); // repositione le curseur
  wrefresh(windows[ROOT]);
  usleep(200000);
  // fin dessin de la fenetre
}

void ScreenViews::NCurses::end () {
  mvwprintw(stdscr, 14, 20, "******************");
  mvwprintw(stdscr, 15, 20, "This is the end...");
  mvwprintw(stdscr, 16, 20, "******************");
}

int ScreenViews::NCurses::getChar () {
  return wgetch(stdscr);
}

void ScreenViews::NCurses::hello () {
  // pour tester
    initscr();
    printw("Hello World");
    refresh();
    getch();
    endwin();
}

