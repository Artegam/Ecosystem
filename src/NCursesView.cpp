#include "ConsoleView.h"

using namespace ConsoleView;

#define EMPTY_PAIR     1
#define WATER_PAIR     2


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
  mvwprintw(main, 2, 50, "Nb etres vivants: %u", data->getWildlifeCount());
  if(data->isRunning()) {
    mvwprintw(main, 3, 50, "execution: en cours...");
  } else {
    mvwprintw(main, 3, 50, "execution: stopee");
  }
  mvwprintw(main, 4, 50, "tours: %u", data->getTurns());
  mvwprintw(main, 5, 50, "moyenne age: %u", data->getAverageAge());
  mvwprintw(main, 6, 50, "esperance de vie: %u", data->getLifeExpectancy());
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

  window= subwin(stdscr, data->getWorldHeight()+2, data->getWorldWidth()+2, 1, 0);

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
    attron(COLOR_PAIR(EMPTY_PAIR));
    mvwprintw(window, (*it)->getY(), (*it)->getX(), "%c", (*it)->getDisplayChar());
    attroff(COLOR_PAIR(EMPTY_PAIR));
  }
  refresh();
  wrefresh(main);
  wrefresh(window);
  usleep(500000);
  // fin dessin de la fentre

  //getch();

}

