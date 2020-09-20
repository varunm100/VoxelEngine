#pragma once
#include "Common.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Shader {
  u32 program;

  Shader();
  ~Shader();

  void create(const char* location);
  void use();
  void setInt(const char* location, int integer);
  void setMat4(const char* location, glm::mat4& matrix);
};
