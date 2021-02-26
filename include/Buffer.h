#pragma once

#include "Common.h"
#include <glad/glad.h>
#include "Texture.h"
#include <glm/glm.hpp>

struct Vert {
  glm::vec3 pos;
  glm::vec2 uv;
  float block_type;
  Vert(float x, float y, float z, float ux, float uy, float _block_type) {
    pos.x = x;    pos.y = y;    pos.z = z;    uv.x = ux;    uv.y = uy;   block_type = _block_type;
  }
};

enum BufferType {
  Vertex,
  Index,
  Uniform,
};

struct Buffer {
  u32 id;

  Buffer() {}
  ~Buffer();

  void create();
  void upload(const void* data, size_t size, GLbitfield flags);
  void init(size_t size, const void* data, GLbitfield flags);
  void* map(u32 offset, size_t length, GLbitfield flags);
  void update(u32 offset, size_t size, const void *data);
  void bind_base(GLenum target, u32 index, u32 offset, size_t size);
  void bind(GLenum target);
};

struct FrameBuffer {
  u32 id;

  FrameBuffer() {}
   ~FrameBuffer();

  void create();
  void attach_tex(GLenum attachment, Texture& texture, u32 level);
  void draw_buffers(u32 n, const GLenum *bufs);
  /* void attach_rbo(GLenum attachment, Buffer& rbo); 
     Do not support render buffers yet!
  */
};
