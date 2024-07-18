#include "Bored.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

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

int main() {
  // Assimp::Importer importer;
  //
  // const aiScene* scene = importer.ReadFile(cube,
  //                                          aiProcess_CalcTangentSpace       |
  //                                          aiProcess_Triangulate            |
  //                                          aiProcess_JoinIdenticalVertices  |
  //                                          aiProcess_SortByPType);
  //
  // if (scene == nullptr) {
  //   std::cerr << importer.GetErrorString() << std::endl;
  //   return 1;
  // }
  //
  // ExploreScene(scene);

  Bored::GLFW::Window* w = Bored::GLFW::Window::GetInstance();
  w->OnSetup();
  w->UseRenderContext(Bored::Render::OGL::Context::GetDefault());
  auto r = w->GetRenderer();
  
  std::shared_ptr<Bored::Render::Camera> camera = std::make_shared<Bored::Render::Camera>();
  std::shared_ptr<Bored::Render::Projector> projector = std::make_shared<Bored::Render::OrthoProjector>(0.0f, 100.0f, 0.0f, 100.0f);

  r.SetCamera(camera);
  r.SetProjector(projector);
  r.SetClearColor({ 0.3f, 0.1f, 0.1f, 1.0f });

  auto cubeModel = r.LoadModel(cube);
  std::shared_ptr<Transform> transform = std::make_shared<Transform>();

  while (w->OnUpdate(1000)) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
    glfwSwapBuffers((GLFWwindow*)w->GetNativeWindow());
    // w->NewFrame();
    // r.DrawModel(cubeModel, transform->GetMat());
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  w->OnShutdown();

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

