#include "Core.h"
#include "OALMods/OAL.h"
#include "OGLMods/OGL.h"

USE_GAME_MODULES

struct TransComp : public Bored::Component {
  // Inherited via Component
  virtual const char* GetName() override { return "Transform"; }

  glm::vec3 pos = glm::vec3(0.0f);
  glm::vec3 rotate = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
};

class MyActor : public Bored::Actor {
 public:
  virtual bool OnTick(double dt) override { 
      return Bored::Actor::OnTick((dt));
  }
  virtual void OnImGui() override {
  }
};

class MyGamemode : public Bored::Gamemode {
  std::shared_ptr<Bored::Actor> me;
 public:
  virtual void OnSetup() override {
    me = actorManager->Create<MyActor>();
      std::cout << "Me: " << me << std::endl;
  }
};

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

int main() {
  // Do this for now, will use config file or define directory
  // to select the right module
  Bored::title = "Bored Chess";
  std::shared_ptr<Bored::OGL::Renderer> r =
      std::make_shared<Bored::OGL::Renderer>();
  renderer = r;

  std::shared_ptr<Bored::OGL::Input> i = std::make_shared<Bored::OGL::Input>();
  input = i;
  i->window = r->window;

  audio = std::make_shared<Bored::OAL::Audio>();
  gamemode = std::make_shared<MyGamemode>();
  addons.push_back(std::make_shared<FPS>());

  Bored::Run();
  Bored::Stop();
}
