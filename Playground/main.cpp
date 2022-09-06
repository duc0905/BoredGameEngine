#include "pch.h"
#include <GLFW/glfw3.h>
#include "Renderer/ShaderClass.h"

const int WIDTH = 800;
const int HEIGHT = 600;

float positions[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.0f, 0.5f
};

unsigned int indices[] = { 0, 1, 2 };

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

	unsigned int VAO, VBO, IBO;

	// gen vertex array, attach the I of the vertex array to VAO
	glGenVertexArrays(1, &VAO);
	// tell opengl to select the vertex array with ID VAO
	glBindVertexArray(VAO);

	// gen buffer - create buffer and attach the ID of the buffer to VBO
	glGenBuffers(1, &VBO);
	// tell open gl to use the array buffer with the ID VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Buffer data - push data into the bound array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// High level opengl
	glEnableVertexArrayAttrib(VAO, 0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// see VBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	Shader normalShader("normal.vert", "normal.frag");
	normalShader.Activate();

	while (!glfwWindowShouldClose(window))
    {
		/* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.1f, 0.4f, 0.3f, 1.0f);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
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
