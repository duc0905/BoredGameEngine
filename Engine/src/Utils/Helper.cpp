#include "../pch.h"
#include "Helper.h"

std::vector<std::tuple<std::shared_ptr<MeshBuffers>, std::shared_ptr<ITexture>>> Helper::ReadModel(const std::string& file)
{	
	Assimp::Importer imp;
	std::vector<std::tuple<std::shared_ptr<MeshBuffers>, std::shared_ptr<ITexture>>> ret;

	const aiScene* scene = imp.ReadFile("./res/" + file,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
	{
		LOG_COLOR("Cannot Read file " + file, COLOR::RED, COLOR::BLACK);
		return {};
	}


	for (unsigned int i {0}; i < scene->mNumMeshes; i++)
	{
		auto& mesh = scene->mMeshes[i];
		std::shared_ptr<ITexture> texture = std::make_shared<OpenGLTexture>();
		std::vector<glm::vec3> positions, uvs, normals;
		std::vector<unsigned int> indices;

		for (unsigned int k{ 0 }; k < mesh->mNumVertices; k++)
		{
			auto& vert = mesh->mVertices[k];
			glm::vec3 v { vert.x, vert.y, vert.z };
			positions.push_back(v);


			if (mesh->HasTextureCoords(0))
			{
				auto& uv = mesh->mTextureCoords[0][k];
				glm::vec3 u { uv.x, uv.y, uv.z };
				uvs.push_back(u);
			}

			if (mesh->HasNormals())
			{
				auto& normal = mesh->mNormals[k];
				glm::vec3 n { normal.x, normal.y, normal.z };
				normals.push_back(n);
			}

		}

		for (unsigned int k{ 0 }; k < mesh->mNumFaces; k++)
		{
			auto& face = mesh->mFaces[k];
			for (unsigned int j{ 0 }; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		if (scene->HasMaterials())
		{
			aiString path;
			auto mat = scene->mMaterials[mesh->mMaterialIndex];

			if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string fullpath = "./res/" + std::string(path.data);
				texture->Load(fullpath);
			}
			else {
				char white[4] = { 255, 255, 255, 255 };
				texture->Load(1, 1, 4, (void*)white);
			}
		}

		std::shared_ptr<MeshBuffers> buffers = std::make_shared<MeshBuffers>();

		buffers->positions_ = std::make_unique<VertexBuffer>(&positions[0], positions.size() * sizeof(positions[0]), GL_STATIC_DRAW);
		buffers->positions_->SetLayout({ { "Position", Float3, false} });

		buffers->uvs_ = std::make_unique<VertexBuffer>(&uvs[0], uvs.size() * sizeof(uvs[0]), GL_STATIC_DRAW);
		buffers->uvs_->SetLayout({ { "UV", Float3, false} });

		buffers->normals_ = std::make_unique<VertexBuffer>(&normals[0], normals.size() * sizeof(normals[0]), GL_STATIC_DRAW);
		buffers->normals_->SetLayout({ { "Normal", Float3, false} });

		buffers->ib_ = std::make_unique<IndexBuffer>(&indices[0], indices.size() * sizeof(indices[0]), GL_STATIC_DRAW);
		buffers->numIndices_ = indices.size();

		buffers->va_->AddVertexBuffer(*buffers->positions_);
		buffers->va_->AddVertexBuffer(*buffers->uvs_);
		buffers->va_->AddVertexBuffer(*buffers->normals_);

		buffers->ib_ = std::make_unique<IndexBuffer>(&indices[0], indices.size() * sizeof(indices[0]), GL_STATIC_DRAW);

		auto tup = std::make_tuple(buffers, texture);
		ret.push_back(tup);
	}

	return ret;
}