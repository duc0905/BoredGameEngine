#include "OGL.h"
#include "Render.h"
#include <gl/gl.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>

#include "../Frontend/OGLMesh.hpp"

namespace Bored
{
namespace Render
{
namespace OGL
{

static GLenum GetOpenGLTypeOf(const ComponentType& t)
{
    switch (t)
    {
    case ComponentType::Float:
        return GL_FLOAT;
    case ComponentType::Float2:
        return GL_FLOAT;
    case ComponentType::Float3:
        return GL_FLOAT;
    case ComponentType::Float4:
        return GL_FLOAT;
    case ComponentType::Int:
        return GL_INT;
    case ComponentType::Int2:
        return GL_INT;
    case ComponentType::Int3:
        return GL_INT;
    case ComponentType::Int4:
        return GL_INT;
    case ComponentType::Mat3:
        return GL_FLOAT;
    case ComponentType::Mat4:
        return GL_FLOAT;
    case ComponentType::Bool:
        return GL_BOOL;
    default:
        return 0;
    }
}

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &id);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &id);
    Unbind();
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SubData(const std::vector<char>& p_data)
{
    size = p_data.size();
    std::cout << &p_data << std::endl;
    glNamedBufferSubData(id, 0, size, &p_data);
}

void VertexBuffer::SubData(void* p_data, size_t p_size)
{
    size = p_size;

    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, p_data);
    Unbind();
}

std::vector<char> VertexBuffer::GetData() const
{
    std::vector<char> data(size);

    Bind();
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, data.data());
    Unbind();

    return data;
}

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &id);
}
IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &id);
    Unbind();
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SubData(std::vector<unsigned int>& p_data)
{
    // TODO: Implement
    size = p_data.size();

    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof(unsigned int), p_data.data());
    Unbind();
}

std::vector<char> IndexBuffer::GetData() const
{
    // TODO: Implement
    std::vector<char> data(size * sizeof(unsigned int));

    Bind();
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof(unsigned int), data.data());
    Unbind();

    return data;
}

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &id);
    Unbind();
}

void VertexArray::Bind() const
{
    glBindVertexArray(id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AttachBuffer(Bored::Render::VertexBuffer& vbo)
{
    Bind();
    vbo.Bind();

    auto bl = vbo.GetLayout();
    auto stride = GetStride(bl);
    GLintptr offset = 0;

    for (auto& [_, comp] : bl)
    {
        glEnableVertexAttribArray(m_attribIndex);
        glVertexAttribPointer(m_attribIndex, GetCountOf(comp), GetOpenGLTypeOf(comp), GL_FALSE, stride,
                              (GLvoid*)offset);
        offset += GetSizeOf(comp);

        m_attribIndex++;
    }

    vbo.Unbind();
    Unbind();
}

void VertexArray::AttachBuffer(std::shared_ptr<Bored::Render::VertexBuffer> vbo)
{
    AttachBuffer(*vbo);
}

Texture::Texture() : Render::ITexture(), id(0)
{
    glGenTextures(1, &id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void* Texture::GetId() const
{
    return (void*)(intptr_t)id;
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SubData(unsigned w, unsigned h, unsigned int b, void* d)
{
    _width = w;
    _height = h;
    _bpp = b;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, d);
    Unbind();
}

Texture2D::Texture2D() : Texture()
{
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Unbind();
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id);
}

FrameBuffer::FrameBuffer(int w, int h) : colorBuffer(std::make_shared<Texture2D>())
{
    glGenFramebuffers(1, &id);
    Bind();
    colorBuffer->SubData(w, h, 3, nullptr);
    colorBuffer->Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)(intptr_t)colorBuffer->GetId(),
                           0);
    colorBuffer->Unbind();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &id);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Render::ITexture> FrameBuffer::GetColorTexture()
{
    return colorBuffer;
}

bool FrameBuffer::CheckStatus()
{
    Bind();
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool FrameBuffer::HasDepthTest()
{
    // TODO:
    throw std::exception("Not implemented");
    return false;
}

bool FrameBuffer::HasStencilTest()
{
    // TODO:
    throw std::exception("Not implemented");
    return false;
}

void FrameBuffer::AddRenderBuffer(std::shared_ptr<RenderBuffer> rbo)
{
    // TODO:
    throw std::exception("Not implemented");
}

void FrameBuffer::ResizeBuffers(int w, int h)
{
    if (id != 0)
        colorBuffer->SubData(w, h, 4, nullptr);
}

Context::Context()
{
    fbo = std::make_unique<FrameBuffer>();
}

Context::~Context()
{
}

void Context::DrawVertexArray(std::shared_ptr<Render::VertexArray> vao, std::shared_ptr<Render::ShaderProgram> pipeline)
{
}

Render::FrameBuffer& Context::GetActiveFrameBuffer()
{
    return *fbo;
}

void Context::ClearFrameBuffer(const glm::vec4& color)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color.r, color.g, color.b, color.a);
}

