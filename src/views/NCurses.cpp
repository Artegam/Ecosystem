#include "Views.h"

using namespace Views;

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
  getmaxyx(stdscr, screenSize.height, screenSize.width);
}

Views::NCurses::~NCurses () {
  endwin();
}

void Views::NCurses::init (int height, int width) {
  this->worldHeight = height;
  this->worldWidth = width;

  // Creation of the windows
  windows[0] = subwin(stdscr, screenSize.height, screenSize.width, 0, 0);
  log->log("Current console size : width: " + to_string(screenSize.width) + " height: " + to_string(screenSize.height));

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
  log->log("create window() x: " + to_string(x) + " y: " + to_string(y) + " height: " + to_string(height) + " width: " + to_string(width));

  if(windows.size() < (unsigned long)(screen + 1)) 
    windows.resize(screen+1);
  windows[screen] = subwin(stdscr, height, width, y, x);

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


void Views::NCurses::displayCursorPosition (int keybPosition) {
  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
}

void Views::NCurses::logCursorPosition (int keybPosition) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/NCurses.log");
  log->log("Cursor position: " + to_string(keybPosition));
}

void Views::NCurses::validateOption (int optionNumber) {
  scr.select(optionNumber);
}


void Views::NCurses::clearScreen() {
  toClear = true;
}

void Views::NCurses::initScreen() {
  if (toClear) {
    wclear(stdscr);
    wclear(windows[scr.window()]);
    toClear = false;
  }
}

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

  wclear(windows[scr.window()]);
  keyboard();
  box(windows[scr.window()], ACS_VLINE, ACS_HLINE);

  lst = scr.components();

  mvwprintw(stdscr, 1, 1, "_keyboardx: %d", _keyboardx);

  for(map<int, GraphicComponent *>::iterator it = lst.begin(); it != lst.cend(); it++) {
    if (Selector * sel = dynamic_cast<Selector*>(it->second); sel != nullptr) {
      display((*sel));
    } else if (Text * text = dynamic_cast<Text*>(it->second); text != nullptr) {
      display((*text));
    } else if (Menu * menu = dynamic_cast<Menu*>(it->second); menu != nullptr) {
      display((*menu));
    } else if (Calendar * cal = dynamic_cast<Calendar*>(it->second); cal != nullptr) {
      display((*cal));
    } else if (Agenda * age = dynamic_cast<Agenda*>(it->second); age != nullptr) {
      display((*age));
    } else if (Table * tab = dynamic_cast<Table*>(it->second); tab != nullptr) {
      display((*tab));
    } else if (Cell * cell = dynamic_cast<Cell*>(it->second); cell != nullptr) {
      display((*cell));
    }
  }

  bubbles();
  wrefresh(windows[scr.window()]);
  usleep(100000);
}

void Views::NCurses::display (Screen screen) {
  scr = screen;
  View::display(scr);
}

void Views::NCurses::display (Menu menu) {
  list<string>::iterator it;
  list<string> items = menu.items();
  unsigned int y = (menu.y() * screenSize.height) / 100;
  unsigned int x = (menu.x() * screenSize.width) / 100;

  unsigned int cursorPosition = 0;

  it = items.begin();
  advance(it, _keyboardx);
  mvwprintw(stdscr, 2, 1, "label: %s", dict.translate((*it)).c_str());
  mvwprintw(stdscr, 5, 1, "_maxKeyboardx: %d", _maxKeyboardx);

  //TODO: le 10 c'est la largeur, donc le calcul de la plus longue chaine de caracteres
  WINDOW * sub = subwin(windows[menu.window()], items.size() + 2, 10,  y - 1, x - 1);
  box(sub, ACS_VLINE, ACS_HLINE);

  for(it = items.begin(); it != items.end(); it++) {
    if(cursorPosition == _keyboardx)
      wattron(windows[menu.window()], A_REVERSE);

    mvwprintw(windows[menu.window()], y, x, "%s", dict.translate((*it)).c_str());

    if(cursorPosition == _keyboardx)
      wattroff(windows[menu.window()], A_REVERSE);

    y++;
    cursorPosition++;
  }
}

