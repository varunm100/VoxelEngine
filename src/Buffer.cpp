#include "Buffer.h"

void Buffer::create() {
  glCreateBuffers(1, &id);
}

void Buffer::init(size_t size, const void* data, GLbitfield flags) {
  glNamedBufferStorage(id, size, data, flags);
}

void Buffer::update(u32 offset, size_t size, const void *data) {
  glNamedBufferSubData(id, offset, size, data);
}

void* Buffer::map(u32 offset, size_t length, GLbitfield flags) {
  return glMapNamedBufferRange(id, offset, length, flags);
}

void Buffer::bind_base(GLenum target, u32 index, u32 offset, size_t size) {
  glBindBufferRange(target, index, id, offset, size);
}

Buffer::~Buffer() { glDeleteBuffers(1, &id); }
