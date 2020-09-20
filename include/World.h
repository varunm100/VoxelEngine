#include "Common.h"
#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

struct World {
  Shader shader;
  VertexArray vao;
  Buffer vbo;
  Texture3D textures; // all block textures
  
  World();
  ~World() { }

  void init_textures();
  
  void init();
  void update_mesh();
  void render();
};
