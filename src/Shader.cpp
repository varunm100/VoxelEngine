#include "Shader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>

using namespace std;

void getShaderSrc(const char* filepath, string& vs, string& fs) {
  ifstream stream(filepath);

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  string line;
  stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    if (line.find("#SHADER") != string::npos) {
      if (line.find("VERTEX") != string::npos) {
	type = ShaderType::VERTEX;
      }
      else if (line.find("FRAGMENT") != string::npos) {
	type = ShaderType::FRAGMENT;
      }
    }
    else {
      if (type == ShaderType::VERTEX) {
	ss[0] << line << '\n';
      }
      else if (type == ShaderType::FRAGMENT) {
	ss[1] << line << '\n';
      }
    }
  }
  vs = ss[0].str();
  fs = ss[1].str();
}


Shader::Shader() { 
}

void Shader::create(const char* location) {
  string a, b;
  getShaderSrc(location, a, b);
  const char* vs_src;
  const char* fs_src;
  vs_src = a.c_str();
  fs_src = b.c_str();
  int shaders[3];
  char infoLog[512];
  shaders[0] = glCreateShader(GL_VERTEX_SHADER);
  shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shaders[0], 1, &vs_src, nullptr);
  glShaderSource(shaders[1], 1, &fs_src, nullptr);
  glCompileShader(shaders[0]);
  glCompileShader(shaders[1]);
  glGetShaderiv(shaders[0], GL_COMPILE_STATUS, &shaders[2]);

  if (!(shaders[2])) {
    glGetShaderInfoLog(shaders[0], 512, nullptr, infoLog);
    printf("Vertex Shader Compilation Failed: %s\n", infoLog);
  }
  glGetShaderiv(shaders[1], GL_COMPILE_STATUS, &shaders[2]);
  if (!(shaders[2])) {
    glGetShaderInfoLog(shaders[1], 512, nullptr, infoLog);
    printf("Fragment Shader Compilation Failed: %s\n", infoLog);
  }

  program = glCreateProgram();
  glAttachShader(program, shaders[0]);
  glAttachShader(program, shaders[1]);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &shaders[2]);
  if (!(shaders[2])) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    printf("Linking Shader Failed: \n%s", infoLog);
  }
  glDeleteShader(shaders[0]);
  glDeleteShader(shaders[1]);
}

void Shader::setInt(const char* location, int integer) {
  glUniform1i(glGetUniformLocation(program, location), integer);
}

void Shader::setMat4(const char* location, glm::mat4& matrix) {
  glUniformMatrix4fv(glGetUniformLocation(program, location), 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader::~Shader() { glDeleteProgram(program); }

void Shader::use() { glUseProgram(program); }

