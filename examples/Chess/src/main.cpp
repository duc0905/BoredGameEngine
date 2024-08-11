#include "Bored.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "ChessLogic.hpp"

// NOTE: The path will be relative to the path of the terminal,
// so we should use the path to the project file in conjuncton
// with the file path to get the final path
std::string cube = "build/win-deb/examples/Chess/res/cube.gltf";

class Mod : public Bored::Module
{
  public:
    Mod() = default;
    virtual void OnSetup() override
    {
    }
    virtual bool OnUpdate(double dt) override
    {
        auto am = GetActorManager();
        auto v = am->Get<IDToPtr>();
        am->Create<Bored::Actor>();

        std::cout << "Hi from mod" << std::endl;
        std::cout << "Number of actors: " << v.size() << std::endl;

        return true;
    };
    virtual void OnShutdown() override {};
};

struct Game
{
    Bored::Window* window;
    std::shared_ptr<Bored::Scene> activeScene;
    // std::vector<std::shared_ptr<Bored::Scene>> scenes;

    void SwitchScene(std::shared_ptr<Bored::Scene> p_scene) {
        activeScene = p_scene;
        activeScene->OnSwitchScene();
    }
};

int main()
{
    Game g;
    g.window = Bored::GLFW::Window::GetInstance();
    std::shared_ptr<Bored::Scene> s1 = std::make_shared<Bored::Scene>();
    // g.scenes.push_back(s1);
    s1->AddModule<ChessLogic>();
    // s1->AddModule<Mod>();

    // Bored::GLFW::Window* w = Bored::GLFW::Window::GetInstance();
    g.window->OnSetup();
    g.window->UseRenderContext(Bored::Render::OGL::Context::GetDefault());
    auto r = g.window->GetRenderer();

    // TODO: Change camera to be an actor in the scene
    // std::shared_ptr<Bored::Render::Camera> camera = std::make_shared<Bored::Render::Camera>();
    // std::shared_ptr<Bored::Render::Projector> projector =
    //     std::make_shared<Bored::Render::OrthoProjector>(0.0f, 100.0f, 0.0f, 100.0f);
    //
    // r.SetCamera(camera);
    // r.SetProjector(projector);
    r.SetClearColor({0.3f, 0.1f, 0.1f, 1.0f});

    s1->OnSetup();
    g.SwitchScene(s1);

    while (g.window->OnUpdate(1000))
    {
        g.window->NewFrame();
        g.activeScene->OnUpdate(1000);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
        glfwSwapBuffers((GLFWwindow*)g.window->GetNativeWindow());

        // r.DrawModel(cubeModel, transform->GetMat());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    g.activeScene->OnShutdown();
    g.window->OnShutdown();

    return 0;
}
