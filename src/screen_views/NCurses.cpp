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

WINDOW * ScreenViews::NCurses::createWindow (int size, int width) {
  //TODO: 12 est le max de la plus grande chaine. Doit servir aussi Ã  calculer X pour le centrage
  WINDOW * window = subwin(stdscr, size+2, width, (this->windowHeight / 2) - 5, ((this->windowWidth - width) / 2));
  box(window, ACS_VLINE, ACS_HLINE);
  refresh();
  return window;
}

WINDOW * ScreenViews::NCurses::getWindow () {
  return windows[currentWindow];
}

void ScreenViews::NCurses::redraw (WINDOW * win) {
  refresh();
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(win);
  usleep(20000);
}

void ScreenViews::NCurses::display (WINDOW * win, list<Node *> menu, int keybPosition) {
  list<Node *>::iterator it;
  int i = 0;
  for(it = menu.begin(); it != menu.end(); it++) {
    if(i == keybPosition)
      wattron(win, A_REVERSE);
    string prefix = "";
    if (Item* item = dynamic_cast<Item*>(*it)) {
      if(item->isSelected()) {
        prefix = "[x] ";
      } else {
        prefix = "[ ] ";
      }
    }
    mvwprintw(win, 1+i, 1, "%s%s", prefix.c_str(), (*it)->getName().c_str());
    wattroff(win, A_REVERSE);
    i++;
  }
}

void ScreenViews::NCurses::mainMenu (int keybPosition) {

  const unsigned int menuSize = 5;
  windows[MENU_MAIN] = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
  box(windows[MENU_MAIN], ACS_VLINE, ACS_HLINE);
  Node * root = this->data->getMenu();
  string title = "Ecosystem V0.1";
  int x = (this->windowWidth - title.length()) / 2;
  int y = this->windowHeight / 4;

  mvwprintw(windows[ROOT], y, x, "%s", title.c_str());
  refresh();

  // Affiche le menu principal
  std::thread t_m(&NCurses::display, windows[MENU_MAIN], root->getChildren(), keybPosition);
  t_m.detach();

  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[MENU_MAIN]);
  usleep(20000);
}

void ScreenViews::NCurses::options (list<string> options, int keybPosition) {

  if (toClear) {
    clear();
    toClear = false;
  }

  Node * root = this->data->getMenu();
  list<Node *> menu =  root->getChildren();
  list<Node *>::iterator it = menu.begin();
	advance(it, MENU_OPTIONS - 1);

  list<Node *> opts =  (*it)->getChildren();
  windows[MENU_OPTIONS] = createWindow((int)opts.size(), 15);
  display(windows[MENU_OPTIONS], opts, keybPosition);

  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[MENU_OPTIONS]);
  usleep(20000);
}

void ScreenViews::NCurses::validateOption (int optionNumber) {
  Node * root = this->data->getMenu();
  list<Node *> menu =  root->getChildren();
  list<Node *>::iterator it = menu.begin();
	advance(it, MENU_OPTIONS - 1);

  list<Node *> opts =  (*it)->getChildren();
  list<Node *>::iterator it2 = opts.begin();
  advance(it2, optionNumber);
	clearOptions(opts);
  (*it2)->validate();
}

void ScreenViews::NCurses::save (list<string> files, const unsigned int menuSize, int keybPosition) {
  string choices[menuSize];

  windows[MENU_SAVE] = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
  box(windows[MENU_SAVE], ACS_VLINE, ACS_HLINE);
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
      wattron(windows[MENU_SAVE], A_REVERSE);
    mvwprintw(windows[MENU_SAVE], 1+i, 1, "%s", choices[i].c_str());
    wattroff(windows[MENU_SAVE], A_REVERSE);
  }

  // Ecoute le clavier
  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[MENU_SAVE]);
  usleep(20000);
}

void ScreenViews::NCurses::load (list<string> files, const unsigned int menuSize, int keybPosition) {
  string choices[menuSize];

  windows[MENU_LOAD] = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
  box(windows[MENU_LOAD], ACS_VLINE, ACS_HLINE);
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
      wattron(windows[MENU_LOAD], A_REVERSE);
    mvwprintw(windows[MENU_LOAD], 1+i, 1, "%s", choices[i].c_str());
    wattroff(windows[MENU_LOAD], A_REVERSE);
  }

  // Ecoute le clavier
  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
  wmove(windows[ROOT], 0, 0); // repositione le curseur
  wrefresh(windows[ROOT]);
  wrefresh(windows[MENU_LOAD]);
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

  if (toClear) {
    clear();
    toClear = false;
  }

  windows[IN_GAME] = subwin(stdscr, worldHeight + 2, worldWidth + 2, 1, 0);
  // dessin du bord de la fenetre
  box(windows[IN_GAME], ACS_VLINE, ACS_HLINE);

	for(unsigned int index = 0; index < size; index++) {
    int x = xOffset + (index % worldWidth);
    int y = yOffset + ((index - (x - xOffset)) / worldWidth);

		if(worldMap[index] == OCEAN) {
      //TODO: A reecrire...
			wattron(windows[IN_GAME], COLOR_PAIR(WATER_PAIR));
			mvwaddch(windows[IN_GAME], y, x, '~');
			wattroff(windows[IN_GAME], COLOR_PAIR(WATER_PAIR));
		} else if (worldMap[index] == PLAIN) {
			wattron(windows[IN_GAME], COLOR_PAIR(PLAIN_PAIR));
			mvwaddch(windows[IN_GAME], y, x, 'o');
			wattroff(windows[IN_GAME], COLOR_PAIR(PLAIN_PAIR));
    } else {
			wattron(windows[IN_GAME], COLOR_PAIR(EMPTY_PAIR));
			mvwaddch(windows[IN_GAME], y, x, '.');
			wattroff(windows[IN_GAME], COLOR_PAIR(EMPTY_PAIR));
    }
	}

/*
  for(int y = 1; y <= data->getWorldHeight(); y++) {
    for(int x = 1; x <= data->getWorldWidth(); x++) {
      wattron(window, COLOR_PAIR(WATER_PAIR));
      mvwaddch(window, y, x, '~');
      wattroff(window, COLOR_PAIR(WATER_PAIR));
    }
  }
*/

  //On affiche la faune
  list<Wildlife *>::iterator it;
  list<Wildlife *> wildlifes = data->getWildlife();
  for(it = wildlifes.begin(); it != wildlifes.end(); it++) {
    char type = (*it)->getDisplayChar();

    if(type == 'F') {
      wattron(windows[IN_GAME], COLOR_PAIR(FISH_PAIR));
    } else if(type == 'S') {
      wattron(windows[IN_GAME], COLOR_PAIR(SHARK_PAIR));
    } else {
      wattron(windows[IN_GAME], COLOR_PAIR(EMPTY_PAIR));
    }

    mvwaddch(windows[IN_GAME], (*it)->getY()+1, (*it)->getX()+1, (*it)->getDisplayChar());

    if(type == 'F') {
      wattroff(windows[IN_GAME], COLOR_PAIR(FISH_PAIR));
    } else if(type == 'S') {
      wattroff(windows[IN_GAME], COLOR_PAIR(SHARK_PAIR));
    } else {
      wattroff(windows[IN_GAME], COLOR_PAIR(EMPTY_PAIR));
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

