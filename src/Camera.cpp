#include "Camera.h"

Camera::Camera(glm::vec3 initialPos, glm::vec3 dir)
    : m_Pos{initialPos}, m_Dir{dir} {
  view = glm::lookAt(m_Pos, glm::vec3(0.0f, 0.0f, 0.0f), m_Up);
  ubo.create();
  ubo.init(smat4*2, nullptr, GL_DYNAMIC_STORAGE_BIT);
  ubo.bind_base(GL_UNIFORM_BUFFER, 0, 0, smat4*2);
  ubo.update(0, smat4, glm::value_ptr(projection));
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (first) {
    prevX = xpos;
    prevY = ypos;
    first = false;
  }
  float dx = xpos - prevX;
  float dy = -ypos + prevY;
  prevX = xpos;
  prevY = ypos;
  m_Yaw += (dx * rotSpeed);
  m_Pitch += (dy * rotSpeed);
  if (m_Pitch > 89.0f)
    m_Pitch = 89.0f;
  if (m_Pitch < -89.0f)
    m_Pitch = -89.0f;
  m_Dir = glm::normalize(glm::vec3(cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)), sin(glm::radians(m_Pitch)), sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))));
  view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);
}

void Camera::check_input(GLFWwindow* window, float dt) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    m_Pos += m_Dir * moveSpeed * dt;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    m_Pos -= m_Dir * moveSpeed * dt;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    m_Pos += glm::normalize(glm::cross(m_Up, m_Dir)) * moveSpeed * dt;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    m_Pos -= glm::normalize(glm::cross(m_Up, m_Dir)) * moveSpeed * dt;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    m_Pos += glm::normalize(m_Up) * moveSpeed * dt;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    m_Pos -= glm::normalize(m_Up) * moveSpeed * dt;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    exit(0);
  update_ubo();
}

void Camera::update_ubo() {  
  view = glm::lookAt(m_Pos, m_Pos + m_Dir, m_Up);


  ubo.update(smat4, smat4, glm::value_ptr(view));
}
