#include "Views.h"
#include "Logs.h"
#include "Translations.h"

#include <cstring>
#include <GL/gl.h>
#include <GL/freeglut.h>

using namespace Views;
using namespace Logs;
using namespace Translations;

#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define PLAIN_PAIR     3
#define SHARK_PAIR     4
#define FISH_PAIR      5

static void *font;
static void *fonts[] =
{
  GLUT_BITMAP_9_BY_15,
  GLUT_BITMAP_TIMES_ROMAN_10,
  GLUT_BITMAP_TIMES_ROMAN_24
};

Screen Views::OpenGL::scr;
int Views::OpenGL::_keyboardx;
int Views::OpenGL::_maxKeyboardx;
Presenter Views::OpenGL::_presenter;
Dictionary Views::OpenGL::dict;
vector<WINDOW*> Views::OpenGL::windows;

Views::OpenGL::OpenGL (Presenter * presenter) : View (presenter) {
  // OpenGL initialization
  // Pour un écran Widescreen ratio d'aspect = 16:9
  long width = 800;
  long height = 450;
  float zNear = 0.1;
  float zFar = 50.0;
  float zoomFactor = 1.0;

  int i, msg_submenu, color_submenu;

  int argc = 1;
  char * argv[1] = {(char*)"Ecosystem vX.Xyyyy - OpenGL"};
  glutInit(&argc, argv);
  
  font = GLUT_BITMAP_9_BY_15;
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1920, 1080); // Affichage de mon pc de developpement
  int window = glutCreateWindow("Ecosystem");
  glutPositionWindow(0, 0);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutDisplayFunc(displayRoutine);
  glutReshapeFunc(reshape);
  glutIdleFunc(tick);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  msg_submenu = glutCreateMenu(selectMessage);
  glutAddMenuEntry("abc", 1);
  glutAddMenuEntry("ABC", 2);
  color_submenu = glutCreateMenu(selectColor);
  glutAddMenuEntry("Green", 1);
  glutAddMenuEntry("Red", 2);
  glutAddMenuEntry("White", 3);
  glutCreateMenu(selectFont);
  glutAddMenuEntry("9 by 15", 0);
  glutAddMenuEntry("Times Roman 10", 1);
  glutAddMenuEntry("Times Roman 24", 2);
  glutAddSubMenu("Messages", msg_submenu);
  glutAddSubMenu("Color", color_submenu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);


/*
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowPosition(100, 100);
  glutInitWindowSize(width, height);
  glutCreateWindow("Ecosystem vX.X - OpenGL");
*/
// FIN DE LA FENETRE

/*
    GLDrawable glDrawable = GLContext.getCurrent().getGLDrawable();
    GL gl = GLContext.getCurrent().getGL();
   
    gl.glMatrixMode(GL.GL_PROJECTION);   //select the Projection matrix
    gl.glPushMatrix();                   //save the current projection matrix
    gl.glLoadIdentity();                 //reset the current projection matrix to creates a new Orthographic projection
    //Creates a new orthographic viewing volume
    gl.glOrtho(0, glDrawable.getWidth(), 0, glDrawable.getHeight(), NEAR, FAR);
   
    gl.glMatrixMode(GL.GL_MODELVIEW);
    gl.glPushMatrix();
    gl.glLoadIdentity();
*/

  //Initialisation des matrices
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective (50.0*zoomFactor, (float)width/(float)height, zNear, zFar);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);


// TEXTE ICI
  glRasterPos2i(1, 2);
  glColor3f(1.0f, 1.0f, 1.0f);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Coucou");

}

Views::OpenGL::~OpenGL () {
  endwin();
}

void Views::OpenGL::init (int height, int width) {
/*
  initLight();
  gluLookAt(0.0, 6.0, 6.0, 0.0, 0.0, 0.0, 0.0 , 1.0, 0.0);

  //Initialisation des objets (chargement dans la carte graphique pour le rendu
  for(it = objets.begin(); it != objets.end(); it++) {
    it->init();
  }

  glutDisplayFunc(render);
  glutIdleFunc(idle);
  //glutReshapeFunc(Reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);

  glutMainLoop();
*/
}

