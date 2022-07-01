#include "Window.h"

Window::Window(IGame* game, const unsigned int& width, const unsigned int& height, const std::string& title)
    : System(game), Width_(width), Height_(height), Title_(title)
{
}

Window::~Window()
{
    glfwTerminate();
}

void Window::Init()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        glfwTerminate();
        std::cout << "Cannot initialize glfw" << std::endl;
    }

    /* Create a windowed mode window and its OpenGL context */
    Window_ = glfwCreateWindow(Width_, Height_, Title_.c_str(), NULL, NULL);
    if (!Window_)
    {
        glfwTerminate();
        std::cout << "Cannot create window" << std::endl;
    }
    else {
        std::cout << "Created window" << std::endl;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(Window_);

    /*if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    else {
        std::cout << "Initialized GLAD" << std::endl;
    }

    std::cout << "OpenGL version: " << (char*)glGetString(GL_VERSION) << std::endl;*/
}
