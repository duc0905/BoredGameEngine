#include "ChessLogic.hpp"
#include <exception>
#include <iostream>

void ChessLogic::OnSetup()
{
    std::string cubeFile = "build/win-deb/examples/Chess/res/cube.gltf";
    std::cout << "Setting up" << std::endl;

    auto am = GetActorManager();
    auto cubeModel = Bored::Render::LoadModel(cubeFile);

    // cube = am->Create<Bored::Actor>();
    // auto& model = am->AddComponent<Bored::Render::Model>(cube->id, *cubeModel);

    triangle = am->Create<Bored::Actor>();

    Bored::Render::Model tri;
    auto triMesh = std::make_shared<Bored::Render::OGL::Mesh>();

    triMesh->name = "Triangle Mesh";
    triMesh->subPos({{0.0f, 0.5f, 1.0f}, {-0.5f, -0.5f, 1.0f}, {0.5f, -0.5f, 1.0f}});
    triMesh->subUVs({{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}});
    triMesh->subNorms({{0.1f, 2.3f, 4.5f}, {6.7f, 8.9f, 0.1f}, {2.3f, 4.5f, 6.7f}});
    triMesh->subIndices({0, 1, 2});

    tri.renderables.push_back({triMesh, nullptr});
    am->AddComponent<Bored::Render::Model>(triangle->id, tri);

    auto camera = am->Create<Bored::Actor>();
    auto& cameraComponent = am->AddComponent<Bored::ECS::Camera>(camera->id);
    auto& transformComponent = am->GetOrCreate<Bored::ECS::Transform>(camera->id);

    m_scene->UseCamera(camera);

    transformComponent.pos = {5.0f, -5.0f, 0.0f};
    cameraComponent.up = {0.0f, 1.0f, 0.0f};
    cameraComponent.dir = {0.0f, 0.0f, 1.0f};
}

void ChessLogic::OnSwitchScene()
{
    // auto am = GetActorManager();
    // auto model = am->Get<Bored::Render::Model>(cube->id);

    // TODO: Converting to OGL mesh & materials should be done by a built-in module, not user-created module
    // NOTE: Convert into OGL meshes
    // for (int i = 0; i < model->renderables.size(); i++)
    // {
    //     auto mesh = model->renderables[i].first;
    //     auto pos = mesh->getPos();
    //
    //     std::cout << "Converting mesh: " << mesh->name << std::endl;
    //     // TODO: Find a way to use the same OGLMesh for the same IMesh
    //     // std::shared_ptr<Bored::Render::OGL::Mesh> oglMesh = std::make_shared<Bored::Render::OGL::Mesh>(*mesh);
    //     // model->renderables[i].first = oglMesh;
    // }
}

bool ChessLogic::OnUpdate(double dt)
{
    auto am = GetActorManager();
    auto camera = m_scene->GetActiveCamera();
    auto camTransform = am->Get<Bored::ECS::Transform>(camera->id);

    camTransform->pos.z += dt;

    // camTransform->pos.z += dt;
    // std::cout << "z: " << camTransform->pos.z << std::endl;

    // camTransform->pos.x += dt / 100;

    // auto model = am->Get<Bored::Render::Model>(triangle->id);
    //
    // if (!model)
    // {
    //     std::cout << "Triangle does not have a model" << std::endl;
    // }

    // std::cout << "===========" << std::endl;
    // std::cout << "In Update: " << model->renderables.size() << std::endl;
    // for (auto vert : mesh->getPos())
    // {
    //     std::cout << vert.x << " " << vert.y << " " << vert.z << std::endl;
    // }
    // std::cout << "===========" << std::endl;

    return true;
}

void ChessLogic::OnShutdown()
{
    std::cout << "Shutting down" << std::endl;
}

void ChessLogic::HandleClick()
{
    std::cout << "Clicked" << std::endl;
}

extern "C"
{

    __declspec(dllexport) ChessLogic* CreateInstance()
    {
        return new ChessLogic();
    }

    __declspec(dllexport) void DeleteInstance(ChessLogic* p)
    {
        delete p;
    }
}
