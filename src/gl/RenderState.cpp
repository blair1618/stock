//
// Created by Matt Blair on 4/2/16.
//
#include "gl/Error.hpp"
#include "gl/RenderState.hpp"

namespace stock {

// TODO: read max texture units from hardware
constexpr size_t maxCombinedTextureUnits = 16;

GLuint RenderState::getTextureUnit(GLuint _unit) { return GL_TEXTURE0 + _unit; }

void RenderState::reset() {

  m_blending.set = false;
  m_blendingFunc.set = false;
  m_clearColor.set = false;
  m_colorMask.set = false;
  m_cullFace.set = false;
  m_culling.set = false;
  m_depthTest.set = false;
  m_depthMask.set = false;
  m_frontFace.set = false;
  m_stencilTest.set = false;
  m_stencilMask.set = false;
  m_program.set = false;
  m_indexBuffer.set = false;
  m_vertexBuffer.set = false;
  m_texture.set = false;
  m_textureUnit.set = false;

  attributeBindings.fill(0);

  CHECK_GL(glDepthFunc(GL_LESS));
}

int RenderState::nextAvailableTextureUnit() {
  if (m_nextTextureUnit + 1 > maxCombinedTextureUnits) {
    Log::e("Too many combined texture units are being used");
  }

  return ++m_nextTextureUnit;
}

void RenderState::releaseTextureUnit() { m_nextTextureUnit--; }

int RenderState::currentTextureUnit() { return m_nextTextureUnit; }

void RenderState::resetTextureUnit() { m_nextTextureUnit = -1; }

inline void setGlFlag(GLenum flag, GLboolean enable) {
  if (enable) {
    CHECK_GL(glEnable(flag));
  } else {
    CHECK_GL(glDisable(flag));
  }
}

bool RenderState::blending(GLboolean enable) {
  if (!m_blending.set || m_blending.enabled != enable) {
    m_blending = {enable, true};
    setGlFlag(GL_BLEND, enable);
    return false;
  }
  return true;
}

bool RenderState::blendingFunc(GLenum sfactor, GLenum dfactor) {
  if (!m_blendingFunc.set || m_blendingFunc.sfactor != sfactor || m_blendingFunc.dfactor != dfactor) {
    m_blendingFunc = {sfactor, dfactor, true};
    CHECK_GL(glBlendFunc(sfactor, dfactor));
    return false;
  }
  return true;
}

bool RenderState::clearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {
  if (!m_clearColor.set || m_clearColor.r != r || m_clearColor.g != g || m_clearColor.b != b || m_clearColor.a != a) {
    m_clearColor = {r, g, b, a, true};
    CHECK_GL(glClearColor(r, g, b, a));
    return false;
  }
  return true;
}

bool RenderState::colorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
  if (!m_colorMask.set || m_colorMask.r != r || m_colorMask.g != g || m_colorMask.b != b || m_colorMask.a != a) {
    m_colorMask = {r, g, b, a, true};
    CHECK_GL(glColorMask(r, g, b, a));
    return false;
  }
  return true;
}

bool RenderState::cullFace(GLenum face) {
  if (!m_cullFace.set || m_cullFace.face != face) {
    m_cullFace = {face, true};
    CHECK_GL(glCullFace(face));
    return false;
  }
  return true;
}

bool RenderState::culling(GLboolean enable) {
  if (!m_culling.set || m_culling.enabled != enable) {
    m_culling = {enable, true};
    setGlFlag(GL_CULL_FACE, enable);
    return false;
  }
  return true;
}

bool RenderState::depthTest(GLboolean enable) {
  if (!m_depthTest.set || m_depthTest.enabled != enable) {
    m_depthTest = {enable, true};
    setGlFlag(GL_DEPTH_TEST, enable);
    return false;
  }
  return true;
}

bool RenderState::depthMask(GLboolean enable) {
  if (!m_depthMask.set || m_depthMask.enabled != enable) {
    m_depthMask = {enable, true};
    CHECK_GL(glDepthMask(enable));
    return false;
  }
  return true;
}

bool RenderState::frontFace(GLenum face) {
  if (!m_frontFace.set || m_frontFace.face != face) {
    m_frontFace = {face, true};
    CHECK_GL(glFrontFace(face));
    return false;
  }
  return true;
}

bool RenderState::stencilMask(GLuint mask) {
  if (!m_stencilMask.set || m_stencilMask.mask != mask) {
    m_stencilMask = {mask, true};
    CHECK_GL(glStencilMask(mask));
    return false;
  }
  return true;
}

bool RenderState::stencilFunc(GLenum func, GLint ref, GLuint mask) {
  if (!m_stencilFunc.set || m_stencilFunc.func != func || m_stencilFunc.ref != ref || m_stencilFunc.mask != mask) {
    m_stencilFunc = {func, ref, mask, true};
    CHECK_GL(glStencilFunc(func, ref, mask));
    return false;
  }
  return true;
}

bool RenderState::stencilOp(GLenum sfail, GLenum spassdfail, GLenum spassdpass) {
  if (!m_stencilOp.set || m_stencilOp.sfail != sfail || m_stencilOp.spassdfail != spassdfail ||
      m_stencilOp.spassdpass != spassdpass) {
    m_stencilOp = {sfail, spassdfail, spassdpass, true};
    CHECK_GL(glStencilOp(sfail, spassdfail, spassdpass));
    return false;
  }
  return true;
}

bool RenderState::stencilTest(GLboolean enable) {
  if (!m_stencilTest.set || m_stencilTest.enabled != enable) {
    m_stencilTest = {enable, true};
    setGlFlag(GL_STENCIL_TEST, enable);
    return false;
  }
  return true;
}

bool RenderState::shaderProgram(GLuint program) {
  if (!m_program.set || m_program.program != program) {
    m_program = {program, true};
    CHECK_GL(glUseProgram(program));
    return false;
  }
  return true;
}

bool RenderState::texture(GLenum target, GLuint handle) {
  if (!m_texture.set || m_texture.target != target || m_texture.handle != handle) {
    m_texture = {target, handle, true};
    CHECK_GL(glBindTexture(target, handle));
    return false;
  }
  return true;
}

bool RenderState::textureUnit(GLuint unit) {
  if (!m_textureUnit.set || m_textureUnit.unit != unit) {
    m_textureUnit = {unit, true};
    // Our cached texture handle is irrelevant on the new unit, so unset it.
    m_texture.set = false;
    CHECK_GL(glActiveTexture(getTextureUnit(unit)));
    return false;
  }
  return true;
}

bool RenderState::vertexBuffer(GLuint handle) {
  if (!m_vertexBuffer.set || m_vertexBuffer.handle != handle) {
    m_vertexBuffer = {handle, true};
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, handle));
    return false;
  }
  return true;
}

bool RenderState::indexBuffer(GLuint handle) {
  if (!m_indexBuffer.set || m_indexBuffer.handle != handle) {
    m_indexBuffer = {handle, true};
    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle));
    return false;
  }
  return true;
}

void RenderState::vertexBufferUnset(GLuint handle) {
  if (m_vertexBuffer.handle == handle) {
    m_vertexBuffer.set = false;
  }
}

void RenderState::indexBufferUnset(GLuint handle) {
  if (m_indexBuffer.handle == handle) {
    m_indexBuffer.set = false;
  }
}

void RenderState::shaderProgramUnset(GLuint program) {
  if (m_program.program == program) {
    m_program.set = false;
  }
}

void RenderState::textureUnset(GLenum target, GLuint handle) {
  if (m_texture.handle == handle) {
    m_texture.set = false;
  }
}

} // namespace stock
