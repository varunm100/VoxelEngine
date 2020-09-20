#pragma once

#include "Common.h"
#include "Buffer.h"

struct VertexArray {
  u32 id;

  VertexArray() {}
  ~VertexArray();

  void create();
  void addBuffer(Buffer& buffer, BufferType type, size_t stride);
  void addAttrib(const u32 index, const u32 count, const u32 offset);
  void bind();
};
