#include "World.h"
#include "stb_image.h"

#include <iostream>
World::World() {
}

void World::init_textures() {
  textures.create(GL_TEXTURE_2D_ARRAY);
  int width, height, channels;
  BYTE* image_data = stbi_load("../Textures/sample_wood_texture.png", &width, &height, &channels, 4);
  stbi_set_flip_vertically_on_load(true);
  BYTE* image_data2 = stbi_load("../Textures/sample_grass_texture.jpg", &width, &height, &channels, 4);
  if (image_data != nullptr) {
    textures.init(3,             //no. mipmaps
		  GL_RGBA8,      //internal format
		  width, height, //width, height
		  2);            //depth
  } else {
    std::cout << "failed to create textures: " << stbi_failure_reason() << "\n";
    exit(0);
  }

  textures.set_param(GL_TEXTURE_WRAP_S, GL_CLAMP);
  textures.set_param(GL_TEXTURE_WRAP_T, GL_CLAMP);
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

  textures.generate_mipmaps();
  shader.use();
  shader.setInt("diffuse", 0);
  textures.bind(0);
}

void World::init() {
  float vertices[] = {
    //positons          /texture coords
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
    
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,   // top left
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
  };

  vao.create();
  vbo.create();
  shader.create("../Shaders/empty.shader");

  vao.addAttrib(0, 3, 0);
  vao.addAttrib(1, 2, 3*sizeof(float));
  vao.addBuffer(vbo, BufferType::Vertex, 5*sizeof(float));
  
  vbo.init(sizeof(vertices), nullptr, GL_DYNAMIC_STORAGE_BIT);
  vbo.update(0, sizeof(vertices), &vertices);
  init_textures();
  
  glm::mat4 model(1);
  shader.use();
  shader.setMat4("model", model);
}

void World::render() {
  shader.use();
  vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
