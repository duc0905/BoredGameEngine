#include "Renderer.h"

/* ========== Renderer =========== */

void Renderer::Init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	else {
		std::cout << "Initialized GLAD" << std::endl;
	}

	std::cout << "OpenGL version: " << (char*)glGetString(GL_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);

	//quadDrawer_.Init();
	//cube_.LoadModel("cube.obj");
	/*std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>();
	cubeMesh->LoadModel("cube.obj");

	auto x = cube_.FindComponent<TransformComponent>();
	x->Scale(glm::vec3(0.5f));
	x->Rotate(glm::vec3(0.0f, glm::pi<float>() / 180 * 30, glm::pi<float>() / 180 * 45));

	auto meshComp = cube_.CreateComponent<MeshComponent>();
	meshComp->UseMesh(cubeMesh);*/

	meshShader_ = Shader("mymesh.vert", "mymesh.frag");
}

void Renderer::Render(World& world)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& actor : world.GetActors())
	{
		Draw(*actor);
	}
}

//void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color)
//{
//	quadDrawer_.Draw(pos, size, glm::vec4(color, 1.0f));
//}

void Renderer::Draw(const Mesh& mesh)
{
	if (!mesh.IsInit())
	{
		std::cout << "Mesh not initialized" << std::endl;
		return;
	}
	mesh.Bind();
	meshShader_.Activate();

	glDrawElements(GL_TRIANGLES, mesh.GetNumIndices(), GL_UNSIGNED_INT, 0);

	mesh.UnBind();
}

void Renderer::Draw(const Actor& actor)
{
	try
	{
		auto meshComp = actor.FindComponent<MeshComponent>();
		Mesh& mesh = meshComp->GetMesh();
		auto transformComp = actor.FindComponent<TransformComponent>();
		auto modelMat = transformComp->GetModelMatrix();

		auto viewMat = activeCam_->GetViewMat();
		auto projMat = activeCam_->GetProjectionMat();

		//glm::mat4 viewMat = glm::lookAt(glm::vec3(-2.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 viewMat = glm::mat4(1.0f);
		//glm::mat4 projMat = glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, 0.0f, 100.0f);

		auto mvp = projMat * viewMat * modelMat;

		meshShader_.Activate();
		meshShader_.SetUniformMatrix4fv("u_ModelMat", glm::value_ptr(modelMat));
		meshShader_.SetUniformMatrix4fv("u_ViewMat", glm::value_ptr(viewMat));
		meshShader_.SetUniformMatrix4fv("u_ProjectionMat", glm::value_ptr(projMat));

		Draw(mesh);
	}
	catch (const std::exception&)
	{
		std::cout << "Cannot draw actor" << std::endl;
	}
}

/* ========== QuadDrawer =========== */
/*
void Renderer::QuadDrawer::Init()
{
	shader_ = Shader("default.vert", "default.frag");

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	va_ = std::make_unique<VertexArray>();
	ib_ = std::make_unique<IndexBuffer>(indices, sizeof(indices), GL_STATIC_DRAW);
	vb_ = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), GL_STATIC_DRAW);
	//vb_->PrintData();
	BufferLayout bl{
		{ "Position", VertLayCompType::Float3, GL_FALSE}};
	vb_->SetLayout(bl);
	va_->AddVertexBuffer(*vb_);

	va_->Unbind();
	vb_->Unbind();
	ib_->Unbind();
}

void Renderer::QuadDrawer::Draw(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
	shader_.Activate();
	va_->Bind();
	vb_->Bind();
	ib_->Bind();

	glm::mat4 Model = glm::mat4(1.0f);
	Model = glm::scale(Model, glm::vec3(size, 1.0f));
	Model = glm::translate(Model, glm::vec3(pos, 0.0f));

	shader_.SetUniformMatrix4fv("u_Model", glm::value_ptr(Model));
	shader_.SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

/* ========== MyMesh =========== */

//void Renderer::MyMesh::Init()
//{
//	//shader_ = Shader("mymesh.vert", "mymesh.frag");
//
//	std::vector<glm::vec3> verts, uvs, normals;
//	std::vector<unsigned int> indices;
//	Helper::ReadOBJModel("donut.obj", verts, uvs, normals, indices);
//
//	va_ = std::make_unique<VertexArray>();
//
//	vertBuffer_ = std::make_unique<VertexBuffer>(&verts[0], verts.size() * sizeof(verts[0]), GL_STATIC_DRAW);
//	BufferLayout vertBl{{ "Position", VertLayCompType::Float3, GL_FALSE }};
//	vertBuffer_->SetLayout(vertBl);
//
//	uvBuffer_ = std::make_unique<VertexBuffer>(&uvs[0], uvs.size() * sizeof(uvs[0]), GL_STATIC_DRAW);
//	BufferLayout uvBl{ { "UV", VertLayCompType::Float3, GL_FALSE } };
//	uvBuffer_->SetLayout(uvBl);
//
//	normBuffer_ = std::make_unique<VertexBuffer>(&normals[0], normals.size() * sizeof(normals[0]), GL_STATIC_DRAW);
//	BufferLayout normalBl{ { "Normal", VertLayCompType::Float3, GL_FALSE } };
//	normBuffer_->SetLayout(normalBl);
//	numIndices_ = indices.size();
//
//	va_->AddVertexBuffer(*vertBuffer_);
//	va_->AddVertexBuffer(*uvBuffer_);
//	va_->AddVertexBuffer(*normBuffer_);
//
//	ib_ = std::make_unique<IndexBuffer>(&indices[0], indices.size() * sizeof(indices[0]), GL_STATIC_DRAW);
//}

//void Renderer::MyMesh::Draw()
//{
//	shader_.Activate();
//	va_->Bind();
//
//	glDrawElements(GL_TRIANGLES, numIndices_, GL_UNSIGNED_INT, 0);
//}
