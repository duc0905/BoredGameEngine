#include "pch.h"
#include "Mesh.h"

/* ======= Mesh ======== */

Mesh::Mesh(Mesh&& other)
{
    isInit_ = other.isInit_;

    buffers_.swap(other.buffers_);
}

Mesh::Mesh(std::shared_ptr<MeshBuffers> mb, std::shared_ptr<ITexture> tex)
{
    buffers_ = mb;
    texture_ = tex;
}

bool Mesh::LoadModel(const std::string& file)
{
    //auto data = Helper::ReadModel(file);
    auto [buffers, subMeshes, materials] = Helper::ReadModel(file);
    //auto [buffers, texture] = data.at(0);
    isInit_ = true;

    buffers_ = buffers;
    m_Meshes = subMeshes;
    m_Material = materials;
    texture_ = materials[0];

    return true;
}

bool Mesh::Bind(int texSlot) const
{
    buffers_->Bind();
    texture_->Bind(texSlot);
    return true;
}

bool Mesh::Unbind() const
{
    buffers_->Unbind();
    texture_->Unbind();

    return true;
}

void Mesh::Delete()
{
    buffers_.reset();
    texture_.reset();
}

/* ========== MeshBuffers =========== */

MeshBuffers::MeshBuffers()
{
    va_ = std::make_shared<VertexArray>();
}

//MeshBuffers::MeshBuffers(MeshBuffers&& other)
//{
//    numIndices_ = other.numIndices_;
//    va_.swap(other.va_);
//    positions_.swap(other.positions_);
//    uvs_.swap(other.uvs_);
//    normals_.swap(other.normals_);
//    ib_.swap(other.ib_);
//}

bool MeshBuffers::Bind()
{
    va_->Bind();
    positions_->Bind();
    uvs_->Bind();
    normals_->Bind();
    ib_->Bind();

    return true;
}

bool MeshBuffers::Unbind()
{
    va_->Unbind();
    positions_->Unbind();
    uvs_->Unbind();
    normals_->Unbind();
    ib_->Unbind();

    return true;
}
