#include "ConsoleView.h"
#include <cstring>

using namespace ConsoleView;

#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define SHARK_PAIR     3
#define FISH_PAIR      4

NCursesView::NCursesView () {
}

NCursesView::~NCursesView () {
  endwin();
}

void NCursesView::init () {
  initscr();

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(EMPTY_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(WATER_PAIR, COLOR_BLUE, COLOR_BLACK);
	init_pair(SHARK_PAIR, COLOR_RED, COLOR_BLACK);
	init_pair(FISH_PAIR, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(EMPTY_PAIR));
  clear();
}

void NCursesView::menu (WINDOW * main) {
  wprintw(main, "This is the virtual world");
  if(data->getTic()) {
    mvwprintw(main, 1, 50, "tic");
  } else {
    mvwprintw(main, 1, 50, "tac");
  }
  mvwprintw(main, 2, 50, "Living beings number: %u", data->getWildlifeCount());
  if(data->isRunning()) {
    mvwprintw(main, 3, 50, "execution: running...");
  } else {
    mvwprintw(main, 3, 50, "execution: stopped");
  }
  mvwprintw(main, 4, 50, "turns: %u", data->getTurns());
  mvwprintw(main, 5, 50, "age average: %u", data->getAverageAge());
  mvwprintw(main, 6, 50, "life expectancy: %u", data->getLifeExpectancy());
}

void NCursesView::display (ScreenViewModel * data) {
  this->data = data;

  WINDOW *main;
  WINDOW *window;

  this->init();
  /*
  // demande la taille de l'ecran
  char* qHaut = new char[20];
  sprintf(qHaut, "Hauteur ? (max: %d)", LINES);
  printw(qHaut);
  getstr(hauteur);

  printw("la hauteur est : ");
  printw(hauteur);
  getch();
*/
  // Fin de demande de la taille de l'ecran

  // dessin de la fenetre
  main = subwin(stdscr, LINES, COLS, 0, 0);
  this->menu(main);

  window= subwin(stdscr, data->getWorldHeight() + 2, data->getWorldWidth() + 2, 1, 0);

  box(window, ACS_VLINE, ACS_HLINE);

  for(int y = 1; y <= data->getWorldHeight(); y++) {
    for(int x = 1; x <= data->getWorldWidth(); x++) {
      wattron(window, COLOR_PAIR(WATER_PAIR));
      mvwaddch(window, y, x, '~');
      wattroff(window, COLOR_PAIR(WATER_PAIR));
    }
  }

  //On affiche la faune
  list<Wildlife *> lst = data->getWildlife();
  list<Wildlife *>::iterator it;

  for(it = lst.begin(); it != lst.end(); it++) {
    char type = (*it)->getDisplayChar();

    if(type == 'F') {
      wattron(window, COLOR_PAIR(FISH_PAIR));
    } else if(type == 'S') {
      wattron(window, COLOR_PAIR(SHARK_PAIR));
    } else {
      wattron(window, COLOR_PAIR(EMPTY_PAIR));
    }

    mvwaddch(window, (*it)->getY(), (*it)->getX(), (*it)->getDisplayChar());

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
  usleep(500000);
  // fin dessin de la fenetre
}
