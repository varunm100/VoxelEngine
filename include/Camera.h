#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Buffer.h"
#include <GLFW/glfw3.h>

struct Camera
{
  float m_Yaw = 0.0f, m_Pitch = 0.0f, prevX =  1920/2, prevY = 1080/2;
  glm::vec3 m_Pos, m_Up = glm::vec3(0.0f, 1.0f, 0.0f), m_Dir;
  glm::mat4 view;
  bool first = true;
  const glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080,
		0.1f, 2000.0f);
  const float rotSpeed = 0.045f;
  const float moveSpeed = 3.5f;
  Buffer ubo;
  size_t smat4 = sizeof(glm::mat4);
  
  Camera(glm::vec3 initialPos, glm::vec3 dir);
  ~Camera() {}

  void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  void check_input(GLFWwindow* window, float dt);
  void update_ubo();
};
