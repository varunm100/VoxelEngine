#include "World.h"
#include "stb_image.h"
#include <PerlinNoise/PerlinNoise.hpp>

World::World() {
}

void World::init_textures() {
  textures.create(GL_TEXTURE_2D_ARRAY);
  int width, height, channels;
  BYTE* image_data = stbi_load("../Textures/sample_wood_texture.png", &width, &height, &channels, 4);
  stbi_set_flip_vertically_on_load(true);
  BYTE* image_data2 = stbi_load("../Textures/sample_grass_texture.jpg", &width, &height, &channels, 4);
  BYTE* image_data3 = stbi_load("../Textures/sample_stone_texture.jpeg", &width, &height, &channels, 4);
  if (image_data && image_data2 && image_data3) {
    textures.init(3,             //no. mipmaps
		  GL_RGBA8,      //internal format
		  width, height, //width, height
		  3);            //depth
  } else {
    printf("failed to create textures: %s", stbi_failure_reason()); 
    exit(0);
 }

  textures.set_param(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  textures.set_param(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  textures.set_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  textures.set_param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textures.set_param(GL_TEXTURE_MAX_LEVEL, 3);
  
  textures.update(0,               //mipmap no.
		  0, 0, 0,         //xoffset, yoffset, zoffset
		  width, height, 1,//width, height, depth
		  GL_RGBA,         //format
		  image_data);     //data

  textures.update(0,               //mipmap no.
		  0, 0, 1,         //xoffset, yoffset, zoffset
		  width, height, 1,//width, height, depth
		  GL_RGBA,         //format
		  image_data2);     //data

  textures.update(0,               //mipmap no.
		  0, 0, 2,         //xoffset, yoffset, zoffset
		  width, height, 1,//width, height, depth
		  GL_RGBA,         //format
		  image_data3);     //data

  textures.generate_mipmaps();
  shader.use();
  shader.setInt("diffuse", 0);
  textures.bind(0);
}

void World::init() {
  
  world.emplace_back();
  siv::PerlinNoise perlin(std::random_device{});

  const double frequency = 10;
  const u32 octaves = 5;
  const double fy = CHUNK_SIZE / frequency;
  const double fx = CHUNK_SIZE / frequency;
  const double fz = CHUNK_SIZE / frequency;

  world[0].model = glm::mat4(1);
  // world[0].blocks[1][1][1] = BlockType::WOOD;
  // world[0].blocks[1][1][2] = BlockType::GRASS;
  for (u32 y = 0; y < MAX_HEIGHT; ++y) {
    for (u32 z = 0; z < CHUNK_SIZE; ++z) {
      for (u32 x = 0; x < CHUNK_SIZE; ++x) {
	const double noise(perlin.accumulatedOctaveNoise3D(x/fx, y/fy, z/fz, octaves));
         if (noise < 0.2) {
	   world[0].blocks[y][z][x] = BlockType::STONE;
        }
      }
    }
  }

  vao.create();
  for (auto &c : world) {
    c.vbo.create();
    c.ibo.create();
  }

  shader.create("../Shaders/empty.shader");

  vao.addAttrib(0, 3, offsetof(Vert, pos));
  vao.addAttrib(1, 2, offsetof(Vert, uv));
  vao.addAttrib(2, 1, offsetof(Vert, block_type));
  for (auto &c : world) {
    vao.addBuffer(c.vbo, BufferType::Vertex, sizeof(Vert));
    vao.addBuffer(c.ibo, BufferType::Index, sizeof(u32));
    c.update_mesh();
  }
  
  init_textures();
}

bool Chunk::valid_pos(u32 x, u32 y, u32 z) {
  return (x < CHUNK_SIZE) && (y < CHUNK_SIZE) && (z < CHUNK_SIZE)
    && (x >= 0) && (y >= 0) && (z >= 0);
}

void Chunk::place_block(u32 x, u32 y, u32 z, std::vector<Vert>& vertices, std::vector<u32>& indices) {
  u32 vert_index = vertices.size() - 1;
  BlockType type = blocks[y][z][x];
  if (valid_pos(x+1, y, z) && blocks[y][z][x+1] == BlockType::AIR) {
    vertices.emplace_back(x+1, y, z, 1, 0, type);
    vertices.emplace_back(x+1, y, z+1, 0, 0, type);
    vertices.emplace_back(x+1, y+1, z+1, 0, 1, type);
    vertices.emplace_back(x+1, y+1, z, 1, 1, type);
    indices.emplace_back(vert_index+1);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+4);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+3);
    indices.emplace_back(vert_index+4);
    vert_index += 4;
  }
  if (valid_pos(x, y, z+1) && blocks[y][z+1][x] == BlockType::AIR) { 
    vertices.emplace_back(x, y, z+1, 0, 0, type);
    vertices.emplace_back(x, y+1, z+1, 0, 1, type);
    vertices.emplace_back(x+1, y+1, z+1, 1, 1, type);
    vertices.emplace_back(x+1, y, z+1, 1, 0, type);
    indices.emplace_back(vert_index+1);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+4);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+3);
    indices.emplace_back(vert_index+4);
    vert_index += 4;   
  }
  if (valid_pos(x, y+1, z) && blocks[y+1][z][x] == BlockType::AIR) {
    vertices.emplace_back(x, y+1, z, 1, 1, type);
    vertices.emplace_back(x, y+1, z+1, 0, 1, type);
    vertices.emplace_back(x+1, y+1, z+1, 0, 0, type);
    vertices.emplace_back(x+1, y+1, z, 1, 0, type);
    indices.emplace_back(vert_index+1);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+4);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+3);
    indices.emplace_back(vert_index+4);
    vert_index += 4;
  }
  if (valid_pos(x-1, y, z) && blocks[y][z][x-1] == BlockType::AIR) {
    vertices.emplace_back(x, y, z, 1, 0, type);
    vertices.emplace_back(x, y, z+1, 0, 0, type);
    vertices.emplace_back(x, y+1, z+1, 0, 1, type);
    vertices.emplace_back(x, y+1, z, 1, 1, type);
    indices.emplace_back(vert_index+1);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+4);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+3);
    indices.emplace_back(vert_index+4);
    vert_index += 4;
  }
  if (valid_pos(x, y, z-1) && blocks[y][z-1][x] == BlockType::AIR) {
    vertices.emplace_back(x, y, z, 0, 0, type);
    vertices.emplace_back(x, y+1, z, 0, 1, type);
    vertices.emplace_back(x+1, y+1, z, 1, 1, type);
    vertices.emplace_back(x+1, y, z, 1, 0, type);
    indices.emplace_back(vert_index+1);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+4);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+3);
    indices.emplace_back(vert_index+4);
    vert_index += 4;   
  }
  if (valid_pos(x, y-1, z) && blocks[y-1][z][x] == BlockType::AIR) {
    vertices.emplace_back(x, y, z, 1, 1, type);
    vertices.emplace_back(x, y, z+1, 0, 1, type);
    vertices.emplace_back(x+1, y, z+1, 0, 0, type);
    vertices.emplace_back(x+1, y, z, 1, 0, type);
    indices.emplace_back(vert_index+1);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+4);
    indices.emplace_back(vert_index+2);
    indices.emplace_back(vert_index+3);
    indices.emplace_back(vert_index+4);
    vert_index += 4;
  }
}

