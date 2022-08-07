#include "Mesh.h"

bool Mesh::LoadModel(const std::string& file)
{
    std::cout << "Loading model: " << file << std::endl;
    std::vector<glm::vec3> pos, uvs, nos;
    std::vector<unsigned int> indices;
    if (!Helper::ReadOBJModel(file, pos, uvs, nos, indices))
        return false;

    va_ = std::make_unique<VertexArray>();

    positions_ = std::make_unique<VertexBuffer>(&pos[0], pos.size() * sizeof(pos[0]), GL_STATIC_DRAW);
    BufferLayout posBl{ { "Position", Float3, false} };
    positions_->SetLayout(posBl);

    uvs_ = std::make_unique<VertexBuffer>(&uvs[0], uvs.size() * sizeof(uvs[0]), GL_STATIC_DRAW);
    BufferLayout uvBl{ { "UV", Float3, false} };
    uvs_->SetLayout(uvBl);

    normals_ = std::make_unique<VertexBuffer>(&nos[0], nos.size() * sizeof(nos[0]), GL_STATIC_DRAW);
    BufferLayout normalBl{ { "Normal", Float3, false} };
    normals_->SetLayout(normalBl);

    ib_ = std::make_unique<IndexBuffer>(&indices[0], indices.size() * sizeof(indices[0]), GL_STATIC_DRAW);
    numIndices_ = indices.size();

    va_->AddVertexBuffer(*positions_);
    va_->AddVertexBuffer(*uvs_);
    va_->AddVertexBuffer(*normals_);

    UnBind();
    isInit_ = true;
    return true;
}

bool Mesh::Bind() const
{
    va_->Bind();
    ib_->Bind();
    return true;
}

bool Mesh::UnBind() const
{
    va_->Unbind();
    ib_->Unbind();
    positions_->Unbind();
    uvs_->Unbind();
    normals_->Unbind();
    return true;
}

void Mesh::Delete()
{
    va_->Delete();
    positions_->Delete();
    uvs_->Delete();
    normals_->Delete();
    ib_->Delete();
}
