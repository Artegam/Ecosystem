#include "ScreenViews.h"
#include <cstring>

using namespace ScreenViews;

#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define PLAIN_PAIR     3
#define SHARK_PAIR     4
#define FISH_PAIR      5

ScreenViews::NCurses::NCurses (Keyboard * keyb) {
  this->keyb = keyb;

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
  keyb->setPositionsCount((int)size);
  refresh();
  return mainMenu;
}

/*
void ScreenViews::NCurses::listenKeyboard (WINDOW * win, int size) {
  // Ecoute le clavier
  // TODO: A modifier et a rendre generique sur les positions / valeur de retour ?
  keyb->setWindow(win);
  keyb->listen();
  mvprintw(25, 0, "POSITION: %d", keyb->getPositionSelected());
  const unsigned int position = keyb->getPositionSelected();
  if(keyb->isValid() && position < (const unsigned int)(size-1)) {
    //TODO: surtout ici, pas sur que ce soit le bon endroit pour piloter les fenetres
    changeScreen(IN_GAME);
  } else if (keyb->isValid() && position == (const unsigned int)(size-1)) {
    changeScreen(MENU_MAIN);
  }
  keyb->resetValid();
}
*/

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

void ScreenViews::NCurses::display (WINDOW * win, list<Node *> menu) {
  list<Node *>::iterator it;
  int i = 0;
  for(it = menu.begin(); it != menu.end(); it++) {
    if(i == keyb->getPosition())
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

void ScreenViews::NCurses::mainMenu () {

  const unsigned int menuSize = 5;
  window = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
  box(window, ACS_VLINE, ACS_HLINE);
  Node * root = this->data->getMenu();
  keyb->setPositionsCount((int)menuSize);
  string title = "Ecosystem V0.1";
  int x = (this->windowWidth - title.length()) / 2;
  int y = this->windowHeight / 4;

  mvwprintw(main, y, x, "%s", title.c_str());
  refresh();

  // Affiche le menu principal
  display(window, root->getChildren());

  // Ecoute le clavier
  //keyb->listen();
  //keyb->listen(getChar());
  mvprintw(25, 0, "POSITION: %d", keyb->getPosition());
/*
  // TODO: Reflechir a ameliorer la gestion du menu, fonction anonyme ?
  const int position = keyb->getPosition();
  if(keyb->isValid()) {
    if (position == NEW) {
      changeScreen(IN_GAME);
    }
    else if (position == OPTIONS) {
      changeScreen(MENU_OPTIONS);
    }
    else if (position == SAVE) {
    }
    else if (position == LOAD) {
      changeScreen(MENU_LOAD);
    }
    else if (position == QUIT) {
      endwin();
      Clock * cl = data->getWorldData().getClock();
      cl->stop();
      cl->~Clock();
      exit(0);
    }
  }
  */
  keyb->resetValid();

  refresh();
  wmove(main, 0, 0); // repositione le curseur
  wrefresh(main);
  wrefresh(window);
  usleep(20000);
}

void ScreenViews::NCurses::options (list<string> options) {

  Node * root = this->data->getMenu();

  list<Node *> menu =  root->getChildren();

  unsigned N = 1;/* index of the element you want to retrieve */
  list<Node *>::iterator it = menu.begin();
  if (menu.size() > N)
  {
    advance(it, N);
    // 'it' points to the element at index 'N'
  }

  list<Node *> opts =  (*it)->getChildren();

  if (toClear) {
    clear();
    toClear = false;
  }

  WINDOW * optionMenu = subMenuInit(opts.size(), 15);
  display(optionMenu, opts);

  // Ecoute le clavier
  // TODO: A reecrire et a reflechir mieux que ca
  //keyb->listen();
  mvprintw(25, 0, "POSITION: %d", keyb->getPosition());
  /*
  const unsigned int position = keyb->getPosition();

  if(keyb->isValid() && position < (opts.size()-1)) {
    //loadSavedGame(keyb->getPosition());
    changeScreen(IN_GAME);
  } else if (keyb->isValid() && position == (opts.size()-1)) {
    changeScreen(MENU_MAIN);
  }
*/
  keyb->resetValid();
  refresh();
  wmove(main, 0, 0); // repositione le curseur
  wrefresh(main);
  wrefresh(optionMenu);
  usleep(20000);

}

void ScreenViews::NCurses::load (list<string> files) {
  unsigned int size = 2;
  if(files.size() > 0)
    size = files.size() + 1;
  const unsigned int menuSize = size;
  string choices[menuSize];

  WINDOW * mainMenu;
  mainMenu = subwin(stdscr, menuSize+2, 10, (this->windowHeight / 2) - 5, (this->windowWidth / 2) - 5);
  if (toClear) {
    clear();
    toClear = false;
  }
  box(mainMenu, ACS_VLINE, ACS_HLINE);
  keyb->setPositionsCount((int)menuSize);
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
    if(i == keyb->getPosition())
      wattron(mainMenu, A_REVERSE);
    mvwprintw(mainMenu, 1+i, 1, "%s", choices[i].c_str());
    wattroff(mainMenu, A_REVERSE);
  }

  // Ecoute le clavier
  //keyb->listen();
  mvprintw(25, 0, "POSITION: %d", keyb->getPosition());
  /*
  const unsigned int position = keyb->getPosition();
  if(keyb->isValid() && position < (menuSize-1)) {
    //loadSavedGame(keyb->getPosition());
    changeScreen(IN_GAME);
  } else if (keyb->isValid() && position == (menuSize-1)) {
    changeScreen(MENU_MAIN);
  }
  */
  keyb->resetValid();
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
/*
//TODO: Mettre cette fonction dans une classe appropriee au clavier (controller?)
void ScreenViews::NCurses::keyboardListener(WorldModel worldData) {
  int c = getch();

  Clock * cl = worldData.getClock();

  switch(c)
  {	case KEY_UP:
      break;
    case KEY_DOWN:
      break;
    case 115:// 's' pour save
      if(cl->isRunning())
        cl->stop();
      //data->save();
      mvprintw(24, 0, "SAVED");
      break;
    case 108:// 'l' pour load
      if(cl->isRunning())
        cl->stop();
      //data->load();
      mvprintw(24, 0, "LOADED");
      break;
    case 112: // 'p' pour la pause
      if(cl->isRunning()) {
        cl->stop();
        mvprintw(24, 0, "PAUSE");
      } else {
        cl->run();
        mvprintw(24, 0, "PLAY");
      }
      break;
    case 27: // la touche echap ou alt (voir pour mieux ecrire pour traiter le esc comme il faut)
      cl->stop();
      changeScreen(0); // 0 est le main menu
      break;
    default:
      mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
      break;
  }
  refresh();
}
*/

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

