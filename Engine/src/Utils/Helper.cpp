#include "../pch.h"
#include "Helper.h"

std::tuple<std::shared_ptr<MeshBuffers>, std::vector<SubMesh>, std::vector<std::shared_ptr<ITexture>>> Helper::ReadModel(const std::string& file)
{	
	Assimp::Importer imp;
	std::vector<glm::vec3> positions, uvs, normals;
	std::vector<unsigned int> indices;
	std::shared_ptr<ITexture> texture = std::make_shared<OpenGLTexture>();
	std::vector<std::shared_ptr<ITexture>> m_Material;
	std::vector<SubMesh> m_Mesh;
	std::vector<std::tuple<std::shared_ptr<MeshBuffers>, std::shared_ptr<ITexture>>> ret;
	unsigned int indexOffset = 0;

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

	m_Mesh.resize(scene->mNumMeshes);
	m_Material.resize(scene->mNumMaterials);
	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	for (unsigned int i = 0; i < m_Mesh.size(); i++) {

		m_Mesh[i].MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
		m_Mesh[i].NumIndices = scene->mMeshes[i]->mNumFaces * 3;
		m_Mesh[i].BaseVertex = NumVertices;
		m_Mesh[i].BaseIndex = NumIndices;

		NumVertices += scene->mMeshes[i]->mNumVertices;
		NumIndices += m_Mesh[i].NumIndices;
	}


	for (unsigned int i {0}; i < scene->mNumMeshes; i++)
	{	
		//if (i != 1) {

		auto& mesh = scene->mMeshes[i];

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
			else {
				uvs.push_back(glm::vec3(0.0f));
			}

			if (mesh->HasNormals())
			{
				auto& normal = mesh->mNormals[k];
				glm::vec3 n { normal.x, normal.y, normal.z };
				normals.push_back(n);
			}
			else {
				normals.push_back(glm::vec3(0.0f));
			}

		}


		for (unsigned int k{ 0 }; k < mesh->mNumFaces; k++)
		{
			auto& face = mesh->mFaces[k];
			assert(face.mNumIndices == 3);
			for (unsigned int j{ 0 }; j < face.mNumIndices; j++)
			{
				//indices.push_back(face.mIndices[j] + indexOffset);
				indices.push_back(face.mIndices[j]);
			}
		}
		
		//indexOffset += mesh->mNumVertices;


		/*if (scene->HasMaterials())
			const auto* material = scene->
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
		}*/
	}

	/*std::string::size_type SlashIndex = file.find_last_of("/");
	std::string Dir;

	if(SlashIndex == string::npos)*/

	for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
		const auto* pMaterial = scene->mMaterials[i];
		m_Material[i] = std::make_shared<OpenGLTexture>();
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string fullpath = "./res/" + std::string(path.data);
				m_Material[i]->Load(fullpath);
			}
			else {
				aiColor3D color(0.f, 0.f, 0.f);
				pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);

				char colorData[4] = { (char)((uint8_t)(color[0] * 255)), (char)((uint8_t)(color[1] * 255)), (char)((uint8_t)(color[2] * 255)), 255 };
				m_Material[i]->Load(1, 1, 4, (void*)colorData);
			}
		}
		else {
			aiColor3D color(0.f, 0.f, 0.f);
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);

			char colorData[4] = { (char)((uint8_t) (color[0] * 255)), (char)((uint8_t)(color[1] * 255)), (char)((uint8_t)(color[2] * 255)), 255};
			m_Material[i]->Load(1, 1, 4, (void*)colorData);
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

	auto tup = std::make_tuple(buffers, m_Mesh, m_Material);

	return tup;
}