void Views::NCurses::display (Text text) {
  unsigned int y = (text.y() * screenSize.height) / 100;
  unsigned int x = (text.x() * screenSize.width) / 100;
  mvwprintw(windows[text.window()], y, x, "%s", dict.translate(text.label()).c_str());
}

void Views::NCurses::display (Selector selector) {
  WINDOW * win = windows[selector.window()];
  unsigned int y = (selector.y() * screenSize.height) / 100;
  unsigned int x = (selector.x() * screenSize.width) / 100;

  mvwprintw(win, y, x, "< %s >", dict.translate(selector.label()).c_str());
}

void Views::NCurses::display (Table table) {
  basic b = table.getBasic();
  tab t = table.getTab();
  wmove(stdscr, 0, 0);

  unsigned int x = (b.x * screenSize.width) / 100;
  unsigned int y = (b.y * screenSize.height) / 100;
  const unsigned int width = b.width;
  const unsigned int cols = t.cols;
  unsigned int maxrowsize = 1;

  if (width > 1) {
    unsigned int cpt = 0;
    list<unsigned int>::iterator rowit;
    list<unsigned int>::iterator colit;

    for(rowit = t.rowssizes.begin(); rowit != t.rowssizes.end(); rowit++) {
      x = (b.x * screenSize.width) / 100;
      if(cpt>0)
        y = y+maxrowsize+1;
      if(cpt==0)
        tablerow (table.getRow(0), t.colssizes, x, y, cols);
      else
        tablerow (table.getRow(cpt), t.colssizes, x, y, cols, nullptr, 1);
      cpt++;
    }
  }
}


void Views::NCurses::display (Agenda age) {
  Text t = age.getName();
  display(t);
  Table tab = age.getHourly();
  display(tab);
}

void Views::NCurses::display (Calendar calendar) {
  Selector s = calendar.getMonth();
  display(s);
  Selector y = calendar.getYear();
  display(y);
  Table t = calendar.getDaily();
  display(t);
}

//Mode : type of row
// 0 - A simple row
// 1 - Last row of a table
void Views::NCurses::tablerow (list<Cell*> lst, list<unsigned int> colssizes, unsigned int x, unsigned int y, const unsigned int cols, Cell * cursor, unsigned int mode) {
  WINDOW * win = stdscr; //ATTENTION ICI !!! On ne respecte pas la fenetre selectionnee dans les donnees
  unsigned int maxcolsize = 0;
  unsigned int maxrowsize = 1;
  unsigned int cpt = 0;
  list<unsigned int>::iterator rowit;
  list<unsigned int>::iterator colit;
  unsigned int col;

  list<Cell*>::iterator it = lst.begin();

  //begin header
  wmove(win, y, x);
  if(mode==0)
    hline(ACS_ULCORNER, 1);
  else
    hline(ACS_LTEE, 1);
  // begin body
  wmove(win, y+maxrowsize, x);
  hline(ACS_VLINE, 1);
  // begin foot
  wmove(win, y+maxrowsize+1, x);
  hline(ACS_LLCORNER, 1);

  for(colit = colssizes.begin(); colit != colssizes.end(); colit++) {
    maxcolsize = *colit;
    col = cpt%cols;

    // length of header and junction tee
    wmove(win, y, x+col+1);
    hline(ACS_HLINE, maxcolsize);
    wmove(win, y, x+maxcolsize+col+1);
    if(mode==0)
      hline(ACS_TTEE, 1);
    else
      hline(ACS_PLUS, 1);

    // body
    wmove(win, y+maxrowsize, x+maxcolsize+col+1);
    hline(ACS_VLINE, 1);
    // Value
    Cell c = **it;

    if(c.isSelected())
      wattron(stdscr, COLOR_PAIR(WATER_PAIR));

    mvwprintw(win, y+maxrowsize, x+col+1, "%s", dict.translate(c.value()).c_str());
    if(c.isSelected())
      wattroff(stdscr, COLOR_PAIR(WATER_PAIR));

    // length of foot and junction tee
    wmove(win, y+maxrowsize+1, x+col+1);
    hline(ACS_HLINE, maxcolsize);
    wmove(win, y+maxrowsize+1, x+maxcolsize+col+1);
    hline(ACS_BTEE, 1);

    cpt++;
    it++;
    if(cpt<cols)
      x+=maxcolsize;
  }
  //End of header
  wmove(win, y, x+maxcolsize+col+1);
  if(mode==0)
    hline(ACS_URCORNER, 1);
  else if(mode == 1)
    hline(ACS_RTEE, 1);
  // End of foot
  wmove(win, y+maxrowsize+1, x+maxcolsize+col+1);
  hline(ACS_LRCORNER, 1);
}

