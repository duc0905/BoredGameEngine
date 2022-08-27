#pragma once
#include "../pch.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Renderer/Mesh.h"
#include "../Renderer/Texture/OpenGLTexture.h"

class MeshBuffers;
class Helper
{
private:
	Helper() = default;
	// static std::vector<std::string> splitString(const std::string& s, const std::string& delim);
	// static bool converibleToInt(const std::string& s);
	// static bool vsToVi(std::vector<std::string> v, std::vector<int>& ans);
public:
	static std::vector<std::tuple<std::shared_ptr<MeshBuffers>, std::shared_ptr<ITexture>>> ReadModel( const std::string& file );
};