void Context::SetViewport(int l, int b, int r, int t)
{
    fbo->Bind();
    glViewport(l, b, r, t);
    int newWidth = abs(r - l);
    int newHeight = abs(t - b);

    if (newWidth != width && newHeight != height)
    {
        width = newWidth;
        height = newHeight;
        fbo->ResizeBuffers(width, height);
    }
}

FrameBuffer* FrameBuffer::defaultFbo = nullptr;
FrameBuffer::FrameBuffer(int x) : id(0)
{
}

FrameBuffer* FrameBuffer::GetDefault()
{
    if (!defaultFbo)
        defaultFbo = new FrameBuffer();
    return defaultFbo;
}

Context* Context::defaultContext = nullptr;
Context::Context(int x)
{
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    fbo = std::unique_ptr<FrameBuffer>(FrameBuffer::GetDefault());
    // TODO: Setup debug context for opengl

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION);   // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;
}

std::shared_ptr<Render::ITexture> Context::CreateTexture()
{
    // TODO:
    return std::make_shared<Texture2D>();
}

Context* Context::GetDefault()
{
    if (!defaultContext)
        defaultContext = new Context(0);
    return defaultContext;
}

ShaderProgram::ShaderProgram()
{
    m_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);
}

void ShaderProgram::Bind()
{
    glUseProgram(m_id);
}
void ShaderProgram::Unbind()
{
    glUseProgram(0);
}

GLint ShaderProgram::GetLocation(const std::string& p_name)
{
    auto it = m_locations.find(p_name);
    if (it == m_locations.end())
        throw std::exception("Uniform location not found");
    return it->second;
}

void ShaderProgram::SetUniform(const std::string& p_name, int p_value)
{
    glUniform1i(GetLocation(p_name), p_value);
}
void ShaderProgram::SetUniform(const std::string& p_name, int p_v1, int p_v2)
{
    glUniform2i(GetLocation(p_name), p_v1, p_v2);
}
void ShaderProgram::SetUniform(const std::string& p_name, int p_v1, int p_v2, int p_v3)
{
    glUniform3i(GetLocation(p_name), p_v1, p_v2, p_v3);
}
void ShaderProgram::SetUniform(const std::string& p_name, int p_v1, int p_v2, int p_v3, int p_v4)
{
    glUniform4i(GetLocation(p_name), p_v1, p_v2, p_v3, p_v4);
}

void ShaderProgram::SetUniform(const std::string& p_name, unsigned int p_value)
{
    glUniform1ui(GetLocation(p_name), p_value);
}
void ShaderProgram::SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2)
{
    glUniform2ui(GetLocation(p_name), p_v1, p_v2);
}
void ShaderProgram::SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2, unsigned int p_v3)
{
    glUniform3ui(GetLocation(p_name), p_v1, p_v2, p_v3);
}
void ShaderProgram::SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2, unsigned int p_v3,
                               unsigned int p_v4)
{
    glUniform4ui(GetLocation(p_name), p_v1, p_v2, p_v3, p_v4);
}

void ShaderProgram::SetUniform(const std::string& p_name, float p_value)
{
    glUniform1f(GetLocation(p_name), p_value);
}
void ShaderProgram::SetUniform(const std::string& p_name, float p_v1, float p_v2)
{
    glUniform2f(GetLocation(p_name), p_v1, p_v2);
}
void ShaderProgram::SetUniform(const std::string& p_name, float p_v1, float p_v2, float p_v3)
{
    glUniform3f(GetLocation(p_name), p_v1, p_v2, p_v3);
}
void ShaderProgram::SetUniform(const std::string& p_name, float p_v1, float p_v2, float p_v3, float p_v4)
{
    glUniform4f(GetLocation(p_name), p_v1, p_v2, p_v3, p_v4);
}

void ShaderProgram::SetUniform(const std::string& p_name, glm::mat2 p_value)
{
    glUniformMatrix2fv(GetLocation(p_name), 1, GL_FALSE, glm::value_ptr(p_value));
}
void ShaderProgram::SetUniform(const std::string& p_name, glm::mat3 p_value)
{
    glUniformMatrix3fv(GetLocation(p_name), 1, GL_FALSE, glm::value_ptr(p_value));
}
void ShaderProgram::SetUniform(const std::string& p_name, glm::mat4 p_value)
{
    glUniformMatrix4fv(GetLocation(p_name), 1, GL_FALSE, glm::value_ptr(p_value));
}

