#pragma once
#include "../pch.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Renderer/Mesh.h"
#include "../Renderer/Texture/OpenGLTexture.h"
#define INVALID_MATERIAL 0xFFFFFFFF;
class MeshBuffers;
struct SubMesh;

//struct SubMesh {
//	SubMesh()
//	{
//		NumIndices = 0;
//		BaseVertex = 0;
//		BaseIndex = 0;
//		MaterialIndex = INVALID_MATERIAL;
//	}
//	unsigned int NumIndices;
//	unsigned int BaseVertex;
//	unsigned int BaseIndex;
//	unsigned int MaterialIndex;
//};
class Helper
{
private:
	Helper() = default;
	// static std::vector<std::string> splitString(const std::string& s, const std::string& delim);
	// static bool converibleToInt(const std::string& s);
	// static bool vsToVi(std::vector<std::string> v, std::vector<int>& ans);
public:
	static std::tuple<std::shared_ptr<MeshBuffers>, std::vector<SubMesh>, std::vector<std::shared_ptr<ITexture>>> ReadModel(const std::string& file);
};

