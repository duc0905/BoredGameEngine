#pragma once
#include "../pch.h"

#include "../Utils/Helper.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
private:
	std::unique_ptr<VertexArray> va_;
	std::unique_ptr<VertexBuffer> positions_;
	std::unique_ptr<VertexBuffer> uvs_;
	std::unique_ptr<VertexBuffer> normals_;
	std::unique_ptr<IndexBuffer> ib_;
	unsigned int numIndices_ = 0;
	bool isInit_ = false;
public:
	Mesh() = default;

	bool LoadModel(const std::string& file);
	bool Bind() const;
	bool UnBind() const;

	inline const bool IsInit() const { return isInit_; }

	inline const unsigned int GetNumIndices() const { return numIndices_; }

	void Delete();
};

