#include "Bored.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
#include "ChessLogic.hpp"

// void ExploreMeshes(const aiScene* scene, const aiNode* node) {
//   std::cout << "Node: " << node->mName.C_Str() << std::endl;
//   if (node->mNumMeshes > 0) {
//     for (int i = 0; i < node->mNumMeshes; i++) {
//       unsigned int index = node->mMeshes[i];
//       aiMesh* mesh = scene->mMeshes[index];
//       std::cout << "Mesh: " << mesh->mName.C_Str() << std::endl;
//       for (int i = 0; i < mesh->mNumVertices; i++) {
//         std::cout << "Vertex: " << mesh->mVertices[i].x << " "
//                                 << mesh->mVertices[i].y << " "
//                                 << mesh->mVertices[i].z << std::endl;
//
//       }
//
//       for (int i = 0; i < mesh->mNumFaces; i++) {
//         std::cout << "Face: " << mesh->mFaces[i].mIndices[0] << " "
//                               << mesh->mFaces[i].mIndices[1] << " "
//                               << mesh->mFaces[i].mIndices[2] << std::endl;
//       }
//     }
//   }
//
//   for (int i = 0; i < node->mNumChildren; i++)
//     ExploreMeshes(scene, node->mChildren[i]);
// }
//
// void ExploreScene(const aiScene* scene) {
//   std::cout << (unsigned long long)scene << std::endl;
//
//   std::cout << "Name: " << scene->mName.C_Str() << std::endl;
//
//   ExploreMeshes(scene, scene->mRootNode);
// }

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
    std::vector<std::shared_ptr<Bored::Scene>> scenes;
};

int main()
{
    Game g;
    g.window = Bored::GLFW::Window::GetInstance();
    std::shared_ptr<Bored::Scene> s1 = std::make_shared<Bored::Scene>();
    g.activeScene = s1;
    s1->AddModule<ChessLogic>();
    // s1->AddModule<Mod>();

    // Bored::GLFW::Window* w = Bored::GLFW::Window::GetInstance();
    g.window->OnSetup();
    g.window->UseRenderContext(Bored::Render::OGL::Context::GetDefault());
    auto r = g.window->GetRenderer();
    auto s = g.activeScene;

    // TODO: Change camera to be an actor in the scene
    std::shared_ptr<Bored::Render::Camera> camera = std::make_shared<Bored::Render::Camera>();
    std::shared_ptr<Bored::Render::Projector> projector =
        std::make_shared<Bored::Render::OrthoProjector>(0.0f, 100.0f, 0.0f, 100.0f);

    r.SetCamera(camera);
    r.SetProjector(projector);
    r.SetClearColor({0.3f, 0.1f, 0.1f, 1.0f});

    // auto cubeModel = r.LoadModel(cube);
    // std::cout << cubeModel->renderables[0].first->norms.size() << std::endl;
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();

    while (g.window->OnUpdate(1000))
    {
        g.activeScene->OnUpdate(1000);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
        glfwSwapBuffers((GLFWwindow*)g.window->GetNativeWindow());
        // w->NewFrame();

        // r.DrawModel(cubeModel, transform->GetMat());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    g.window->OnShutdown();

    return 0;
}

/* TODO: Try to use ChessLogic.dll in this file */

// int main2() {
//   auto* window = Bored::GLFW::Window::GetInstance();
//
//   window->SetTitle("Bored Chess");
//   window->SetWidth(800);
//   window->SetHeight(800);
//
//   window->OnSetup();
//   window->UseRenderContext(Bored::Render::OGL::Context::GetDefault());
//
//   Bored::GameLogic gl;
//
//   Bored::Util::DLLLoader mod_loader;
//   mod_loader.Load("ChessLogic.dll");
//   auto x = mod_loader.GetIntance<Bored::Module>();
//   gl.AddModule(x);
//
//   bool isRunning = true;
//   double dt = 0;
//   std::chrono::steady_clock::time_point start, end;
//
//   start = std::chrono::steady_clock::now();
//
//   while (isRunning) {
//     end = std::chrono::steady_clock::now();
//     dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//
//     isRunning &= window->OnUpdate(dt);
//     window->DrawContent();
//
//     // isRunning &= logic.OnUpdate(dt);
//     start = end;
//   }
//
//   return 0;
// }
