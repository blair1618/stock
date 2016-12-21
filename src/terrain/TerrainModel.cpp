//
// Created by Matt Blair on 12/18/16.
//

#include "TerrainModel.hpp"

namespace stock {

const static std::string vs_src = R"SHADER_END(
attribute vec2 a_position;
varying vec2 v_uv;
uniform mat4 u_mvp;
uniform sampler2D u_elevationTex;

void main() {
    vec4 encoded = texture2D(u_elevationTex, a_position);
    float elevation = (encoded.r * 256. * 256. + encoded.g * 256. + encoded.b) - 32768.;
    vec4 position = vec4(a_position, elevation / 19568., 1.);
    v_uv = a_position;
    gl_Position = u_mvp * position;
}
)SHADER_END";

const static std::string fs_src = R"SHADER_END(
varying vec2 v_uv;
uniform vec4 u_tint;
uniform sampler2D u_normalTex;
const vec3 lightDirection = vec3(.5, .5, -.5);

void main() {
    vec4 normal = texture2D(u_normalTex, v_uv);
    float brightness = .5 + dot(normal.xyz, lightDirection);
    gl_FragColor = u_tint * vec4(vec3(brightness), 1.);
}
)SHADER_END";

TerrainModel::TerrainModel()
    : m_shader(fs_src, vs_src),
      m_elevTexLocation("u_elevationTex"),
      m_normalTexLocation("u_normalTex"),
      m_mvpLocation("u_mvp"),
      m_tintLocation("u_tint") {
  m_mesh.setVertexLayout(VertexLayout({ VertexAttribute("a_position", 2, GL_FLOAT, GL_FALSE) }));
}

void TerrainModel::loadElevationTexture(std::vector<uint8_t> data) {
  m_elevationTexture = Texture(Pixmap(data), Texture::Options());
}

void TerrainModel::loadNormalTexture(std::vector<uint8_t> data) {
  m_normalTexture = Texture(Pixmap(data), Texture::Options());
}

void TerrainModel::generateMesh(uint32_t resolution) {

  m_resolution = resolution;

  m_mesh.reset();

  uint16_t index = 0;

  for (uint32_t col = 0; col < resolution; col++) {
    float y = static_cast<float>(col) / resolution;
    for (uint32_t row = 0; row < resolution; row++) {
      float x = static_cast<float>(row) / resolution;
      m_mesh.vertices.push_back({x, y});

      if (row < resolution - 1 && col < resolution - 1) {
        m_mesh.indices.push_back(index);
        m_mesh.indices.push_back(index + 1);
        m_mesh.indices.push_back(index + resolution);

        m_mesh.indices.push_back(index + 1);
        m_mesh.indices.push_back(index + resolution + 1);
        m_mesh.indices.push_back(index + resolution);
      }

      index++;
    }
  }
}

void TerrainModel::render(RenderState &rs, const Camera& camera) {

  m_elevationTexture.bind(rs, 0);
  m_shader.setUniformi(rs, m_elevTexLocation, 0);

  m_normalTexture.bind(rs, 1);
  m_shader.setUniformi(rs, m_normalTexLocation, 1);

  m_shader.setUniformMatrix4f(rs, m_mvpLocation, camera.viewProjectionMatrix());

  if (m_hullIsOn) {
    m_shader.setUniformf(rs, m_tintLocation, glm::vec4(1.f, 1.f, 1.f, 1.f));
    m_mesh.setDrawMode(GL_TRIANGLES);
    m_mesh.draw(rs, m_shader);
  }

  if (m_gridIsOn) {
    m_shader.setUniformf(rs, m_tintLocation, glm::vec4(1.f, .2f, .2f, 1.f));
    m_mesh.setDrawMode(GL_LINES);
    m_mesh.draw(rs, m_shader);
  }

}

void TerrainModel::toggleGrid() {
  m_gridIsOn = !m_gridIsOn;
}

void TerrainModel::toggleHull() {
  m_hullIsOn = !m_hullIsOn;
}

uint32_t TerrainModel::increaseResolution() {
  generateMesh(m_resolution * 2);
  return m_resolution;
}

uint32_t TerrainModel::decreaseResolution() {
  generateMesh(m_resolution / 2);
  return m_resolution;
}

void TerrainModel::dispose(RenderState& rs) {
  m_mesh.dispose(rs);
  m_shader.dispose(rs);
  m_elevationTexture.dispose(rs);
  m_normalTexture.dispose(rs);
}

} // namespace stock