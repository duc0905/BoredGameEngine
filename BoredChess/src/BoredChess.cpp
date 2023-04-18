#include "bpch.h"
#include "OALMods/Modules.h"
#include "OGLMods/Modules.h"
#include "Pieces.h"
#include "ClassicGameMode.h"

USE_GAME_MODULES

class FPS : public Bored::Module {
private:
  float m_dt;
  // Inherited via Module
  virtual void OnSetup() override {}

  virtual bool OnTick(double dt) {
    m_dt = dt;
    return false;
  }

  virtual void OnImGui() override {
    ImGui::Begin("SPF");
    ImGui::Text("SPF: %.4f s", m_dt);
    ImGui::End();
  }
};

//int main() {
//    Bored::Render::Model m = Bored::Helper::Load("./res/models/bishop.gltf");
//    std::cout << m.renderables.size() << std::endl;
//
//    return 0;
//}

void GetDefaultAssets()
{
  // Load meshes
  // Cube, sphere, ...

  // Load textures
  // Red, green, blue, yellow, magenta, cyan, black, white
  
  // Load materials
  // Metal, wood, ceramic, bronze, silver
}

int main() {
  // Do this for now, will use config file or define directory
  // to select the right module
  Bored::title = "Bored Chess";
  std::shared_ptr<Bored::OGL::Renderer> r =
    std::make_shared<Bored::OGL::Renderer>();
  renderer = r;

  std::shared_ptr<Bored::OGL::Input> i = std::make_shared<Bored::OGL::Input>();
  i->window = r->window;

  input = i;
  input = std::make_shared<Bored::OGL::Input>();

  audio = std::make_shared<Bored::OAL::Audio>();
  gamemode = std::make_shared<ClassicGamemode>();
  addons.push_back(std::make_shared<FPS>());

  GetDefaultAssets();

  Bored::Run();
  Bored::Stop();
}
