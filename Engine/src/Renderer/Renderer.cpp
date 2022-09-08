#include "pch.h"
#include "Renderer.h"
#include "Buffers/FrameBuffer.h"
#include "../IGame.h"

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
	const auto& window = IGame::GetWindow();
	std::cout << "OpenGL version: " << (char*)glGetString(GL_VERSION) << std::endl;
	fbo = new FrameBuffer();
	fbo->Bind();
	GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	colorBuffer = OpenGLTexture::CreateColorBuffer(window.GetWidth(), window.GetHeight(), GL_RGB, GL_RGB);
	idBuffer = OpenGLTexture::CreateColorBuffer(window.GetWidth(), window.GetHeight(), GL_R32I, GL_RED_INTEGER);
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

	fbo->AttachColorBuffer(colorBuffer, 0);
	fbo->AttachColorBuffer(idBuffer, 1);

	for (auto& actor : world.GetActors())
	{
		Draw(*actor);
	}
}

void Renderer::Draw(const Mesh& mesh, unsigned int ID)
{
	if (!mesh.IsInit())
	{
		LOG_COLOR("Mesh not initialized", COLOR::YELLOW, COLOR::BLACK);
		return;
	}
	mesh.Bind();
	meshShader_.Activate();
	meshShader_.SetUniform1i("u_ActorID", (int) ID);
	glDrawElements(GL_TRIANGLES, mesh.GetNumIndices(), GL_UNSIGNED_INT, 0);

	mesh.Unbind();
}

unsigned int Renderer::GetMouseHoverEntityID(int x, int y)
{
	fbo->Bind();
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	const auto& window = IGame::GetWindow();

	int data;
	glReadPixels((int)x, window.GetHeight() - (int)y, 1, 1, GL_RED_INTEGER, GL_INT, &data);
	fbo->Unbind();
	return (unsigned int) data;
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

		Draw(mesh, actor.GetID());
	}
	catch (const std::exception&)
	{
		//LOG_COLOR(e.what(), COLOR::YELLOW, COLOR::BLACK);
	}
}


