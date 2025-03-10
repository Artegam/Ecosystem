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

void Views::OpenGL::init () {
/*
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

  myTriangles = new Shader(vertexShaderSource, fragmentShaderSource);
  float vertices[] = {
    0.5f, 0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left 
    -0.5f, 0.5f, 0.f // TEST 
  };

  myTriangles->generateObjects(vertices, sizeof(vertices) / sizeof(float));
// ### fin initialisation et calcul pour un carre triangle
*/
}

void Views::OpenGL::renderText()
{

}

void Views::OpenGL::output (int x, int y, const char *string) {
}

void Views::OpenGL::displayRoutine (void) {
  map<int, GraphicComponent *> components = scr.components();
  _keyboardx = scr.selected();

  glClear(GL_COLOR_BUFFER_BIT);

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
  _pres.display();
  usleep(100000);
}

void Views::OpenGL::keyboard (unsigned char key, int x, int y) {
  if(key == KEYB_CR) {
    _valid = true;
    openglend();
  }
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

void Views::OpenGL::tick(void) {
  _pres.display();
}

void Views::OpenGL::selectMessage (int msg) {
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

void Views::OpenGL::display (Screen screen) {
  win->setRenderFunc(&test);
  win->display();

/*
  Texture gTextTexture;
  //Initialize SDL_ttf
  if (TTF_Init () == -1)
   cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError () << endl;

  SDL_Window * w = SDL_CreateWindow("Ecosystem vX.Xyyyy - SDL/OpenGL", 0, 0, 800, 600, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
  SDL_Renderer * gRenderer = SDL_CreateRenderer (w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (gRenderer == NULL)
    cout << "Renderer could not be created! SDL Error: " << SDL_GetError () << endl;

  TTF_Font * gFont = TTF_OpenFont ("lazy.ttf", 28);
  if (gFont == NULL) {
    cout << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << endl;
    exit(0);
  }
  SDL_Color textColor = { 0, 0, 0 };



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

    // ### MATRIX #####
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-400.f, 400.f, -300.f, 300.f, 0.f, 50.f);

    // hardwire view !!
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    //draw objects
    myTriangles->use();
    //testView->use();

    //############## RENDER ##############
    //TODO: Il y a un pb quelque part ici....
    // Deja comment dessinner un QUAD avec des coordonnees qui sortent de l'ensemble [0, 1] ???
    //Shader fontShader(fontVertexShader, fontFragmentShader);
    //fontShader->use();


    //Render text

    gTextTexture.loadFromRenderedText ("The quick brown fox jumps over the lazy dog", gRenderer, gFont, textColor);

    //################################

    SDL_GL_SwapWindow(win);
  }
*/
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
  string m = "cursorPosition: " + cursorPosition;
  m = "_maxKeyboardx: " + _maxKeyboardx;

  //TODO: le 10 c'est la largeur, donc le calcul de la plus longue chaine de caracteres
  //15 et 9 sont les tailles de font
  box(make_pair(x, y), make_pair(x + 10, y + items.size() + 1));

  for(it = items.begin(); it != items.end(); it++) {
    if(cursorPosition == _keyboardx)
      glColor3f(0.0, 1.0, 0.0);
    // le 15 est la hauteur de la font => à stocker dans un tableau ou est-ce qu elle est accessible ?
    if(cursorPosition == _keyboardx)
      glColor3f(1.0, 1.0, 1.0);
    y++;
    cursorPosition++;
  }
}

void Views::OpenGL::disp (Text text) {
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
} 

void Views::OpenGL::test (Window * win, Shader * shader) {
  win->renderText(*shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
  win->renderText(*shader, "(C) LearnOpenGL.com", 0.f, 450.f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
  win->renderText(*shader, "Test", 250.f, 250.f, 0.5f, glm::vec3(0.9, 0.7f, 0.3f));
}

