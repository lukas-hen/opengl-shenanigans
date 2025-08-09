#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

#include "file.h"

#define WIDTH 800
#define HEIGHT 800

// ------------------
// Forward declarations
// ------------------
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char** argv) {

  // ------------------
  // Initialize GLFW
  // ------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // OSX Specific
#endif

  // ------------------
  // Configure window
  // ------------------
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Could not create window.\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  
  // Callbacks should always be registered after window init, before rendering.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // ------------------
  // Load openGL functions
  // ------------------
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD.\n");
    return -1;
  }
  
  // ------------------
  // Vertex Shader
  // ------------------

  // Load Shader
  size_t vertexShaderSize;
  char *vertexShaderSource;
  FileReadResult res = f_readall("./shaders/vertex_shader.glsl", &vertexShaderSource, &vertexShaderSize);
  if (res != READ_SUCCESSFUL) {
    fprintf(stderr, "Failed to load vertex shader.\n");
  }

  // Create shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  
  // Attach & Compile shader
  glShaderSource(vertexShader, 1, (const GLchar *const *)&vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      fprintf(stderr, "Failed to compile vertex shader:\n%s\n", infoLog);
  }

  // ------------------
  // Fragment Shader
  // ------------------

  // Load external data
  size_t fragmentShaderSize;
  char *fragmentShaderSource;
  res = f_readall("./shaders/fragment_shader.glsl", &fragmentShaderSource, &fragmentShaderSize);
  if (res != READ_SUCCESSFUL) {
    fprintf(stderr, "Failed to load fragment shader.\n");
  }

  // Create shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  
  // Attach & Compile shader
  glShaderSource(fragmentShader, 1, (const GLchar *const *)&fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);


  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      fprintf(stderr, "Failed to compile fragment shader:\n%s\n", infoLog);
  }

  // ------------------
  // Shader Linking
  // ------------------
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      fprintf(stderr, "Failed to link shaders:\n%s\n", infoLog);
  }

  // Early cleanup, already loaded.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  free(vertexShaderSource);
  free(fragmentShaderSource);

  float vertices[] = {
    // vertices:         // colors:
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // bottom left
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // bottom left
   -0.5f, 0.5f,  0.0f,   0.3f, 0.3f, 0.05f,  // top left
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // top right
  };

  unsigned int VBO, VAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  // Bind vertex array obj first. Then attributes.
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Set vertex pointer settings

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) (3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // ------------------
  // Main Render Loop
  // ------------------
  while(!glfwWindowShouldClose(window)) {

    // input
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // float timeValue = glfwGetTime();
    // float greenValue = sin(timeValue)/ 2.0f + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "triangleColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 0.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3*2);

    // swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}


void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}