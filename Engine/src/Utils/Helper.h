#pragma once
#include "../pch.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Helper
{
private:
	Helper() = default;
	// static std::vector<std::string> splitString(const std::string& s, const std::string& delim);
	// static bool converibleToInt(const std::string& s);
	// static bool vsToVi(std::vector<std::string> v, std::vector<int>& ans);
public:
	static bool ReadOBJModel(
		const std::string& file,
		std::vector<glm::vec3>& positions,
		std::vector<glm::vec3>& uvs,
		std::vector<glm::vec3>& normals,
		std::vector<unsigned int>& indices);

	//static bool ReadModel(
	//	const std::string& file,
	//	Mesh& mesh,
	//	Texture& tex
	//);
};

