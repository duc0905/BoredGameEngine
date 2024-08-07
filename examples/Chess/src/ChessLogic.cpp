#include "ChessLogic.hpp"
#include <iostream>
#define DLL_EXPORT

void ChessLogic::OnSetup()
{
    auto am = GetActorManager();
    auto a = am->Create<Bored::Actor>();
    std::string cube = "build/win-deb/examples/Chess/res/cube.gltf";
    auto cubeModel = r.LoadModel(cube);
    am->AddComponent<Bored::Render::Model>(a->id, cube);
}

bool ChessLogic::OnUpdate(double dt)
{
    std::cout << "Hello from ChessLogic!" << std::endl;
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
