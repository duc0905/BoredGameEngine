#include "ChessLogic.hpp"
#include <iostream>
// #define DLL_EXPORT

void ChessLogic::OnSetup()
{
    std::string cubeFile = "build/win-deb/examples/Chess/res/cube.gltf";
    std::cout << "Setting up" << std::endl;

    auto am = GetActorManager();
    auto cubeModel = Bored::Render::LoadModel(cubeFile);

    cube = am->Create<Bored::Actor>();
    auto model = am->AddComponent<Bored::Render::Model>(cube->id, *cubeModel);
    std::cout << "In setup: " << model.renderables.size() << std::endl;
}

void ChessLogic::OnSwitchScene()
{
    auto am = GetActorManager();
    auto& model = am->Get<Bored::Render::Model>(cube->id);

    // TODO: Convert into OGL meshes {}
    for (int i = 0; i < model.renderables.size(); i++) {
        auto& mesh = model.renderables[i].second;
    }
}

bool ChessLogic::OnUpdate(double dt)
{
    auto am = GetActorManager();
    auto model = am->Get<Bored::Render::Model>(cube->id);
    std::cout << "In Update: " << model.renderables.size() << std::endl;

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
