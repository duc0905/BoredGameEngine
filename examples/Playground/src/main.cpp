#include <iostream>
#include <memory>
#include "Adapter/OGL.h"
#include "PlaygroundConfig.h"
#include "Adapter/STDFS.h"

#define PrintMacro(mac) std::cout << "Value of " << #mac << ": " << mac << std::endl;

using namespace Bored::FileSystem::STDFS;

int main() {
  try {
    std::cout << "Hello from playground!" << std::endl;
    std::string test =
        "C:/Users/nguye/Desktop/Random/BoredGameEngine/examples/Playground/src/"
        "tests/helloworld.txt";
    std::shared_ptr<File> file = std::make_shared<File>(test);
    std::string testContent = "hohohohoho";
    file->AppendData(testContent);
  }
  catch (std::exception e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
