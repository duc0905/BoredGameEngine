#include "pch.h"
#include <GLFW/glfw3.h>
#include "Renderer/ShaderClass.h"
#include "Renderer/Buffers/VertexArray.h"
#include "Renderer/Buffers/VertexBuffer.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "src/FrameBuffer.h"

const int WIDTH = 800;
const int HEIGHT = 600;
FrameBuffer* fbo;

float positions[] = {
	// Position   // Color
	-0.5f, -0.5f, 1.0f, 0.5f, 1.0f,
	0.5f, -0.5f, 0.0f, 0.1f, 1.0f,
	0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.7f, 0.1f, 1.0f,
	1.0f, 1.0f, 0.2f, 0.1f, 1.0f,
	1.0f, 0.0f, 0.5f, 0.1f, 1.0f,
	-1.0f, 0.0f, 0.0f, 0.1f, 1.0f,
	-1.0f, -1.0f, 0.5f, 0.1f, 1.0f,
	0.0f, -1.0f, 0.5f, 1.0f, 1.0f
};

unsigned int indices[] = { 0, 1, 2 };

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

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
	std::stringstream ss;
	ss << "X: " << x << " | Y: " << y;
	LOG_COLOR(ss.str(), COLOR::BLUE, COLOR::BLACK);

	fbo->Bind();
	glReadBuffer(GL_COLOR_ATTACHMENT1);

	int data;
	glReadPixels((int)x, HEIGHT - (int)y, 1, 1, GL_RED_INTEGER, GL_INT, &data);
	LOG_COLOR(data, COLOR::YELLOW, COLOR::BLACK);
	fbo->Unbind();
}

int main()
{
	if (!glfwInit())
	return -1;

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Playground", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_COLOR("Failed to initialize OpenGL context", COLOR::RED, COLOR::BLACK);
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	// Triangle
	VertexArray vao;
	VertexBuffer vbo(positions, sizeof(positions));
	IndexBuffer ibo(indices, sizeof(indices));
	vbo.SetLayout({ { "Position", Float2, GL_FALSE}, {"Color", Float3, GL_FALSE} });
	vao.AddVertexBuffer(vbo);
	vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();
	// End triangle

	// Screen
	VertexArray screenVao;
	VertexBuffer screenVbo(screenVerts, sizeof(screenVerts));
	IndexBuffer screenIbo(screenIndices, sizeof(screenIndices));
	screenVbo.SetLayout({ {"Position", Float2, GL_FALSE }, { "UV", Float2, GL_FALSE} });
	screenVao.AddVertexBuffer(screenVbo);
	screenVao.Unbind();
	screenVbo.Unbind();
	screenIbo.Unbind();

	fbo = new FrameBuffer();
	fbo->Bind();

	// Define number of Color Attachment to render
	GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	std::shared_ptr<RenderBuffer> rbo = std::make_shared<RenderBuffer>(WIDTH, HEIGHT);
	fbo->AttachRenderBuffer(rbo);
	auto colorbuffer = OpenGLTexture::CreateColorBuffer(WIDTH, HEIGHT, GL_RGB, GL_RGB);
	fbo->AttachColorBuffer(colorbuffer, 0);
	auto idbuffer = OpenGLTexture::CreateColorBuffer(WIDTH, HEIGHT, GL_R32I, GL_RED_INTEGER);
	fbo->AttachColorBuffer(idbuffer, 1);

	glfwSetCursorPosCallback(window, cursorPosCallback);

	if (GLenum e = glGetError())
		LOG_COLOR(e, COLOR::RED, COLOR::BLACK);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG_COLOR("DUCK", COLOR::RED, COLOR::BLACK);
	else
		LOG_COLOR("YAY", COLOR::GREEN, COLOR::WHITE);


	Shader normalShader("normal.vert", "normal.frag");
	Shader screenShader("screen.vert", "screen.frag");

	screenShader.Activate();
	screenShader.SetUniform1i("screenTexture", 0);


	// while loop here

	while (!glfwWindowShouldClose(window))
    {
		if (GLenum e = glGetError())
			LOG_COLOR(e, COLOR::RED, COLOR::BLACK);
			
		/* Render here */

		fbo->Bind();
		//glEnable(GL_DEPTH_TEST);
		fbo->ClearBuffer();
		fbo->Bind();

		normalShader.Activate();
		vao.Bind();
		ibo.Bind();

		normalShader.SetUniform1i("id", 6);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		normalShader.SetUniform1i("id", 9);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		normalShader.SetUniform1i("id", 69);
		glDrawArrays(GL_TRIANGLES, 6, 3);

		fbo->Unbind();
		//glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		screenShader.Activate();
		screenVao.Bind();
		screenIbo.Bind();
		colorbuffer->Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
	}


	glfwTerminate();

    return 0;
}