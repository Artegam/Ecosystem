#include "Views.h"


Views::Window::Window(const unsigned int width, const unsigned int height) {
  uiWidth = width;
  uiHeight = height;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  createWindow (uiWidth, uiHeight);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  render = NULL;

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

Views::Window::~Window() {
}

int Views::Window::createWindow (const unsigned int width, const unsigned int height) {
  window = glfwCreateWindow(uiWidth, uiHeight, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    cout << "Failed to create GLFW window" << endl;
    glfwTerminate();
    return -1;
  }
  return 0;
}

void Views::Window::resizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void Views::Window::processInput (GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    keyPressed = GLFW_KEY_DOWN;
  else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    keyPressed = GLFW_KEY_UP;
  else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    keyPressed = GLFW_KEY_LEFT;
  else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    keyPressed = GLFW_KEY_RIGHT;
  else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    keyPressed = GLFW_KEY_ENTER;
  else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && keyPressed == GLFW_KEY_DOWN) {
    keyPressed = 0;
    ycursor++;
    calculate();
  }
  else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && keyPressed == GLFW_KEY_UP) {
    keyPressed = 0;
    ycursor--;
    calculate();
  }
  else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && keyPressed == GLFW_KEY_LEFT) {
    keyPressed = 0;
    xcursor--;
    calculate();
  }
  else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE && keyPressed == GLFW_KEY_RIGHT) {
    keyPressed = 0;
    xcursor++;
    calculate();
  }
  else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && keyPressed == GLFW_KEY_ENTER) {
    keyPressed = 0;
    isValidated = true;
  }
}

bool Views::Window::shouldClose () {
  return glfwWindowShouldClose(window);
}

void Views::Window::close () {
  glfwTerminate();
}

void Views::Window::display () {
  processInput (window);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  if((*render) != NULL)
    render(this, shader);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Views::Window::arraysConfig () {
  glGenVertexArrays(1, &uiVAO);
  glGenBuffers(1, &uiVBO);
  glBindVertexArray(uiVAO);
  glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Views::Window::setCharacters (std::map<GLchar, Character> lst) {
  Characters.clear();
  Characters.insert(lst.begin(), lst.end());
}

void Views::Window::setRenderFunc (void (*fct)(Window * win, Shader * shader)) {
  render = fct;
}

const unsigned int Views::Window::width () {
  return uiWidth;
}

const unsigned int Views::Window::height () {
  return uiHeight;
}

void Views::Window::renderText (Shader &shader, string text, float x, float y, float scale, glm::vec3 color) {
  // activate corresponding render state	
  shader.use();
  glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(uiVAO);

  // y recalcule en fonction de l ecran et de la hauteur des characteres
  // 30 px = hauteur du charactere
  float consoleHeight = 30.;
  float consoleWidth = 254.;
  x = (uiWidth / consoleWidth) * x;
  y = uiHeight - ((uiHeight / consoleHeight) * y);
  // iterate through all characters
  string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) 
  {
    Character ch = Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;
    // update VBO for each character
    float vertices[6][4] = {
      { xpos,     ypos + h,   0.0f, 0.0f },            
      { xpos,     ypos,       0.0f, 1.0f },
      { xpos + w, ypos,       1.0f, 1.0f },

      { xpos,     ypos + h,   0.0f, 0.0f },
      { xpos + w, ypos,       1.0f, 1.0f },
      { xpos + w, ypos + h,   1.0f, 0.0f }           
    };
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Views::Window::useShader (Shader * vshader) {
  shader = vshader;
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(uiWidth), 0.0f, static_cast<float>(uiHeight));
  shader->use();
  glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

int Views::Window::getXCursorPosition () {
  return xcursor;
}

int Views::Window::getYCursorPosition () {
  return ycursor;
}

void Views::Window::setYCursorLimits(int min, int max) {
  ymin = min;
  ymax = max;
  yLimitsDefined = true;
}

void Views::Window::setXCursorLimits(int min, int max) {
  xmin = min;
  xmax = max;
  xLimitsDefined = true;
}

void Views::Window::disableXLimits() {
  xmin = 0;
  xmax = 0;
  xLimitsDefined = false;
}

void Views::Window::disableYLimits() {
  ymin = 0;
  ymax = 0;
  yLimitsDefined = false;
}

void Views::Window::calculate() {
  if(xLimitsDefined) {
    if(xcursor < xmin)
      xcursor = xmin;
    if(xcursor > xmax)
      xcursor = xmax;
  }
  if(yLimitsDefined) {
    if(ycursor < ymin)
      ycursor = ymin;
    if(ycursor > ymax)
      ycursor = ymax;
  }
}

bool Views::Window::isValidate() {
  bool res = false;
  if(isValidated) {
    res = true;
    isValidated = false;
  }
  return res;
}
