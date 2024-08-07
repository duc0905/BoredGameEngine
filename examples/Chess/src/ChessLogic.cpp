#include "ChessLogic.hpp"
#include <iostream>
#define DLL_EXPORT

void ChessLogic::OnSetup()
{
    std::cout << "ca;;ed" << std::endl;
}

bool ChessLogic::OnUpdate(double dt)
{
    // std::cout << "Hello from ChessLogic!" << std::endl;
    auto am = GetActorManager();
    auto a = am->Create<Bored::Actor>();
    std::string cube = "build/win-deb/examples/Chess/res/cube.gltf";
    auto cubeModel = Bored::Render::LoadModel(cube);
    am->AddComponent<Bored::Render::Model>(a->id, *cubeModel);
    std::cout << cubeModel->renderables[0].first->getIndices().size() << std::endl;
    return true;
}

void ChessLogic::OnShutdown()
{
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
