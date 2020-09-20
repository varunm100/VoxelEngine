#pragma once
#include <glad/glad.h>
#include "Common.h"

struct Texture {
  u32 id;

  Texture() {}
  ~Texture();

  void create(GLenum target);
  void init(u32 width, u32 height, GLenum internal_format);
  void update(BYTE* image_data, u32 width, u32 height);

  void generate_mipmaps();  
  void upload_from_path(const char* image_path);
  void set_param(GLenum pname, GLint param);
  void bind(u32 tex_unit_index);
};

struct Texture3D {
  u32 id;

  Texture3D() {}
  ~Texture3D();

  void create(GLenum target);
  void init(u32 levels, GLenum internal_format, u32 width, u32 height, u32 depth);
  void update(u32 level, u32 xoffset, u32 yoffset, u32 zoffset, u32 width, u32 height, u32 depth, GLenum format, BYTE* image_data);

  void generate_mipmaps();
  void set_param(GLenum pname, GLint param);
  void bind(u32 tex_unit_index);
};
