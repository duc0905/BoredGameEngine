#include "pch.h"
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

	meshShader_ = Shader("mymesh.vert", "mymesh.frag");
}

void Renderer::Render(IWorld& world)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!activeCam_)
		throw std::exception("Renderer have not been provided a camera");

	auto viewMat = activeCam_->GetViewMat();
	auto projMat = activeCam_->GetProjectionMat();

	meshShader_.Activate();
	meshShader_.SetUniformMatrix4fv("u_ViewMat", glm::value_ptr(viewMat));
	meshShader_.SetUniformMatrix4fv("u_ProjectionMat", glm::value_ptr(projMat));

	for (auto& actor : world.GetActors())
	{
		Draw(*actor);
	}
}

void Renderer::Draw(const Mesh& mesh)
{
	if (!mesh.IsInit())
	{
		LOG_COLOR("Mesh not initialized", COLOR::YELLOW, COLOR::BLACK);
		return;
	}
	mesh.Bind();
	meshShader_.Activate();

	glDrawElements(GL_TRIANGLES, mesh.GetNumIndices(), GL_UNSIGNED_INT, 0);

	mesh.Unbind();
}

void Renderer::Draw(const Actor& actor)
{
	try
	{
		auto transformComp = actor.FindComponent<TransformComponent>();
		auto modelMat = transformComp->GetModelMatrix();
		meshShader_.SetUniformMatrix4fv("u_ModelMat", glm::value_ptr(modelMat));

		auto meshComp = actor.FindComponent<MeshComponent>();
		Mesh& mesh = meshComp->GetMesh();

		Draw(mesh);
	}
	catch (const std::exception&)
	{
		//LOG_COLOR(e.what(), COLOR::YELLOW, COLOR::BLACK);
	}
}
