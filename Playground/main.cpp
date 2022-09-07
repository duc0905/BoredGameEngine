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
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.0f, 0.5f
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
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	float data;
	glReadPixels((int)x, HEIGHT - (int)y, 1, 1, GL_RED, GL_FLOAT, &data);
	LOG_COLOR(data, COLOR::YELLOW, COLOR::BLACK);
}

// int mainfake()
// {
// 	if (!glfwInit())
// 		return -1;

// 	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Playground", NULL, NULL);
// 	if (!window)
// 	{
// 		glfwTerminate();
// 		return -1;
// 	}
// 	glfwMakeContextCurrent(window);

// 	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
// 		LOG_COLOR("Failed to initialize OpenGL context", COLOR::RED, COLOR::BLACK);
// 		return -1;
// 	}

// 	glViewport(0, 0, WIDTH, HEIGHT);

// 	// Triangle
// 	VertexArray vao;
// 	VertexBuffer vbo(positions, sizeof(positions));
// 	IndexBuffer ibo(indices, sizeof(indices));
// 	vbo.SetLayout({ { "Position", Float2, GL_FALSE} });
// 	vao.AddVertexBuffer(vbo);
// 	vao.Unbind();
// 	vbo.Unbind();
// 	ibo.Unbind();
// 	// End triangle

// 	// Screen
// 	VertexArray screenVao;
// 	VertexBuffer screenVbo(screenVerts, sizeof(screenVerts));
// 	IndexBuffer screenIbo(screenIndices, sizeof(screenIndices));
// 	screenVbo.SetLayout({ {"Position", Float2, GL_FALSE }, { "UV", Float2, GL_FALSE} });
// 	screenVao.AddVertexBuffer(screenVbo);
// 	screenVao.Unbind();
// 	screenVbo.Unbind();
// 	screenIbo.Unbind();
// 	// End screen

// 	glGenFramebuffers(1, &fbo);
// 	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

// 	unsigned int colorBuffer;
// 	glGenTextures(1, &colorBuffer);
// 	glBindTexture(GL_TEXTURE_2D, colorBuffer);
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// 	// Attach texture to framebuffer as COLOR ATTACHMENT
// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
// 	glActiveTexture(GL_TEXTURE0);
// 	if (GLenum e = glGetError())
// 		LOG_COLOR(e, COLOR::RED, COLOR::BLACK);

// 	// Initial render buffer
// 	unsigned int rbo;
// 	glGenRenderbuffers(1, &rbo);
// 	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
// 	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);

// 	// Attach renderbuffer to framebuffer as Depth and Stencil buffers
// 	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
// 	if (GLenum e = glGetError())
// 		LOG_COLOR(e, COLOR::RED, COLOR::BLACK);

// 	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
// 		LOG_COLOR("DUCK", COLOR::RED, COLOR::BLACK);
// 	else
// 		LOG_COLOR("YAY", COLOR::GREEN, COLOR::WHITE);

// 	Shader normalShader("normal.vert", "normal.frag");
// 	Shader screenShader("screen.vert", "screen.frag");

// 	screenShader.Activate();
// 	screenShader.SetUniform1i("screenTexture", 0);

// 	while (!glfwWindowShouldClose(window))
//     {
// 		if (GLenum e = glGetError())
// 			LOG_COLOR(e, COLOR::RED, COLOR::BLACK);

// 		/* Render here */
// 		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
// 		glEnable(GL_DEPTH_TEST);
// 		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 		normalShader.Activate();
// 		vao.Bind();
// 		ibo.Bind();
// 		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

// 		glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 		glDisable(GL_DEPTH_TEST);
// 		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);
// 		screenShader.Activate();
// 		screenVao.Bind();
// 		screenIbo.Bind();
// 		glBindTexture(GL_TEXTURE_2D, colorBuffer);
// 		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

//         /* Swap front and back buffers */
//         glfwSwapBuffers(window);

//         /* Poll for and process events */
//         glfwPollEvents();
// 	}

//     glfwTerminate();
// 	glDeleteFramebuffers(1, &fbo);

//     return 0;
// }

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
	vbo.SetLayout({ { "Position", Float2, GL_FALSE} });
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



	auto colorbuffer = OpenGLTexture::CreateColorBuffer(WIDTH, HEIGHT);
	std::shared_ptr<RenderBuffer> rbo = std::make_shared<RenderBuffer>(WIDTH, HEIGHT);

	fbo = new FrameBuffer();
	fbo->AttachColorBuffer(colorbuffer);
	fbo->AttachRenderBuffer(rbo);

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
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

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