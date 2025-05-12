#ifndef VIEWS_H
#define VIEWS_H

#include "GraphicComponents.h"
#include "Translations.h"

#include <ncurses.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <unistd.h>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <thread>

#include <GLES3/gl3.h>
//#include <EGL/egl.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <ft2build.h>
#include FT_FREETYPE_H

#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;
using namespace GraphicComponents;
using namespace Translations;

namespace Views {
  // Keyboard keys
  const int KEYB_DOWN   = 2;
  const int KEYB_UP     = 3;
  const int KEYB_ENTER  = 10;
  const int KEYB_CR     = 13;
  const int KEYB_SPACE  = 32;
  const int KEYB_ESCAPE = 27;


  // interface
  /// class Presenter - 
  class Presenter {
    protected:
      void * fct;
    public:
      void display ();
  };

  // interface
  /// class View - 
  class View {
    private:

    protected:
      bool toClear = false;
      Presenter * _presenter;
      unsigned int _keyboardx = 0;
      unsigned int _keyboardy = 0;
      unsigned int _maxKeyboardx = 0;
      unsigned int _maxKeyboardy = 0;
      bool _valid = false;
      Screen scr;
      Dictionary dict;
      bool tobeClosed = false;

    // Operations
    public:

      View (Presenter * presenter);
      virtual void init (int height, int width);
      void end ();
      bool shouldClose ();
      void close ();
      virtual void display ();
      virtual void display (Screen screen);
      void display (Menu menu);
      void display (Text text);
      virtual void mainMenu (int keybPosition = -1);
      virtual void options (int keybPosition = -1);
      virtual void languages (int keybPosition = -1);
      virtual void video (int keybPosition = -1);
      virtual void gameplay (int position, map<int, int> worldMap);
      virtual int getChar ();
      virtual void initScreen();
      virtual void clearScreen ();
      virtual void validateOption (int optionNumber);
      virtual void createWindow (int position, int x, int y, int height, int width);
      void keyboard ();
      const int getKeyboardx ();
      const int getKeyboardy ();
      void setMaxKeyboardx(const int max = 0);
      void setMaxKeyboardy(const int max = 0);
      void resetKeyboard();
      bool isValid ();
      void setLanguage(const unsigned int lang);
  };

  struct bubble_data {
    unsigned int index = 0;
    unsigned int y = rand()%25 + 25;
    unsigned int x = rand()%254;
  };

  /// class NCursesView -
  class NCurses : public View {
    private:
      // Attributes
      int worldHeight = 1;
      int worldWidth = 1;
      int choice;
      int highlight = 0;
      int windowHeight = 0;
      int windowWidth = 0;
      void redraw (WINDOW * mainMenu);
      void drawChar (WINDOW * win, int x, int y, char c, char color);
      const unsigned int bub_max = 15;
      unsigned int bub_nb = 15;
      list<bubble_data> bub_data;

    protected:
      bool toClear = true;
      vector<WINDOW*> windows;

      // Operations
    public:
      NCurses (Presenter * presenter);
      ~NCurses ();
      void init (int height, int width);
      void createWindow (int screen, int x, int y, int height = 2, int width = 10);
      void displayCursorPosition (int keybPosition);
      static void logCursorPosition (int keybPosition);
      void gameplay (int position, map<int, int> worldMap);
      void end ();
      int getChar ();
      void hello (); // Pour tester l'affichage en cas de soucis
      void validateOption (int optionNumber);
      void clearScreen();
      void initScreen();
      void display ();
      void display (Screen screen);
      void display (Menu menu);
      void display (Text text);
      bubble_data bubble (bubble_data data);
      void bubbles ();
      void keyboard ();
  };

  //Texture wrapper class
  class Texture
  {
    private:
      SDL_Renderer * renderer;
      SDL_Texture * texture;   //The actual hardware texture
      int width, height;
      void init ();
      void free ();            //Deallocates memory
      bool createTextureFromSurface (SDL_Surface * surface);