void ShaderProgram::LoadVertexShaderFile(std::shared_ptr<FileSystem::File> f)
{
    // TODO: implement
}
void ShaderProgram::LoadGeometryShaderFile(std::shared_ptr<FileSystem::File> f)
{
    // TODO: implement
}
void ShaderProgram::LoadFragmentShaderFile(std::shared_ptr<FileSystem::File> f)
{
    // TODO: implement
}

void ShaderProgram::LoadVertexShaderCode(const std::string& code)
{
    std::cout << "Loading vertex shader:\n" << code << std::endl;
    auto src = code.c_str();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &src, NULL);
    glCompileShader(vs);

    GLint compiled;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compiled);

    if (compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vs, 1024, &log_length, message);

        std::cerr << "Failed to compile vertex shader: " << message << std::endl;

        glDeleteShader(vs);
    }

    glAttachShader(m_id, vs);
}

void ShaderProgram::LoadGeometryShaderCode(const std::string& code)
{
    // TODO: implement
    std::cout << "Loading geometry shader:\n" << code << std::endl;
    GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
    auto src = code.c_str();
    int len = strlen(src);
    glShaderSource(gs, 1, &src, &len);
    glCompileShader(gs);

    GLint compiled;
    glGetShaderiv(gs, GL_COMPILE_STATUS, &compiled);

    if (compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(gs, 1024, &log_length, message);

        std::cerr << "Failed to compile geometry shader: " << message << std::endl;

        glDeleteShader(gs);
    }

    glAttachShader(m_id, gs);
}

void ShaderProgram::LoadFragmentShaderCode(const std::string& code)
{
    // TODO: implement
    std::cout << "Loading fragment shader:\n" << code << std::endl;
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    auto src = code.c_str();
    int len = strlen(src);
    glShaderSource(fs, 1, &src, &len);
    glCompileShader(fs);

    GLint compiled;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compiled);

    if (compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fs, 1024, &log_length, message);

        std::cerr << "Failed to compile fragment shader: " << message << std::endl;

        glDeleteShader(fs);
    }

    glAttachShader(m_id, fs);
}

bool ShaderProgram::IsLinked()
{
    return m_isLinked == GL_TRUE;
}

void ShaderProgram::Link()
{
    GLint nShaders = 0;
    glGetProgramiv(m_id, GL_ATTACHED_SHADERS, &nShaders);

    std::cout << "Number of attached shaders: " << nShaders << std::endl;

    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &m_isLinked);

    if (m_isLinked == GL_FALSE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(m_id, 1024, &log_length, message);

        std::cerr << "Failed to link program: " << message << std::endl;
    }
    else
    {
        GLint num_uniforms, num_attribs;
        GLchar name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &num_uniforms);
        glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &num_attribs);

        std::cout << "Number of active uniforms: " << num_uniforms << std::endl;
        std::cout << "Number of active attributes: " << num_attribs << std::endl;

        for (int i = 0; i < num_uniforms; i++)
        {
            glGetActiveUniform(m_id, i, sizeof(name), &length, &size, &type, name);
            m_locations.insert({name, i});
            std::cout << "Uniform '" << name << "' is at location " << i << std::endl;

            // TODO: do something with type and size
        }
        for (int i = 0; i < num_attribs; i++)
        {
            glGetActiveAttrib(m_id, i, sizeof(name), &length, &size, &type, name);
            std::cout << "Attribute '" << name << "' is at location " << i << std::endl;

            // TODO: do something with this
        }
    }
}
void PrintMatrix(glm::mat4 m)
{
    std::cout << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl;
    std::cout << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl;
    std::cout << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl;
    std::cout << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
}

void ShaderProgram::Draw(std::shared_ptr<Render::IMesh> p_mesh, std::shared_ptr<Render::Material> p_material)
{
    // TODO: implement
    std::shared_ptr<Render::OGL::Mesh> mesh = std::dynamic_pointer_cast<Render::OGL::Mesh>(p_mesh);
    if (mesh == nullptr)
    {
        throw std::exception("p_mesh is not Bored::Render::OGL::Mesh");
    }

    Bind();

    glm::mat4 vpMat;
    glGetUniformfv(m_id, GetLocation("VPMatrix"), glm::value_ptr(vpMat));
    PrintMatrix(vpMat);

    mesh->m_vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh->m_ebo.GetSize());
    Unbind();
}
} // namespace OGL
} // namespace Render
} // namespace Bored
