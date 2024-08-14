#include "ChessLogic.hpp"
#include <exception>
#include <iostream>

void ChessLogic::OnSetup()
{
    std::string cubeFile = "build/win-deb/examples/Chess/res/cube.gltf";
    std::cout << "Setting up" << std::endl;

    auto am = GetActorManager();
    auto cubeModel = Bored::Render::LoadModel(cubeFile);

    cube = am->Create<Bored::Actor>();
    auto& model = am->AddComponent<Bored::Render::Model>(cube->id, *cubeModel);

    // NOTE: trying to use an actor without camera component as the camera
    auto camera = am->Create<Bored::Actor>();

    // NOTE: It would fail as expected
    try
    {
        m_scene->UseCamera(camera);
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
    }

    auto& cameraComponent = am->AddComponent<Bored::ECS::Camera>(camera->id);
    auto& transformComponent = am->GetOrCreate<Bored::ECS::Transform>(camera->id);

    try
    {
        m_scene->UseCamera(camera);
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
    }

    transformComponent.pos.x = -10.0f;
    cameraComponent.dir = {1.0f, 0.0f, 0.0f};
}

void ChessLogic::OnSwitchScene()
{
    auto am = GetActorManager();
    auto model = am->Get<Bored::Render::Model>(cube->id);

    // TODO: Converting to OGL mesh & materials should be done by a built-in module, not user-created module
    // NOTE: Convert into OGL meshes
    for (int i = 0; i < model->renderables.size(); i++)
    {
        auto mesh = model->renderables[i].first;
        auto pos = mesh->getPos();

        std::cout << "Converting mesh: " << mesh->name << std::endl;
        // TODO: Find a way to use the same OGLMesh for the same IMesh
        std::shared_ptr<Bored::Render::OGLMesh> oglMesh = std::make_shared<Bored::Render::OGLMesh>(*mesh);
        model->renderables[i].first = oglMesh;
    }
}

bool ChessLogic::OnUpdate(double dt)
{
    auto am = GetActorManager();
    auto model = am->Get<Bored::Render::Model>(cube->id);
    std::cout << "In Update: " << model->renderables.size() << std::endl;

    // std::cout << "Actor id: " << (unsigned int)cube->id << std::endl;
    // std::cout << model.renderables[0].first->getIndices().size() << std::endl;

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
