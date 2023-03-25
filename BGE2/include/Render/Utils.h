#pragma once
#include "../pch.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Bored
{
	namespace Render
	{
		struct Texture {
			// Load data o trong factory
			std::string filepath_;
			unsigned int Width, Height, BPP;
			unsigned char* LocalBuffer;

		};

		struct Material {
			std::string name;
			float shininess;
			float reflectiveness;
			std::vector<std::shared_ptr<Texture>> diffuses;
			std::vector<std::shared_ptr<Texture>> speculars;
		};

		struct Vertex {
			std::vector<glm::vec3> pos;
			std::vector<glm::vec2> uv;
			std::vector<glm::vec3> norm;
		};

		struct Mesh {
			std::string name;
			std::vector<Vertex> verts;
			std::vector<unsigned int> indices;
		};

		struct Model {
			typedef std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>> Renderable;
			std::vector<Renderable> renderables;
		};

		class TextureFactory {
		private:
			static std::shared_ptr<Texture> find(const std::string& path);
			TextureFactory() = delete;
			~TextureFactory() = delete;
		public:
			static std::vector<std::shared_ptr<Texture>> storage;
			static std::shared_ptr<Texture> Load(const std::string& path);
			static std::shared_ptr<Texture> Load(
				const std::string& name,
				unsigned int w, unsigned int h,
				unsigned char* data);
		};

		class MaterialFactory {
			static std::vector<std::shared_ptr<Material>> storage;
			static std::shared_ptr<Material> find(const std::string& path);
			static std::shared_ptr<Material> Load(
				const std::string& name, 
				float shininess, float reflectiveness, 
				std::vector<std::shared_ptr<Texture>> diffs = {},
				std::vector<std::shared_ptr<Texture>> specs = {});
			MaterialFactory() = delete;
			~MaterialFactory() = delete;
		};

		class MeshFactory {
		public:
			static std::vector<std::shared_ptr<Mesh>> storage;
			static std::shared_ptr<Mesh> find(const std::string& path);
			static std::shared_ptr<Mesh> Load(const std::string& file);
			static std::shared_ptr<Mesh> Load(
				const std::string& name, 
				std::vector<Vertex> verts, 
				std::vector<unsigned int> indices);
			MeshFactory() = delete;
			~MeshFactory() = delete;
		};
	}
}