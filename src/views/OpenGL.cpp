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
bool Views::OpenGL::_valid;
Presenter Views::OpenGL::_pres;
Dictionary Views::OpenGL::dict;
vector<WINDOW*> Views::OpenGL::windows;

Views::OpenGL::OpenGL (Presenter * presenter) : View (presenter) {
  // OpenGL initialization
  // Pour un écran Widescreen ratio d'aspect = 16:9

  Views::OpenGL::_pres = *presenter;
// TEXTE ICI
}

Views::OpenGL::~OpenGL () {
  endwin();
}

void Views::OpenGL::init () {
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
    cout << "ERROR: Initializing SDL lib" << SDL_GetError() << endl;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


  //SDL_Window * win = SDL_CreateWindow("Ecosystem vX.Xyyyy - SDL/OpenGL", 0, 0, 800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  win = SDL_CreateWindow("Ecosystem vX.Xyyyy - SDL/OpenGL", 0, 0, 800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

  SDL_GLContext context = SDL_GL_CreateContext(win);

  if(!context) {
    cout << "Fail to create GL context !!!" << endl;
    exit(0);
  }


//#################### TEST dessin d'un carre ##########
  //Vertex shader
  const char * vertexShaderSource = "#version 460 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main () {\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

  //Fragment shader orange
  const char * fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main () {\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for shader compile error
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    cout << "233: ERROR::SHADER::VERTEX::COMPILATION_FAILED InfoLog: " << infoLog << endl;
    exit(0);
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    cout << "244: ERROR::SHADER::FRAGMENT::COMPILATION_FAILED InfoLog: " << infoLog << endl;
    exit(0);
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    cout << "256: ERROR::SHADER::PROGRAM::LINKING_FAILED InfoLog: " << infoLog << endl;
    exit(0);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


  float vertices[] = {
    0.5f, 0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f // bottom left 
  };

  GLuint VBO; // Vertex Buffer Objects

  // Generate objects (there is only one object for the first test)
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind Vertex Array
  glBindVertexArray(VAO); // attention ici bizarre lire la doc
  glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bizarre ici aussi moi pas compris
  // 3 = nombre de points
  glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), vertices, GL_STATIC_DRAW);
  //set vertex attribute pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
// ### fin initialisation et calcul pour un carre triangle


}

void Views::OpenGL::output (int x, int y, const char *string) {
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    //glutBitmapCharacter(font, string[i]);
  }
}

void Views::OpenGL::displayRoutine (void) {
  glClear(GL_COLOR_BUFFER_BIT);
  //output(0, 24, (char *)"This is written in a GLUT bitmap font.");
  //output(100, 100, (char *)"GLUT means OpenGL.");
  //output(50, 145, (char *)"(positioned in pixels with upper-left origin)");

//******************************************************************************************
  //box(windows[screen.window()], ACS_VLINE, ACS_HLINE);

  map<int, GraphicComponent *> components = scr.components();
  _keyboardx = scr.selected();

  for(long unsigned int i = 0; i < components.size(); i++) {

    GraphicComponent * a = components[i];

    if (Text * text = dynamic_cast<Text*>(a); text != nullptr) {
      disp(*text);
    } else if (Menu * menu = dynamic_cast<Menu*>(a); menu != nullptr) {
      disp(*menu);
    }
  }

  if(_valid)
    openglend();
    //output(100, 100, "******************");
  _pres.display();
  usleep(100000);
//******************************************************************************************

  //glutSwapBuffers();
}

void Views::OpenGL::keyboard (unsigned char key, int x, int y) {
  if(key == KEYB_CR) {
    _valid = true;
    openglend();
  }
}

void Views::OpenGL::special (int key, int x, int y) {
  _keyboardx = scr.selected();
/*
  if(key == GLUT_KEY_UP) {
    if(_keyboardx > 0)
      _keyboardx--;
  } else if (key == GLUT_KEY_DOWN) {
    if(_keyboardx < _maxKeyboardx)
      _keyboardx++;
  }
*/
  scr.select(_keyboardx);
}

void Views::OpenGL::reshape (int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(0, width, height, 0);
  glMatrixMode(GL_MODELVIEW);
}

void Views::OpenGL::tick(void) {
  _pres.display();
}

void Views::OpenGL::selectMessage (int msg) {
  /*char * message;

  switch (msg) {
  case 1:
    message = (char *)"abcdefghijklmnop";
    break;
  case 2:
    message = (char *)"ABCDEFGHIJKLMNOP";
    break;
  }
*/
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
/*
  const int edges = 2;
  const int yOffset = (this->windowHeight / 2) - 5;
  const int xOffset = ((this->windowWidth - width) / 2);
*/

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
  init();
  SDL_Event event;
  while (1) {
    glViewport(0, 0, 800, 600);
    while(SDL_PollEvent(&event)) {
      cout << "event type: " << event.type << " (" << SDL_QUIT << ")" << endl;
      switch(event.type){
        case SDL_QUIT:
          cout << "Je dois quitter !!!" << endl;
          SDL_DestroyWindow(win);
          SDL_Quit();
          exit(0);
        default:
          cout << event.type << ": I don't know what this event is!" << endl;
          // handle event...
      }
    }
    float greyLevel = 0.3f;
    glClearColor(greyLevel, greyLevel, greyLevel, 0.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
/*
    glColor3f(1.f, 1.f, 1.f);

glBegin(GL_LINES);
    glVertex2f(.25, 1.);
    glVertex2f(1., 1.);
glEnd();
*/
    //draw objects
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 4);
    //glDrawArrays(GL_TRIANGLES, 0, 3); //Pourquoi les triangles ne fonctionnent pas ???? je sais pas encore....

    SDL_GL_SwapWindow(win);
  }
}

void Views::OpenGL::disp (Menu menu) {
  list<string>::iterator it;
  list<string> items = menu.items();
  int y = menu.y();
  const int x = menu.x(); 
  unsigned int cursorPosition = 0;

  it = items.begin();
  if(_keyboardx < items.size())
    advance(it, _keyboardx);
  Dictionary d;
  output(0, 15, (char *)("label: " + d.translate(*it)).c_str());
  string m = "cursorPosition: " + cursorPosition;
  output(0, 45, m.c_str());
  m = "_maxKeyboardx: " + _maxKeyboardx;
  output(0, 60, m.c_str());

  //TODO: le 10 c'est la largeur, donc le calcul de la plus longue chaine de caracteres

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
  output(0, 500, "******************");
  output(0, 515, "This is the end...");
  output(0, 530, "******************");
} 
