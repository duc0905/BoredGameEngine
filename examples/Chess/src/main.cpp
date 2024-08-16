#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "Bored.hpp"
#include <GLFW/glfw3.h>
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
        auto v = am->Get<Bored::ECS::IDToPtr>();
        am->Create<Bored::Actor>();

        std::cout << "Hi from mod" << std::endl;
        std::cout << "Number of actors: " << v.size() << std::endl;

        return true;
    };
    virtual void OnShutdown() override{};
};

struct Game
{
    Bored::Window* window;
    std::shared_ptr<Bored::Scene> activeScene;
    // std::vector<std::shared_ptr<Bored::Scene>> scenes;

    void SwitchScene(std::shared_ptr<Bored::Scene> p_scene)
    {
        activeScene = p_scene;
        activeScene->OnSwitchScene();
        window->GetRenderer().OnSwitchScene(p_scene);
    }
};

const char* vShaderSrc = R""""(#version 400
in vec3 vp;

uniform mat4 VPMatrix = mat4(1.0);
uniform mat4 ModelMatrix = mat4(1.0);
void main() {
    gl_Position = VPMatrix * ModelMatrix * vec4(vp, 1.0);
})"""";

const char* fShaderSrc = R""""(#version 400
out vec4 color;

void main() {
    color = vec4(1.0, 0.0, 0.0, 1.0);
})"""";

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
    auto& r = g.window->GetRenderer();

    std::shared_ptr<Bored::Render::OGL::ShaderProgram> myShader = std::make_shared<Bored::Render::OGL::ShaderProgram>();
    myShader->LoadVertexShaderCode(vShaderSrc);
    myShader->LoadFragmentShaderCode(fShaderSrc);
    myShader->Link();
    r.UseShaderProgram(myShader);

    glm::vec4 cc = {0.2f, 0.2f, 0.2f, 1.0f};

    s1->OnSetup();
    g.SwitchScene(s1);

    double dt = 0.016f;

    while (g.window->OnUpdate(dt))
    {
        // r.SetClearColor(cc);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        g.activeScene->OnUpdate(dt);
        r.DrawActiveScene();
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(dt * 1000)));
    }

    g.activeScene->OnShutdown();
    g.window->OnShutdown();

    return 0;
}
