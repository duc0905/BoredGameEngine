#include "MazeScene.hpp"

int main() { 
    MazeScene scene(resource_path + "levels/simple.txt");
    scene.BuildScene();
    scene.AfterBuildScene();
    scene.GameLoop();
}
