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
  main = subwin(stdscr, LINES, COLS, 0, 0);
  window= subwin(stdscr, worldHeight + 2, worldWidth + 2, 1, 0);
  box(window, ACS_VLINE, ACS_HLINE);

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

WINDOW * ScreenViews::NCurses::subMenuInit (int size, int width) {
  WINDOW * mainMenu;
  //TODO: 12 est le max de la plus grande chaine. Doit servir aussi Ã  calculer X pour le centrage
  mainMenu = subwin(stdscr, size+2, width, (this->windowHeight / 2) - 5, ((this->windowWidth - width) / 2));
  box(mainMenu, ACS_VLINE, ACS_HLINE);
  refresh();
  return mainMenu;
}

WINDOW * ScreenViews::NCurses::getWindow () {
  return window;
}

void ScreenViews::NCurses::redraw (WINDOW * win) {
  refresh();
  wmove(main, 0, 0); // repositione le curseur
  wrefresh(main);
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
  window = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
  box(window, ACS_VLINE, ACS_HLINE);
  Node * root = this->data->getMenu();
  string title = "Ecosystem V0.1";
  int x = (this->windowWidth - title.length()) / 2;
  int y = this->windowHeight / 4;

  mvwprintw(main, y, x, "%s", title.c_str());
  refresh();

  // Affiche le menu principal
  std::thread t_m(&NCurses::display, window, root->getChildren(), keybPosition);
  t_m.detach();

  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
  wmove(main, 0, 0); // repositione le curseur
  wrefresh(main);
  wrefresh(window);
  usleep(20000);
}

void ScreenViews::NCurses::options (list<string> options, int keybPosition) {

  Node * root = this->data->getMenu();

  list<Node *> menu =  root->getChildren();
//TODO: tres mal ecrit ici, a refaire.
  unsigned N = 1;/* index of the element you want to retrieve */
  list<Node *>::iterator it = menu.begin();
  if (menu.size() > N)
  {
    advance(it, N);
    // 'it' points to the element at index 'N'
  }

  if (toClear) {
    clear();
    toClear = false;
  }

  list<Node *> opts =  (*it)->getChildren();
  WINDOW * optionMenu = subMenuInit((int)opts.size(), 15);
  display(optionMenu, opts, keybPosition);

  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
  wmove(main, 0, 0); // repositione le curseur
  wrefresh(main);
  wrefresh(optionMenu);
  usleep(20000);

}

void ScreenViews::NCurses::validateOption (int optionNumber) {
  Node * root = this->data->getMenu();
  list<Node *> menu =  root->getChildren();

  // 1 is option menu
  unsigned N = 1;/* index of the element you want to retrieve */
  list<Node *>::iterator it = menu.begin();
  if (menu.size() > N)
  {
    advance(it, N);
    // 'it' points to the element at index 'N'
  }

  list<Node *> opts =  (*it)->getChildren();
  list<Node *>::iterator it2 = opts.begin();
  advance(it2, optionNumber);
	clearOptions(opts);
  (*it2)->validate();

}

void ScreenViews::NCurses::load (list<string> files, const unsigned int menuSize, int keybPosition) {
  string choices[menuSize];

  WINDOW * mainMenu;
  mainMenu = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
  box(mainMenu, ACS_VLINE, ACS_HLINE);
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
      wattron(mainMenu, A_REVERSE);
    mvwprintw(mainMenu, 1+i, 1, "%s", choices[i].c_str());
    wattroff(mainMenu, A_REVERSE);
  }

  // Ecoute le clavier
  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
  wmove(main, 0, 0); // repositione le curseur
  wrefresh(main);
  wrefresh(mainMenu);
  usleep(20000);
}

void ScreenViews::NCurses::infos (list<string> infos) {
  if (toClear) {
    clear();
    toClear = false;
  }
  wprintw(main, "This is the virtual world");
  int ligne = 1;
  list<string>::iterator info;
  for(info = infos.begin(); info != infos.end(); info++) {
    mvwprintw(main, ligne, 50, "%s", info->c_str());
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

  window= subwin(stdscr, worldHeight + 2, worldWidth + 2, 1, 0);
  // dessin du bord de la fenetre
  box(window, ACS_VLINE, ACS_HLINE);

	for(unsigned int index = 0; index < size; index++) {
    int x = xOffset + (index % worldWidth);
    int y = yOffset + ((index - (x - xOffset)) / worldWidth);

		if(worldMap[index] == OCEAN) {
      //TODO: A reecrire...
			wattron(window, COLOR_PAIR(WATER_PAIR));
			mvwaddch(window, y, x, '~');
			wattroff(window, COLOR_PAIR(WATER_PAIR));
		} else if (worldMap[index] == PLAIN) {
			wattron(window, COLOR_PAIR(PLAIN_PAIR));
			mvwaddch(window, y, x, 'o');
			wattroff(window, COLOR_PAIR(PLAIN_PAIR));
    } else {
			wattron(window, COLOR_PAIR(EMPTY_PAIR));
			mvwaddch(window, y, x, '.');
			wattroff(window, COLOR_PAIR(EMPTY_PAIR));
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
      wattron(window, COLOR_PAIR(FISH_PAIR));
    } else if(type == 'S') {
      wattron(window, COLOR_PAIR(SHARK_PAIR));
    } else {
      wattron(window, COLOR_PAIR(EMPTY_PAIR));
    }

    mvwaddch(window, (*it)->getY()+1, (*it)->getX()+1, (*it)->getDisplayChar());

    if(type == 'F') {
      wattroff(window, COLOR_PAIR(FISH_PAIR));
    } else if(type == 'S') {
      wattroff(window, COLOR_PAIR(SHARK_PAIR));
    } else {
      wattroff(window, COLOR_PAIR(EMPTY_PAIR));
    }
  }

  refresh();
  wrefresh(window);
  wmove(main, 0, 50); // repositione le curseur
  wrefresh(main);
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

