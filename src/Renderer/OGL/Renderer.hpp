#pragma once

#include "../../Window/Listeners.hpp"
#include "../I_Object.hpp"
#include "../I_Renderer.hpp"
#include "../Shader/Shader.hpp"
#include "../Texture/OGL_Texture.hpp"
#include <memory>

namespace OGL {
// TODO: Make this class an interface for other renderer too
class ArrayMesh : public I_Object3D {
public:
  ArrayMesh();
  ~ArrayMesh();

  std::vector<glm::vec3> GetPositions() const;
  std::vector<glm::vec2> GetUVs() const;
  std::vector<glm::vec3> GetNormals() const;
  std::vector<uint32_t> GetIndices() const;

  void SubData(const std::vector<glm::vec3> &p_pos,
               const std::vector<glm::vec2> &p_uvs,
               const std::vector<glm::vec3> &p_norms,
               const std::vector<uint32_t> &m_indices);

  std::shared_ptr<Shader> GetShader() const { return m_shader; }

protected:
  std::shared_ptr<Shader> m_shader;

public:
  // 0: position, 1: uvs, 2: normals
  GLuint m_vao, m_vbos[3], m_ebo;
  unsigned int m_numVertices, m_numIndices;
};

class Renderer : public I_Renderer3D, public FrameBufferSizeListener {
public:
  Renderer(Camera&& camera, int width, int height);

  virtual ~Renderer();

  /**
   * This function is called once before any Render call.
   *
   * A renderer can implement this function to setup render data for the objects
   * and implement any pre-rendering optimizations.
   */
  virtual void
  SetupObjects(const std::vector<std::shared_ptr<I_Object3D>> &objects) override;

  /**
   * Interface function responsible for rendering the scene.
   */
  virtual std::shared_ptr<I_Texture2D> Render() override;

  /**
   * Inherits from FrameBufferSizeListener
   */
  virtual void OnFrameBufferSize(int width, int height) override;
private:
  void ResizeColorBuffer(int width, int height);
  void ResizeDepthBuffer(int width, int height);

private:
  Camera m_camera;
  int m_width, m_height;

  std::vector<std::shared_ptr<ArrayMesh>> m_objs;

  GLuint m_fbo, m_depthBuffer;
  std::shared_ptr<OGL_Texture2D> m_colorTexture;

public:
  static std::shared_ptr<Shader> defaultMeshShader;
};

// TODO: Move this to a asset manager
std::shared_ptr<ArrayMesh> LoadModel(const std::string& filepath);
} // namespace OGL
