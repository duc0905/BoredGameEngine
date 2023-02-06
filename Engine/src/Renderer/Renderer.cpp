#include "pch.h"
#include "Renderer.h"
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

	std::shared_ptr<RenderBuffer> depthBuffer = std::make_shared<RenderBuffer>(window.GetWidth(), window.GetHeight());

	fbo->AttachColorBuffer(colorBuffer, 0);
	fbo->AttachColorBuffer(idBuffer, 1);
	fbo->AttachRenderBuffer(depthBuffer);
	glEnable(GL_DEPTH_TEST);

	meshShader_ = Shader("mymesh.vert", "mymesh.frag");
	screenShader_ = Shader("screen.vert", "screen.frag");
	lineShader_ = Shader("line.vert", "line.frag");

	float screenVerts[] = {
	-1.0f,  1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f
	};

	unsigned int screenIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	screenVao = new VertexArray();
	screenVbo = new VertexBuffer(screenVerts, sizeof(screenVerts));
	screenIbo = new IndexBuffer(screenIndices, sizeof(screenIndices));

	screenVbo->SetLayout({ {"Position", Float2, GL_FALSE }, { "UV", Float2, GL_FALSE} });
	screenVao->AddVertexBuffer(*screenVbo);
	screenVao->Unbind();
	screenVbo->Unbind();
	screenIbo->Unbind();

	lineVao = new VertexArray();
	lineVbo = new VertexBuffer(nullptr, 14 * sizeof(float), GL_DYNAMIC_DRAW);
	lineVbo->SetLayout({ { "Pos", Float3, GL_FALSE }, { "Color", Float4, GL_FALSE } });
	lineVao->AddVertexBuffer(*lineVbo);

	meshShader_.SetUniform3f("u_lightPos", 0.0f, 5.0f, 0.0f);
}

void Renderer::DrawLine(Line& line)
{
	lines_.push_back(&line);
}

void Renderer::DrawLine(const Line& line)
{
	lines_.push_back(new Line(line));
}

void Renderer::Draw(const Line& line)
{
	if (line.hide) return;
	float pos[] = {
		line.from.x, line.from.y, line.from.z, 
		line.colorFrom.r, line.colorFrom.g, line.colorFrom.b, line.colorFrom.a,
		line.to.x, line.to.y, line.to.z,
		line.colorTo.r, line.colorTo.g, line.colorTo.b, line.colorTo.a
	};

	lineShader_.Activate();
	lineVbo->SubData(sizeof(pos), pos);
	lineVao->Bind();
	glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::Render(IWorld& world)
{
	//glEnable(GL_DEPTH_TEST);
	fbo->ClearBuffer();
	if (!activeCam_)
		throw std::exception("Renderer have not been provided a camera");

	auto viewMat = activeCam_->GetViewMat();
	auto projMat = activeCam_->GetProjectionMat();
	auto modelMat = glm::mat4(1.0f);
	auto viewPos = activeCam_->FindComponent<TransformComponent>()->GetTranslation();

	meshShader_.Activate();
	meshShader_.SetUniformMatrix4fv("u_ViewMat", glm::value_ptr(viewMat));
	meshShader_.SetUniformMatrix4fv("u_ProjectionMat", glm::value_ptr(projMat));
	meshShader_.SetUniform3f("u_viewPos", viewPos.x, viewPos.y, viewPos.z);

	fbo->Bind();

	for (auto& actor : world.GetActors())
		Draw(*actor);

	lineShader_.Activate();
	lineShader_.SetUniformMatrix4fv("u_ViewMat", glm::value_ptr(viewMat));
	lineShader_.SetUniformMatrix4fv("u_ProjectionMat", glm::value_ptr(projMat));
	lineShader_.SetUniformMatrix4fv("u_ModelMat", glm::value_ptr(modelMat));

	for (Line* line : lines_)
		Draw(*line);

	fbo->Unbind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenShader_.Activate();
	screenVao->Bind();
	screenIbo->Bind();
	colorBuffer->Bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);




	//glDisable(GL_DEPTH_TEST);
	//fbo->Bind();
	//glClear(GL_COLOR_BUFFER_BIT);
	//colorBuffer->Unbind();
	//screenVao->Unbind();
	//screenVbo->Unbind();
	//screenIbo->Unbind();
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
	for (unsigned int i = 0; i < mesh.m_Meshes.size(); i++) {
		unsigned int materialIndex = mesh.m_Meshes[i].MaterialIndex;
		assert(materialIndex < mesh.m_Material.size());
		if (mesh.m_Material[materialIndex]) {
			mesh.m_Material[materialIndex]->Bind();
		}
		glDrawElementsBaseVertex(GL_TRIANGLES,
			mesh.m_Meshes[i].NumIndices,
			GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * mesh.m_Meshes[i].BaseIndex),
			mesh.m_Meshes[i].BaseVertex);
	}
	//glDrawElements(GL_TRIANGLES, mesh.GetNumIndices(), GL_UNSIGNED_INT, 0);
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
	try {
		auto ambient = actor.FindComponent<AmbientLightComponent>();
		meshShader_.SetUniform3f("u_ambientColor", ambient->color_.r, ambient->color_.g, ambient->color_.b);
		meshShader_.SetUniform1f("u_ambientStrength", ambient->strength_);
	}catch (const std::exception&){}

	try {
		auto diffuse = actor.FindComponent<DiffuseLightComponent>();
		auto transformComp = actor.FindComponent<TransformComponent>();
		meshShader_.SetUniform3f("u_diffuseColor", diffuse->color_.r, diffuse->color_.g, diffuse->color_.b);
		meshShader_.SetUniform3f("u_lightPos", transformComp->translation_.x, transformComp->translation_.y, transformComp->translation_.z);
	}
	catch (const std::exception&) {}

	try {
		auto specular = actor.FindComponent<SpecularLightComponent>();
		auto transformComp = activeCam_->FindComponent<TransformComponent>();
		meshShader_.SetUniform3f("u_specularColor", specular->color_.r, specular->color_.g, specular->color_.b);
		meshShader_.SetUniform3f("u_viewPos", transformComp->translation_.x, transformComp->translation_.y, transformComp->translation_.z);
		meshShader_.SetUniform1f("u_specularStrength", specular->strength_);
	}
	catch (const std::exception&) {}

	try
	{
		auto transformComp = actor.FindComponent<TransformComponent>();
		auto modelMat = transformComp->GetModelMatrix();
		meshShader_.SetUniformMatrix4fv("u_ModelMat", glm::value_ptr(modelMat));

		auto meshComp = actor.FindComponent<MeshComponent>();
		Mesh& mesh = meshComp->GetMesh();

		meshShader_.SetUniform1i("u_ActorID", (int)actor.GetID());
		Draw(mesh);
	}
	catch (const std::exception&)
	{
		//LOG_COLOR(e.what(), COLOR::YELLOW, COLOR::BLACK);
	}
}


