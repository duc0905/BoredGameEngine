#pragma once
#include "DemoUtils.h"
#include "IGame.h"

class DemoDebugSystem : public System {
 private:
  IRenderer& renderer;
  std::shared_ptr<Context> c;
  IRenderer::Line linex{{-100, 0, 0}, {100, 0, 0}, {1, 0, 0, 1}},
      liney{{0, -100, 0}, {0, 100, 0}, {0, 1, 0, 1}},
      linez{{0, 0, -100}, {0, 0, 100}, {0, 0, 1, 1}};
  std::vector<IRenderer::Line> lines;

 public:
  DemoDebugSystem() : renderer(IGame::GetRenderer()) {
    for (int i = 5; i <= 100; i += 5) {
      lines.push_back({{100, i, 0}, {-100, i, 0}, {0.3, 0, 0, 0.5}});
      lines.push_back({{100, -i, 0}, {-100, -i, 0}, {0.3, 0, 0, 0.5}});
      lines.push_back({{i, 100, 0}, {i, -100, 0}, {0, 0.3, 0, 0.5}});
      lines.push_back({{-i, 100, 0}, {-i, -100, 0}, {0, 0.3, 0, 0.5}});
    }
    for (auto& l : lines) renderer.DrawLine(l);
    renderer.DrawLine(linex);
    renderer.DrawLine(liney);
    renderer.DrawLine(linez);

    c = std::make_shared<Context>();

    auto& input = IGame::GetInput();
    input.AddContext(c);
    c->AddActionMapping(KeyInput::KEY_0, 0, "HideDebug");
    input.BindAction("HideDebug", [&](KeyInput::Action action) -> void {
      if (action == KeyInput::PRESS) {
        linex.hide = true;
        liney.hide = true;
      }
      if (action == KeyInput::RELEASE) {
        linex.hide = false;
        liney.hide = false;
      }
    });
  }

  virtual void OnTick(float dt) override {}
};

void DebugCoordDemo() {
  SYSTEM_ACCESS;

  IGame::CreateCustomSystem<DemoDebugSystem>();
}