    public:
      Texture ();
      ~Texture ();
      bool loadFromFile (string path, SDL_Renderer * render);                                                     //Loads image at specified path
      bool loadFromRenderedText (string textureText, SDL_Renderer * render, TTF_Font * font, SDL_Color textColor);//Creates image from font string
                                                                                           //Renders texture at given point
      void render (int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
      int getWidth ();
      int getHeight ();
  };


  class Shader {
    private:
      void checkCompileErrors (GLuint shader, string type);

    public:
      unsigned int ID;
      Shader (const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
      void use ();
  };

  struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
  };

  class Font {
    private:
      map<GLchar, Character> Characters;

    public:
      Font();
      ~Font();
      int load(const char * fontName);
      map<GLchar, Character> getTable();
  };

  class Window
  {
    private:
      unsigned int uiWidth = 800;
      unsigned int uiHeight = 600;
      GLFWwindow * window;
      void (*render)(Window * win, Shader * shader); //pointeur sur fonction
      unsigned int uiVAO, uiVBO;
      map<GLchar, Character>  Characters;
      Shader * shader;
      int xcursor = 0;
      int ycursor = 0;
      bool xLimitsDefined = false;
      bool yLimitsDefined = false;
      bool isValidated = false;
      int xmin = 0;
      int xmax = 0;
      int ymin = 0;
      int ymax = 0;
      int keyPressed = 0;
      int XValidated = 0;
      int YValidated = 0;

      void framebuffer_size_callback (GLFWwindow * window, int width, int height);
      void processInput (GLFWwindow * window);
      int createWindow (const unsigned int width, const unsigned int height);
      static void resizeCallback (GLFWwindow* window, int width, int height);
      void calculate(); // calculate x and y cursor with limits if they are sets

    public:
      Window (const unsigned int width, const unsigned int height);
      ~Window ();
      const unsigned int width ();
      const unsigned int height ();
      bool shouldClose ();
      void close ();
      void display ();
      void setCharacters (std::map<GLchar, Character> lst);
      void setRenderFunc (void (*fct)(Window * win, Shader * shader));
      void renderText (Shader &shader, string text, float x, float y, float scale, glm::vec3 color);
      void arraysConfig ();
      void useShader(Shader * vshader);
      int getXCursorPosition ();
      int getYCursorPosition ();
      void setYCursorLimits(int min, int max);
      void setXCursorLimits(int min, int max);
      void disableXLimits();
      void disableYLimits();
      bool isValidate();
  };

  /// class OpenGLView -
  class OpenGL : public View {
    private:
      // Attributes
      int worldHeight = 1;
      int worldWidth = 1;
      int choice;
      int highlight = 0;
      int windowHeight = 0;
      int windowWidth = 0;
      static vector<WINDOW*> windows;
      static Screen scr;
      static int _maxKeyboardx;
      static unsigned int _keybx;
      Presenter * _pres;
      static Dictionary dict;
      unsigned int shaderProgram;
      GLuint VAO; // Vertex Array Objects
      Shader * myTriangles;
      Shader * testView;
      Shader * fontShader;
      SDL_Renderer * renderer = NULL;

      unsigned int fontVAO, fontVBO;
      struct Character {
        unsigned int textureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        long int advance;        // Offset to advance to next glyph
      };

      std::map<char, Character> Characters;
      static void test (Window * win, Shader * shader);
      Window * win;
      Shader * shader;

    protected:
      static int _maxKeybx;

      WINDOW * createWindow (int height = 2, int width = 10);
      void redraw (WINDOW * mainMenu);
      void drawChar (WINDOW * win, int x, int y, char c, char color);
      void special (int key, int x, int y);
      void reshape (int width, int height);
      void output (int x, int y, const char *string);
      void selectColor (int color);
      void selectFont (int newfont);
      bool shouldClose ();
      void close ();
      void display (Screen screen);
      static void display (Window * win, Shader * shader, Menu menu);
      static void display (Text text);
      //list<string> nodesToString (list<Node *> items);
      void box (pair<float, float> pointA, pair<float, float> pointB);
      void openglend ();

      // Operations
    public:
      OpenGL (Presenter * presenter);
      ~OpenGL ();
      void displayCursorPosition (int keybPosition);
  };

};

#endif
