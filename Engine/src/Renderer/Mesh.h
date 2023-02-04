#pragma once
#include "../pch.h"

#include "../Utils/Helper.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Texture/ITexture.h"
#include "Material/OpenGLMaterial.h"

#define INVALID_MATERIAL 0xFFFFFFFF;

class MeshBuffers
{
public:
	std::shared_ptr<VertexArray> va_;
	std::shared_ptr<VertexBuffer> positions_;
	std::shared_ptr<VertexBuffer> uvs_;
	std::shared_ptr<VertexBuffer> normals_;
	std::shared_ptr<IndexBuffer> ib_;

	unsigned int numIndices_ = 0;

public:
	MeshBuffers();
	//MeshBuffers(const MeshBuffers& other) = default;
	//MeshBuffers(MeshBuffers&& other);

	bool Bind();
	bool Unbind();
};

struct SubMesh {
	SubMesh()
	{
		NumIndices  = 0;
		BaseVertex = 0;
		BaseIndex  = 0;
		MaterialIndex = INVALID_MATERIAL;
	}
	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};

class Mesh
{
private:
	std::shared_ptr<MeshBuffers> buffers_;
	std::shared_ptr<ITexture> texture_;

	bool isInit_ = false;

public: 
	std::vector<std::shared_ptr<ITexture>> m_Material;
	std::vector<SubMesh> m_Meshes;

public:
	Mesh() = default;
	Mesh(Mesh&& other);
	Mesh(std::shared_ptr<MeshBuffers> mb, std::shared_ptr<ITexture> tex);

	bool LoadModel(const std::string& file);
	bool Bind(int texSlot = 0) const;
	bool Unbind() const;

	inline const bool IsInit() const { return isInit_; }

	inline const unsigned int GetNumIndices() const { return buffers_->numIndices_; }

	void Delete();
};

