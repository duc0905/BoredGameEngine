#include "../pch.h"
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

	for (auto& actor : world.GetActors())
	{
		Draw(*actor);
	}
}

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
