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

Screen Views::OpenGL::scr;
int Views::OpenGL::_maxKeyboardx;
unsigned int Views::OpenGL::_keybx;
Dictionary Views::OpenGL::dict;
vector<WINDOW*> Views::OpenGL::windows;

Views::OpenGL::OpenGL (Presenter * presenter) : View (presenter) {
  // OpenGL initialization
  // Pour un écran Widescreen ratio d'aspect = 16:9

  _pres = presenter;

  //254 * 50 charactères
  // 640 * 480 px
  // 640/254 = 2.52 pix pour largeur d'un charactere
  // 480/50 = 9.6 pour la hauteur d'un charactère
  Views::OpenGL::win = new Window(640, 480);
  shader = new Shader("text.vs", "text.fs");
  win->useShader(shader);

  Font font;
  font.load("fonts/lazy.ttf");

  win->setCharacters(font.getTable());
  win->arraysConfig();
}

Views::OpenGL::~OpenGL () {
  endwin();
}

void Views::OpenGL::output (int x, int y, const char *string) {
}

void Views::OpenGL::special (int key, int x, int y) {
  _keyboardx = scr.selected();
  scr.select(_keyboardx);
}

void Views::OpenGL::reshape (int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
}

void Views::OpenGL::selectColor (int color) {
  switch (color) {
  case 1:
    glColor3f(0.0, 1.0, 0.0);
    break;
  case 2:
    glColor3f(1.0, 0.0, 0.0);
    break;
  case 3:
    glColor3f(1.0, 1.0, 1.0);
    break;
  }
}

void Views::OpenGL::selectFont (int newfont) {
}

WINDOW * Views::OpenGL::createWindow (int height, int width) {
  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/OpenGL.log");
  log->log("create window() size: " + to_string(height) + " width: " + to_string(width));

  refresh();
  return 0;
}

void Views::OpenGL::drawChar (WINDOW * win, int x, int y, char c, char color) {
  wattron(win, COLOR_PAIR(color));
  mvwaddch(win, y, x, c);
  wattroff(win, COLOR_PAIR(color));
}

void Views::OpenGL::redraw (WINDOW * win) {
  refresh();
  wmove(windows[0], 0, 0); // repositione le curseur
  wrefresh(windows[0]);
  wrefresh(win);
  usleep(20000);
}

void Views::OpenGL::displayCursorPosition (int keybPosition) {
  mvprintw(25, 0, "POSITION: %d", keybPosition);
  refresh();
}

bool Views::OpenGL::shouldClose () {
  return win->shouldClose();
}

void Views::OpenGL::close () {
  win->close();
}

void Views::OpenGL::display (Screen screen) {
  scr = screen;

  //keyboard
  _valid = win->isValidate();
  _keyboardx = (const int)win->getYCursorPosition();

  win->setRenderFunc(&test);
  win->display();
}

void Views::OpenGL::display (Window * win, Shader * shader, Menu menu) {
  list<string>::iterator it;
  list<string> items = menu.items();
  int y = menu.y();
  unsigned int cursorPosition = 0;

  it = items.begin();
  if(_keybx < items.size())
    advance(it, _keybx);
  Dictionary d;
  //string m = "cursorPosition: " + cursorPosition;
  //m = "_maxKeyboardx: " + _maxKeyboardx;

  //TODO: le 10 c'est la largeur, donc le calcul de la plus longue chaine de caracteres
  //15 et 9 sont les tailles de font
  //box(make_pair(x, y), make_pair(x + 10, y + items.size() + 1));

  glm::vec3 color;
  win->setYCursorLimits(0, items.size()-1);
  float scale = .5f;
  for(it = items.begin(); it != items.end(); it++) {
    color = glm::vec3(0.5, 0.8f, 0.2f);
    if(cursorPosition == (const unsigned int)win->getYCursorPosition())
      color = glm::vec3(1.0, 1.0, 1.0);

    //win->renderText(*shader, dict.translate((*it)).c_str(), 175.f + menu.x(), (480 * 2) - (y * 30.0f), scale, color);
    win->renderText(*shader, dict.translate((*it)).c_str(), menu.x(), y, scale, color);
    // le 15 est la hauteur de la font => à stocker dans un tableau ou est-ce qu elle est accessible ?
    y++;
    cursorPosition++;
  }
}

void Views::OpenGL::display (Text text) {
}

void Views::OpenGL::box (pair<float, float> pointA, pair<float, float> pointB) {
  float width = 1.;

  // Compute to font size
  pair<float, float> fontSize = make_pair(9., 15.);
  pointA.first = pointA.first * fontSize.first;
  pointA.second = pointA.second * fontSize.second;
  pointB.first = pointB.first * fontSize.first;
  pointB.second = pointB.second * fontSize.second;

  glColor3f(0.0, 0.0, 0.75);
  glRectf(pointA.first, pointA.second, pointB.first, pointB.second);
  glColor3f(0.0, 0.0, 0.0);
  glRectf(pointA.first + width, pointA.second + width, pointB.first - width, pointB.second - width);
  glColor3f(1.0, 1.0, 1.0);
}

void Views::OpenGL::openglend () {
  win->renderText(*shader, "Valid", 0.f, 450.f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
} 

void Views::OpenGL::test (Window * win, Shader * shader) {

  map<int, GraphicComponent *> lst = scr.components();

  for(map<int, GraphicComponent *>::iterator it = lst.begin(); it != lst.cend(); it++) {
    if (Text * text = dynamic_cast<Text*>(it->second); text != nullptr) {
      win->renderText(*shader, text->label().c_str(), text->x(), text->y(), .5f, glm::vec3(0.5, 0.8f, 0.2f));
    } else if (Menu * menu = dynamic_cast<Menu*>(it->second); menu != nullptr) {
      display(win, shader, (*menu));
    }
  }

//  win->renderText(*shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
//  win->renderText(*shader, "(C) LearnOpenGL.com", 0.f, 450.f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
//  win->renderText(*shader, "Test", 250.f, 250.f, 0.5f, glm::vec3(0.9, 0.7f, 0.3f));
}

