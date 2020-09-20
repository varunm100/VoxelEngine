#include "VertexArray.h"

void VertexArray::create() { glCreateVertexArrays(1, &id); }

void VertexArray::addBuffer(Buffer& buffer, BufferType type, size_t stride) {
  switch (type) {
  case BufferType::Vertex:
    glVertexArrayVertexBuffer(id, 0, buffer.id, 0, stride);
  case BufferType::Index:
    glVertexArrayElementBuffer(id, buffer.id);
  }
}

void VertexArray::addAttrib(const u32 index, const u32 count, const u32 offset) {
    glEnableVertexArrayAttrib(id, index);
    glVertexArrayAttribFormat(id, index, count, GL_FLOAT, GL_FALSE, offset);
    glVertexArrayAttribBinding(id, index, 0);
}

void VertexArray::bind() { glBindVertexArray(id); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &id); }
