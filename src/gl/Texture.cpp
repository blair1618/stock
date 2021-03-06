//
// Created by Matt Blair on 7/30/16.
//

#include "gl/Error.hpp"
#include "gl/Texture.hpp"
#include "gl/RenderState.hpp"
#include <cassert>
#include <cstdlib>

namespace stock {

Texture::Texture() {}

Texture::Texture(Options options)
    : m_options(options) {}

Texture::Texture(Pixmap pixmap, Options options)
    : m_pixmap(pixmap), m_options(options) {}

Texture::~Texture() {
  assert(m_glHandle == 0);
}

uint32_t Texture::width() const { return m_pixmap.width(); }

uint32_t Texture::height() const { return m_pixmap.width(); }

GLuint Texture::glHandle() const { return m_glHandle; }

void Texture::prepare(RenderState& rs, GLuint unit) {

  if (m_glHandle != 0) {
    return;
  }

  CHECK_GL(glGenTextures(1, &m_glHandle));

  bind(rs, unit);

  CHECK_GL(glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(m_options.minFilter)));
  CHECK_GL(glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(m_options.magFilter)));
  CHECK_GL(glTexParameteri(m_target, GL_TEXTURE_WRAP_S, static_cast<GLenum>(m_options.wrapS)));
  CHECK_GL(glTexParameteri(m_target, GL_TEXTURE_WRAP_T, static_cast<GLenum>(m_options.wrapT)));

  auto width = m_pixmap.width();
  auto height = m_pixmap.height();
  auto data = m_pixmap.pixels();
  auto format = static_cast<GLenum>(m_pixmap.format());
  auto type = static_cast<GLenum>(m_pixmap.type());

  CHECK_GL(glTexImage2D(m_target, 0, format, width, height, 0, format, type, data));

  if (m_options.generateMipmaps) {
    CHECK_GL(glGenerateMipmap(m_target));
  }

  m_pixmap.dispose();
}

void Texture::bind(RenderState& rs, GLuint unit) {
  rs.texture(m_target, unit, m_glHandle);
}

void Texture::dispose(RenderState& rs) {

  rs.textureUnset(m_target, m_glHandle);

  CHECK_GL(glDeleteTextures(1, &m_glHandle));
  m_glHandle = 0;

  m_pixmap.dispose();
}

} // namespace stock
