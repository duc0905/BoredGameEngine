#pragma once
#include "FileSystem.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Bored {
namespace Render {
enum ComponentType {
  None = 0,
  Bool,
  Float,
  Float2,
  Float3,
  Float4,
  Int,
  Int2,
  Int3,
  Int4,
  Mat3,
  Mat4
};

/**
 * A buffer in the GPU
 * Will not store data in the GPU, so will not store data in here
 **/
class Buffer {
 public:
  virtual ~Buffer() {}

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  /* *
   *
   * */
  virtual std::vector<char> GetData() = 0;
};

/**
 * Vertex buffer :)
 **/
class VertexBuffer : public Buffer {
 public:
  typedef std::map<std::string, ComponentType> BufferLayout;

 public:
  virtual ~VertexBuffer() {}
  /* *
   * Get buffer's layout
   * */
  BufferLayout GetLayout() const { return layout; }

  /* *
   * Substitute data inside the buffer with new data
   * */
  virtual void SubData(std::vector<char>, BufferLayout) = 0;

 protected:
  /* *
   * Layout of the data inside the buffer
   * */
  BufferLayout layout;
};

/**
 * Buffer of indices to render
 **/
class IndexBuffer : public Buffer {
 public:
  virtual ~IndexBuffer() {}

  virtual void SubData(std::vector<unsigned int>) = 0;
};

/* *
 * Vertex Array :)
 * Always have an index buffer by default
 * */
class VertexArray {
 public:
  virtual ~VertexArray() {}

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

 protected:
  std::unique_ptr<IndexBuffer> ibo;
};

class Attachment {
 public:
  virtual bool CheckStatus() = 0;
};

/**
 * TODO
 **/
class RenderBuffer : public Attachment {};

/* *
 * TODO
 * */
class DepthBuffer : public Attachment {};

/* *
 * TODO
 * */
class StencilBuffer : public Attachment {};

/**
 * TODO
 **/
class ColorBuffer : public Attachment {};

/**
 * TODO
 **/
class FrameBuffer : public Buffer {
  virtual bool CheckStatus() = 0;
};

class Shader {
 public:
  virtual bool IsComplete() = 0;
};
class VertexShader : public Shader {
 public:
  virtual ~VertexShader() {}
};
class GeometryShader : public Shader {
 public:
  virtual ~GeometryShader() {}
};
class FragmentShader : public Shader {
 public:
  virtual ~FragmentShader() {}
};

/**
 * Also known as Shader program
 **/
class ShaderPipeline {
 public:
  virtual ~ShaderPipeline() {}

  virtual void Bind() = 0;
  virtual void Unbind() = 0;
  virtual void SetUniform(const std::string& name, int value) = 0;
  virtual bool IsComplete() = 0;

  virtual void LoadVertexShaderFile(std::shared_ptr<FileSystem::File> f) = 0;
  virtual void LoadGeometryShaderFile(std::shared_ptr<FileSystem::File> f) = 0;
  virtual void LoadFragmentShaderFile(std::shared_ptr<FileSystem::File> f) = 0;

  virtual void LoadVertexShaderCode(const std::string& code) = 0;
  virtual void LoadGeometryShaderCode(const std::string& code) = 0;
  virtual void LoadFragmentShaderCode(const std::string& code) = 0;

 protected:
  std::unique_ptr<VertexShader> vShader;
  std::unique_ptr<GeometryShader> gShader;
  std::unique_ptr<FragmentShader> fShader;
};

class Texture {
 public:
  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;
  // TODO
  virtual void SubData() const = 0;
};

class Texture2D : public Texture {};
class Texture3D : public Texture {};
class Cubemap : public Texture {};

class Context {
 public:
  virtual ~Context() {}

  virtual bool OnTick(double dt) = 0;

  virtual void DrawVertexArray(std::shared_ptr<VertexArray> vao,
                               std::shared_ptr<ShaderPipeline> pipeline) = 0;
};
}  // namespace Render
}  // namespace Bored
