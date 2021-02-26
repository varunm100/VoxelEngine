#include "Common.h"
#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "WorldConfig.h"
#include <vector>

struct Chunk {
  Buffer vbo;
  Buffer ibo;
  u32 draw_count;
  glm::vec3 position;
  glm::mat4 model;
  BlockType blocks[MAX_HEIGHT*2][CHUNK_SIZE][CHUNK_SIZE];

  Chunk() {};
  ~Chunk() {};

  bool valid_pos(u32 x, u32 y, u32 z);
  void place_block(u32 x, u32 y, u32 z, std::vector<Vert>& vertices, std::vector<u32>& indices);
  void update_mesh();
};

struct World {
  Shader shader;
  VertexArray vao;
  Texture3D textures; // all block textures
  std::vector<Chunk> world;
  
  World();
  ~World() { }

  void init_textures();
  
  void init();
  void render();
};
