#include "ChessLogic.hpp"
#include <exception>
#include <iostream>

void ChessLogic::OnSetup()
{
    std::cout << "Setting up" << std::endl;

    auto am = GetActorManager();
    
    std::string cubeFile = "build/win-deb/examples/Chess/res/cube.gltf";
    auto cubeModel = Bored::Render::LoadModel(cubeFile);
    cube = am->Create<Bored::Actor>();
    auto& model = am->AddComponent<Bored::Render::Model>(cube->id, *cubeModel);

    triangle = am->Create<Bored::Actor>();

    Bored::Render::Model tri;
    auto triMesh = std::make_shared<Bored::Render::OGL::Mesh>();
    auto triTrans = am->Get<Bored::ECS::Transform>(triangle->id);

    triMesh->name = "Triangle Mesh";
    triMesh->SubPos({{0.0f, 0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}});
    triMesh->SubUVs({{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}});
    triMesh->SubNorms({{0.1f, 2.3f, 4.5f}, {6.7f, 8.9f, 0.1f}, {2.3f, 4.5f, 6.7f}});
    triMesh->SubIndices({0, 1, 2});

    triTrans->pos = {2.0f, 0.0f, 0.0f};

    tri.renderables.push_back({triMesh, nullptr});
    am->AddComponent<Bored::Render::Model>(triangle->id, tri);

    auto camera = am->Create<Bored::Actor>();
    auto& cameraComponent = am->AddComponent<Bored::ECS::Camera>(camera->id);
    auto& transformComponent = am->GetOrCreate<Bored::ECS::Transform>(camera->id);

    m_scene->UseCamera(camera);

    transformComponent.pos = {0.0f, 0.0f, 1.0f};
    cameraComponent.up = {0.0f, 1.0f, 0.0f};
    cameraComponent.dir = {0.0f, 0.0f, -1.0f};
}

void ChessLogic::OnSwitchScene()
{
}

bool ChessLogic::OnUpdate(double dt)
{
    auto am = GetActorManager();
    auto triTrans = am->Get<Bored::ECS::Transform>(triangle->id);
    auto cubeTrans = am->Get<Bored::ECS::Transform>(cube->id);

    triTrans->rotation.z += 5.0f;
    triTrans->scale *= 1.01f;

    cubeTrans->rotation.z += 7.5f;
    cubeTrans->rotation.x += 2.5f;

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
