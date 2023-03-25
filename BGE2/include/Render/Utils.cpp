#include "pch.h"
#include "Utils.h"

using namespace Bored::Render;
using namespace std;

shared_ptr<Texture> TextureFactory::Load(const string& path)
{
	shared_ptr<Texture> tex = find(path);
	if (tex == nullptr) {
		int w, h, bpp;
		unsigned char* data;

		stbi_set_flip_vertically_on_load(1);
		data = stbi_load(path.c_str(), &w, &h, &bpp, 4);

		if (data)
			stbi_image_free(data);

		else
			std::cout << "Failed to load file: " << path << std::endl;

		shared_ptr<Texture> text = make_shared<Texture>(path, (unsigned int)w, (unsigned int)h, (unsigned int)bpp, data);
		storage.push_back(text);
		return text;
	}
	return tex;
}

std::shared_ptr<Texture> TextureFactory::Load(const std::string& name, unsigned int w, unsigned int h, unsigned char* data)
{
	shared_ptr<Texture> t = find(name);
	if (t == nullptr) {
		t = make_shared<Texture>(name, w, h, 4, data);
		storage.push_back(t);
	}
	return t;
}

shared_ptr<Texture> TextureFactory::find(const std::string& path)
{
	for (auto text : storage)
		if (text->filepath_ == path)
			return text;
	return nullptr;

}


std::shared_ptr<Mesh> MeshFactory::find(const std::string& path)
{
	for (auto mesh : storage)
		if (mesh->name == path)
			return mesh;
	return nullptr;
}

std::shared_ptr<Mesh> MeshFactory::Load(const std::string& file)
{
	std::shared_ptr<Mesh> m = find(file);
	if (!m) 
	{
		Assimp::Importer imp;
		std::vector<glm::vec3> positions, uvs, normals;
		std::vector<unsigned int> indices;
		std::shared_ptr<Texture> texture = TextureFactory::Load(file);
		std::vector<std::shared_ptr<Texture>> m_Material;
		std::vector<Mesh> m_Mesh;
		std::vector<std::tuple<std::shared_ptr<Vertex>, std::shared_ptr<Texture>>> ret;

		const aiScene* scene = imp.ReadFile("./res/" + file,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);
		// TODO
		// Dung assimp de load ik hmmmmmmmmmmmm
	}
	return m;
}

std::shared_ptr<Mesh> MeshFactory::Load(const std::string& name, std::vector<Vertex> verts, std::vector<unsigned int> indices)
{
	std::shared_ptr<Mesh> m = find(name);
	if (!m)
	{
		m = make_shared<Mesh>(name, verts, indices);
		storage.push_back(m);
	}
	return m;


}

std::shared_ptr<Material> MaterialFactory::find(const std::string& path)
{
	for (auto mat : storage)
		if (mat->name == path)
			return mat;
	return nullptr;
}

std::shared_ptr<Material> MaterialFactory::Load(const std::string& name, float shininess, float reflectiveness, std::vector<std::shared_ptr<Texture>> diffs, std::vector<std::shared_ptr<Texture>> specs)
{
	
	shared_ptr<Material> m = find(name);
	if (!m)
	{
		m = make_shared<Material>(name, shininess, reflectiveness);
		storage.push_back(m);
	}
	
	return m;
}
