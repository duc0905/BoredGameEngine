#pragma once
#include "../RenderUtil.hpp"
#include <cstring>

namespace Bored
{
namespace Render
{
namespace CPU
{
class VertexBuffer : public Render::VertexBuffer
{
  public:
    virtual void Bind() const
    {
    }
    virtual void Unbind() const
    {
    }
    virtual void SubData(const std::vector<std::byte>& p_buffer)
    {
        m_buffer = p_buffer;
    }
    virtual void SubData(void* p_data, size_t size)
    {
        m_buffer.resize(size);
        memcpy(m_buffer.data(), p_data, size);
    }

    virtual std::vector<std::byte> GetData() const
    {
        return m_buffer;
    }
    virtual size_t GetSize() const
    {
        return m_buffer.size();
    }

  private:
    std::vector<std::byte> m_buffer;
};

class VertexArray : public Render::VertexArray
{
  public:
    virtual void Bind() const override
    {
    }
    virtual void Unbind() const override
    {
    }

    virtual void AttachBuffer(Bored::Render::VertexBuffer& p_vbo) override
    {
        m_buffers.push_back(&p_vbo);
    }
    virtual void AttachBuffer(std::shared_ptr<Render::VertexBuffer> vbo) override
    {
        m_buffers.push_back(vbo.get());
    }

  private:
    std::vector<Render::VertexBuffer*> m_buffers;
};

class IndexBuffer : public Render::IndexBuffer
{
  public:
    virtual void Bind() const
    {
    }
    virtual void Unbind() const
    {
    }
    virtual void SubData(std::vector<unsigned int>& p_data)
    {
        m_indices = p_data;
    }

    virtual std::vector<unsigned int> GetData() const
    {
        return m_indices;
    }
    virtual size_t GetSize() const
    {
        return m_indices.size();
    }

  private:
    std::vector<unsigned int> m_indices;
};

class Mesh : public IMesh
{
  public:
    Mesh()
    {
        m_pos.SetLayout({{"Pos", Float3}});
        m_uvs.SetLayout({{"UVs", Float2}});
        m_norm.SetLayout({{"Norm", Float3}});

        m_vao.AttachBuffer(m_pos);
        m_vao.AttachBuffer(m_uvs);
        m_vao.AttachBuffer(m_norm);
    }
    Mesh(IMesh& other) : Mesh()
    {
        name = other.name;

        SubPos(other.GetPos());
        SubUVs(other.GetUVs());
        SubNorms(other.GetNorms());

        SubIndices(other.GetIndices());
    }

  public:
    virtual Bored::Render::VertexArray* GetVertexArray() override
    {
        return &m_vao;
    }
    virtual Bored::Render::IndexBuffer* GetIndexBuffer() override
    {
        return &m_ebo;
    }

    virtual std::vector<glm::vec3> GetPos() const override
    {
        std::vector<std::byte> x = m_pos.GetData();
        std::vector<glm::vec3> data(x.size() / sizeof(glm::vec3));
        memcpy(data.data(), x.data(), x.size());
        return data;
    }

    virtual std::vector<glm::vec2> GetUVs() const override
    {
        std::vector<std::byte> x = m_uvs.GetData();
        std::vector<glm::vec2> data(x.size() / sizeof(glm::vec2));
        memcpy(data.data(), x.data(), x.size());
        return data;
    }

    virtual std::vector<glm::vec3> GetNorms() const override
    {
        std::vector<std::byte> x = m_norm.GetData();
        std::vector<glm::vec3> data(x.size() / sizeof(glm::vec3));
        memcpy(data.data(), x.data(), x.size());
        return data;
    }
    virtual std::vector<unsigned int> GetIndices() const override
    {
        return m_ebo.GetData();
    }

    virtual void SubPos(std::vector<glm::vec3> p_pos) override
    {
        size_t size = p_pos.size() * sizeof(glm::vec3);
        m_pos.SubData(p_pos.data(), size);
    }
    virtual void SubUVs(std::vector<glm::vec2> p_uvs) override
    {
        size_t size = p_uvs.size() * sizeof(glm::vec2);
        m_uvs.SubData(p_uvs.data(), size);
    }
    virtual void SubNorms(std::vector<glm::vec3> p_norms) override
    {
        size_t size = p_norms.size() * sizeof(glm::vec3);
        m_norm.SubData(p_norms.data(), size);
    }
    virtual void SubIndices(std::vector<unsigned int> p_indices) override
    {
        m_ebo.SubData(p_indices);
    }

  private:
    VertexArray m_vao;
    IndexBuffer m_ebo;

    VertexBuffer m_pos, m_uvs, m_norm;
};

} // namespace CPU
} // namespace Render
} // namespace Bored
