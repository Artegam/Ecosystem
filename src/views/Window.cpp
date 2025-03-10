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
}

void Views::Window::display () {
  while (!glfwWindowShouldClose(window))
  {
    processInput (window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if((*render) != NULL)
      render(this, shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
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