void Views::OpenGL::output (int x, int y, const char *string) {
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

void Views::OpenGL::displayRoutine (void) {
  glClear(GL_COLOR_BUFFER_BIT);
  //output(0, 24, (char *)"This is written in a GLUT bitmap font.");
  //output(100, 100, (char *)"GLUT means OpenGL.");
  //output(50, 145, (char *)"(positioned in pixels with upper-left origin)");

//******************************************************************************************
  //wclear(windows[screen.window()]);
  //box(windows[screen.window()], ACS_VLINE, ACS_HLINE);

  map<int, GraphicComponent *> components = scr.components();
  _keyboardx = scr.selected();

  output(0, 24, ("selectedIndex: " + to_string(_keyboardx)).c_str());
  for(long unsigned int i = 0; i < components.size(); i++) {

    GraphicComponent * a = components[i];

    if (Text * text = dynamic_cast<Text*>(a); text != nullptr) {
      disp(*text);
    } else if (Menu * menu = dynamic_cast<Menu*>(a); menu != nullptr) {
      disp(*menu);
    }
  }

  usleep(100000);
//******************************************************************************************

  glutSwapBuffers();
}

void Views::OpenGL::keyboard (unsigned char key, int x, int y) {
}

void Views::OpenGL::special (int key, int x, int y) {
  _keyboardx = scr.selected();
  if(key == GLUT_KEY_UP) {
    if(_keyboardx > 0)
      _keyboardx--;
  } else if (key == GLUT_KEY_DOWN) {
    if(_keyboardx < _maxKeyboardx)
      _keyboardx++;
  }
  scr.select(_keyboardx);
}

void Views::OpenGL::reshape (int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, height, 0);
  glMatrixMode(GL_MODELVIEW);
}

void Views::OpenGL::tick(void) {
  _presenter.display();
  glutPostRedisplay();
}

void Views::OpenGL::selectMessage (int msg) {
  char * message;

  switch (msg) {
  case 1:
    message = (char *)"abcdefghijklmnop";
    break;
  case 2:
    message = (char *)"ABCDEFGHIJKLMNOP";
    break;
  }
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
  glutPostRedisplay();
}

void Views::OpenGL::selectFont (int newfont) {
  font = fonts[newfont];
  glutPostRedisplay();
}

WINDOW * Views::OpenGL::createWindow (int height, int width) {
  const int edges = 2;
  const int yOffset = (this->windowHeight / 2) - 5;
  const int xOffset = ((this->windowWidth - width) / 2);

  Logger * log = new Logger("/home/tonio/labo/Ecosystem/bin/OpenGL.log");
  log->log("create window() size: " + to_string(height) + " width: " + to_string(width));

  //WINDOW * window = subwin(stdscr, height + edges, width, yOffset, xOffset);
  //box(window, ACS_VLINE, ACS_HLINE);
  refresh();
  //return window;
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

void Views::OpenGL::display (Screen screen) {
  Views::OpenGL::scr = screen;
  Views::OpenGL::_maxKeyboardx = screen.getSelectSize() - 1;
  glutMainLoop();
}

void Views::OpenGL::disp (Menu menu) {
  list<string>::iterator it;
  list<string> items = menu.items();
  int y = menu.y();
  const int x = menu.x(); 
  int cursorPosition = 0;

  it = items.begin();
  if(_keyboardx < items.size())
    advance(it, _keyboardx);
  Dictionary d;
  output(0, 15, (char *)("label: " + d.translate(*it)).c_str());
  string m = "cursorPosition: " + cursorPosition;
  output(0, 45, m.c_str());
  m = "_maxKeyboardx: " + _maxKeyboardx;
  output(0, 60, m.c_str());

  //mvwprintw(stdscr, 2, 1, "label: %s", dict.translate((*it)).c_str());
  //mvwprintw(stdscr, 5, 1, "_maxKeyboardx: %d", _maxKeyboardx);

  //TODO: le 10 c'est la largeur, donc le calcul de la plus longue chaine de caracteres
  //WINDOW * sub = subwin(windows[menu.window()], items.size() + 2, 10,  y - 1, x - 1);
  //box(sub, ACS_VLINE, ACS_HLINE);

  pair<int, int> fontSize = make_pair(9, 15);
  //15 et 9 sont les tailles de font
  box(make_pair(x, y), make_pair(x + 10, y + items.size() + 1));

  for(it = items.begin(); it != items.end(); it++) {
    if(cursorPosition == _keyboardx)
      glColor3f(0.0, 1.0, 0.0);
// le 15 est la hauteur de la font => à stocker dans un tableau ou est-ce qu elle est accessible ?
    output((x + 1) * fontSize.first, (y + 1) * fontSize.second, d.translate(*it).c_str());

    if(cursorPosition == _keyboardx)
      glColor3f(1.0, 1.0, 1.0);

    y++;
    cursorPosition++;
  }

}

void Views::OpenGL::disp (Text text) {
  // Compute to font size
  pair<int, int> fontSize = make_pair(9, 15);
 
  int x = text.x() * fontSize.first; // lie a la font
  int y = text.y() * fontSize.second;

  output(0, 900, ("x: " + to_string(x)).c_str());
  output(0, 915, ("y: " + to_string(y)).c_str());

  output(x, y, dict.translate(text.label()).c_str());
}

/*
list<string> Views::OpenGL::nodesToString (list<Node *> items) {
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
      //lst.push_back(prefix + (*it)->getName());
    }
  }
  return lst;
}
*/

void Views::OpenGL::box (pair<float, float> pointA, pair<float, float> pointB) {
  float width = 1.;
/*
  output(0, 900, ("x: " + to_string(pointA.first)).c_str());
  output(0, 915, ("y: " + to_string(pointA.second)).c_str());
*/

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
