// #include <iostream>
#include "Editor.hpp"

// Main code
int main(int, char**)
{
    Editor& editor = Editor::GetInstance();

    editor.Init();
    editor.Run();
    editor.Shutdown();

    return 0;
}