void Views::NCurses::display (Cell cell) {
  //TODO: peut-etre deplacer ce calcul ?
  //unsigned int y = (cell.y() * screenSize.height) / 100;
  //unsigned int x = (cell.x() * screenSize.width) / 100;
  basic b = cell.getBasic();
  rect(b);

  //print value
  mvwprintw(stdscr, b.y+1, b.x+1, "%s", dict.translate(cell.value()).c_str());

  wmove(stdscr, 0, 0);
}

void Views::NCurses::rect(basic b) {
  WINDOW * win = stdscr;
  const unsigned int x = b.x;
  const unsigned int y = b.y;
  const unsigned int width = b.width;
  const unsigned int height = b.height;


  wmove(win, y, x);
  hline(ACS_ULCORNER, 1);
  wmove(win, y, x+1);
  hline(ACS_HLINE, width);
  wmove(win, y, x+width+1);
  hline(ACS_URCORNER, 1);
  wmove(win, y+1, x);
  vline(ACS_VLINE, height);
  wmove(win, y+height+1, x);
  hline(ACS_LLCORNER, 1);
  wmove(win, y+height+1, x+1);
  hline(ACS_HLINE, width);
  wmove(win, y+height+1, x+width+1);
  hline(ACS_LRCORNER, 1);
  wmove(win, y+1, x+width+1);
  vline(ACS_VLINE, height);
}

/*
void Views::NCurses::tab(Table tab) {


  //hline(ACS_TTEE, 1);

}
*/
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
      if(_keyboardx < _maxKeyboardx)
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

bubble_data Views::NCurses::bubble (bubble_data data) {
  char lst[4] = {0x2E, 0x6F, 0x4F, 0x20}; // '.', 'o', 'O', ' '

  wattron(stdscr, COLOR_PAIR(WATER_PAIR));
  mvwprintw(stdscr, data.y, data.x, "%c", lst[data.index]);

  if (rand()%2) { //sign
    data.x += rand()%2;
  } else {
    data.x -= rand()%2;
  }

  if (data.index < 3) {
    data.index++;
  } else if (data.y > 0) {
    data.y--;
    data.index = 2;
    mvwprintw(stdscr, data.y, data.x, "%c", lst[data.index]);
  } else {
    data.y = rand()%25 + 25;
    data.x = rand()%254;
    data.index = 0;
  }
  wattroff(stdscr, COLOR_PAIR(WATER_PAIR));
  return data;
}


void Views::NCurses::bubbles () {
  list<bubble_data>::iterator it;
  for (it = bub_data.begin(); it != bub_data.end(); it++) {
    *it = bubble(*it);
  }

  //Add random number of bubbles inferior to max
  unsigned int nb = rand() % (bub_max - bub_data.size());

  for (unsigned int i = 0; i < nb; i++) {
    bubble_data oneBubble;
    bub_data.push_back(oneBubble);
  }
}

