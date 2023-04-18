// Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core.h"
#include "Tests.h"

#define TestFuncHead std::cout << "=========" << __func__ << "=========" << std::endl;
#define TestFuncSuccess std::cout << "====" << __func__ << ": Success ====" << std::endl;

void LoadModelTest() {
	TestFuncHead;
	Bored::Render::Model m = Bored::Render::Load("./res/pawn.gltf");

	// Expect m has 1 mesh
	if (m.renderables.size() != 1) {
		std::cout << "[Failed]: Expect model has 1 mesh, but got: " << m.renderables.size() << std::endl;
		return;
        }

	// Expect m has 24 indices
	if (m.renderables[0].first->indices.size() != 36) {
		std::cout << "[Failed]: Expect model has 36 indices, but got: " << m.renderables[0].first->indices.size() << std::endl;
		return;
	}

	TestFuncSuccess;
}

void MaterialFactoryTest() {
	TestFuncHead;

	using Bored::Render::MaterialFactory;
	using Bored::Render::Material;
	

	std::string name = "non-exist.mat";

	// Load non-existed
	std::shared_ptr<Material> m = MaterialFactory::Load(name);
	if (m->shininess != 0.0f || m->reflectivity != 1.0f) {
		std::cout << "[Failed]: Expect creating defaut material, but not: " << name << std::endl;
		return;
	}

	m = MaterialFactory::Load(name, 1.0f, 0.5f);
	if (!m || m->shininess != 1.0f || m->reflectivity != 0.5f) {
		std::cout << "[Failed]: Fail to create new material: " << name << std::endl;
		return;
	}

	m = MaterialFactory::Load(name);
	if (!m || m->shininess != 1.0f || m->reflectivity != 0.5f) {
		std::cout << "[Failed]: Fail to find created material: " << name << std::endl;
		return;
	}

	std::shared_ptr<Material> m2 = MaterialFactory::Load(*m);
	if (m2 == nullptr || m2->shininess != 1.0f || m2->reflectivity != 0.5f) {
		std::cout << "[Failed]: Fail to load material with created material: " << name << std::endl;    
		return;
	}

	if (m != m2) {
		std::cout << "[Failed]: Expect previous material to exist in the storage, so the return material should be the same as the previous: " << name << std::endl;
		return;
	}

	TestFuncSuccess;
}
void TextureFactoryTest() {
	TestFuncHead;

	using Bored::Render::TextureFactory;
	using Bored::Render::Texture;

	std::string name = "black";
	std::string fileNE = "./res/non-exist.png";
	std::string file = "./res/tex.png";

	// Load non-exist
	std::shared_ptr<Texture> m = TextureFactory::Load(fileNE);
	if (!m || m->height != 1 || m->width != 1) {
		std::cout << "[Failed]: Expect creating defaut texture, but not: " << fileNE << std::endl;
		return;
	}

	unsigned char black[16];
	memset(black, 0, sizeof(unsigned char) * 16);
	m = TextureFactory::Load(name, 2, 2, black);
	if (!m || m->height != 2 || m->width != 2) {
		std::cout << "[Failed]: Fail to create new texture: " << name << std::endl;
		return;
	}

	m = TextureFactory::Load(name);
	if (!m || m->height != 2 || m->width != 2) {
		std::cout << "[Failed]: Fail to find created material: " << name << std::endl;
		return;
	}

	std::shared_ptr<Texture> m2 = TextureFactory::Load(*m);
	if (m2 == nullptr || m2->height != 2 || m2->width != 2) {
		std::cout << "[Failed]: Fail to load material with created material: " << name << std::endl;    
		return;
	}

	if (m != m2) {
		std::cout << "[Failed]: Expect previous material to exist in the storage, so the return material should be the same as the previous: " << name << std::endl;
		return;
	}

	TestFuncSuccess;
}
void MeshFactoryTest() {
	TestFuncHead;

	using Bored::Render::MeshFactory;
	using Bored::Render::Mesh;

	std::string name = "cube";
	std::string name2 = "cube2";

	// Load non-exist
	std::shared_ptr<Mesh> m = MeshFactory::Load(name);
	if (!m) {
		std::cout << "[Failed]: Expect creating defaut mesh, but not: " << name << std::endl;
		return;
	}

	unsigned char black[16];
	memset(black, 0, sizeof(unsigned char) * 16);
	m = MeshFactory::Load(name2, {{}}, {0, 1, 2});
	if (!m || m->indices.size() != 3 || m->verts.size() != 1) {
		std::cout << "[Failed]: Fail to create new mesh: " << name2 << std::endl;
		return;
	}

	m = MeshFactory::Load(name2);
	if (!m || m->indices.size() != 3 || m->verts.size() != 1) {
		std::cout << "[Failed]: Fail to find created mesh: " << name2 << std::endl;
		return;
	}

	std::shared_ptr<Mesh> m2 = MeshFactory::Load(*m);
	if (m2 == nullptr || m2->indices.size() != 3 || m2->verts.size() != 1) {
		std::cout << "[Failed]: Fail to load material with created mesh: " << name2 << std::endl;    
		return;
	}

	if (m != m2) {
		std::cout << "[Failed]: Expect previous material to exist in the storage, so the return mesh should be the same as the previous: " << name2 << std::endl;
		return;
	}

	TestFuncSuccess;
}

int main()
{
	LoadModelTest();
	MaterialFactoryTest();
	TextureFactoryTest();
	MeshFactoryTest();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
