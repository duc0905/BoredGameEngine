#include "MazeScene.hpp"

int main() { 
    MazeScene scene;
    scene.BuildScene();
    scene.AfterBuildScene();
    scene.GameLoop();
}