void Chunk::update_mesh() {
  std::vector<Vert> vertices;
  std::vector<u32> indices;

  for (u32 y = 0; y < MAX_HEIGHT; ++y) {
    for (u32 z = 0; z < CHUNK_SIZE; ++z) {
      for (u32 x = 0; x < CHUNK_SIZE; ++x) {
	if (blocks[y][z][x] != BlockType::AIR) {
	  place_block(x, y, z, vertices, indices);
	}
      }
    }
  }
  draw_count = indices.size();
  vbo.init(vertices.size()*sizeof(Vert), nullptr, GL_DYNAMIC_STORAGE_BIT);
  vbo.update(0, vertices.size()*sizeof(Vert), vertices.data());
  ibo.init(indices.size()*sizeof(u32), nullptr, GL_DYNAMIC_STORAGE_BIT);
  ibo.update(0, indices.size()*sizeof(u32), indices.data());
}

void World::render() {
  shader.use();
  for (auto &c : world) {
    shader.setMat4("model", c.model);
    
    vao.bind();
    c.vbo.bind(GL_ARRAY_BUFFER);
    c.ibo.bind(GL_ELEMENT_ARRAY_BUFFER);
    glDrawElements(GL_TRIANGLES, c.draw_count, GL_UNSIGNED_INT, nullptr);
  }
}
