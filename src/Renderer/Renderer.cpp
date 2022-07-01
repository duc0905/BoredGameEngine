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

	quadDrawer_.Init();
}

void Renderer::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	DrawQuad({ 0.0f, -0.5f }, { 1.0f, 1.0f }, { 0.3f, 0.2f, 0.7f });
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color)
{
	quadDrawer_.Draw(pos, size, glm::vec4(color, 1.0f));
}

/* ========== QuadDrawer =========== */

//Shader Renderer::QuadDrawer::shader_ = Shader();
//VertexArray Renderer::QuadDrawer::va_ = VertexArray();
//VertexBuffer Renderer::QuadDrawer::vb_ = VertexBuffer();
//IndexBuffer Renderer::QuadDrawer::ib_ = IndexBuffer();

void Renderer::QuadDrawer::Init()
{
	shader_ = Shader("default.vert", "default.frag");

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
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
		{ VertLayCompType::Float3, "Position", GL_FALSE},
		{ VertLayCompType::Float4, "Color", GL_FALSE} };
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

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
