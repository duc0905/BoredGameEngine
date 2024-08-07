#include "ChessLogic.hpp"
#include <iostream>
// #define DLL_EXPORT

void ChessLogic::OnSetup()
{
    std::string cubeFile = "build/win-deb/examples/Chess/res/cube.gltf";
    std::cout << "Setting up" << std::endl;

    auto am = GetActorManager();
    auto cubeModel = Bored::Render::LoadModel(cubeFile);

    cube1 = am->Create<Bored::Actor>();
    am->AddComponent<Bored::Render::Model>(cube1->id, *cubeModel);

    cube2 = am->Create<Bored::Actor>();
    am->AddComponent<Bored::Render::Model>(cube2->id, *cubeModel);
}

bool ChessLogic::OnUpdate(double dt)
{
    // std::cout << "Hello from ChessLogic!" << std::endl;
    auto am = GetActorManager();
    auto model1 = am->Get<Bored::Render::Model>(cube1->id);
    auto model2 = am->Get<Bored::Render::Model>(cube2->id);

    std::cout << "Actor id: " << (unsigned int)cube1->id << std::endl;
    std::cout << model1.renderables[0].first->getIndices().size() << std::endl;


    std::cout << "Actor id: " << (unsigned int)cube2->id << std::endl;
    std::cout << model2.renderables[0].first->getIndices().size() << std::endl;
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
