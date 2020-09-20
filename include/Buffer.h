#pragma once

#include "Common.h"
#include <glad/glad.h>

enum BufferType {
  Vertex = 0,
  Index = 1,
  Uniform = 3,
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
};
