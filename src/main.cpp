#include <string>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "World.h"

void  APIENTRY glDebugCallback(GLenum source, GLenum mType, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void render_loop();

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow* window = glfwCreateWindow(1920, 1080, "Voxel Engine", nullptr, nullptr);
  if (window == nullptr) {
    printf("could not glfwCreateWindow()...\n");
    glfwTerminate();
    return 0;
  }

  glfwSwapInterval(true); // vsync
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) printf("Failed to initialize GLAD :-( \n");

  glViewport(0, 0, 1920, 1080);

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glDebugCallback, nullptr);
  glEnable(GL_DEPTH_TEST);

  World world;
  world.init();
  
  Camera cam(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetWindowUserPointer(window, &cam);
  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
    Camera* tcam = (Camera*)glfwGetWindowUserPointer(window);
    tcam->mouse_callback(window, xpos, ypos);
  });

  float time = glfwGetTime();
  float dt;
  glm::mat4 model(1);
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    dt = glfwGetTime() - time;
    time = glfwGetTime();
    cam.check_input(window, dt);
    
    world.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void  APIENTRY glDebugCallback(GLenum source, GLenum mType, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::string src;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        src = "Windows";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        src = "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        src = "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        src = "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        src = "Other";
        break;
    }

    std::string type;
    switch (mType) {
    case GL_DEBUG_TYPE_ERROR:
        type = "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type = "Deprecated Behavior";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type = "Undefined Behavior";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type = "Portability Error";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type = "Performance Error";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type = "Other Message";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        spdlog::trace("GL NOTIFICATION - {} {}: {}", src, type, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        spdlog::warn("GL LOW - {} {}: {}", src, type, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        spdlog::error("GL MEDIUM - {} {}: {}", src, type, message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        spdlog::critical("GL HIGH - {} {}: {}", src, type, message);
        break;
    }
    return;
}
