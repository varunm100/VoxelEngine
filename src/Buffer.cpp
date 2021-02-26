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

void Buffer::bind(GLenum target) { glBindBuffer(target, id); }

Buffer::~Buffer() { glDeleteBuffers(1, &id); }

// FrameBuffer

FrameBuffer::~FrameBuffer() { glDeleteFramebuffers(1, &id); }

void FrameBuffer::create() { glCreateFramebuffers(1, &id); }

void FrameBuffer::attach_tex(GLenum attachment, Texture& texture, u32 level) {
  glNamedFramebufferTexture(id, attachment, texture.id, level);
}

void FrameBuffer::draw_buffers(u32 n, const GLenum *bufs) {
  glNamedFramebufferDrawBuffers(id, n, bufs);
}

// void FrameBuffer::attach_rbo(GLenum attachment, Buffer& rbo) {
//   glNamedFramebufferRenderbuffer(id, attachment, GL_RENDERBUFFER, rbo.id);
// }
