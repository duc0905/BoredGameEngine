#pragma once
#include "Bored.hpp"

class ChessLogic : public Bored::Module
{
  public:
    void OnSetup() override;
    bool OnUpdate(double dt) override;
    void OnShutdown() override;

  public:
    void HandleClick();
};

// int main() {
//   // the Backend
//   Bored::GameLogic logic;
//   auto myLogic = std::make_shared<ChessLogic>();
//   logic.AddModule(myLogic);

//   // the Frontend
//   Bored::GLFW::Window* window = Bored::GLFW::Window::GetInstance();

//   window->SetHeight(1080);
//   window->SetWidth(1920);
//   window->SetTitle("Chess");
//   window->OnSetup();
//   window->UseRenderContext(Bored::Render::OGL::Context::GetDefault());
//   window->GetRenderer().SetClearColor({0.7f, 0.3f, 0.4f});

//   // auto input = window->GetInput();
//   //
//   // TODO: Setup key mapping
//   // Read from file
//   // auto ingameContext = std::make_shared<Bored::Frontend::Input::Context>();
//   // ingameContext->AddActionMapping(Bored::Frontend::Key::KEY_MB_1, 0, "Click");
//   // input->AddContext(ingameContext);
//   //
//   // TODO: Set up action handlers
//   // input->BindAction("Click", [=](Bored::Frontend::Action action) {
//   //   if (action == Bored::Frontend::PRESS) {
//   //     myLogic->HandleClick();
//   //   }
//   // });

//   bool isRunning = true;
//   double dt = 0;
//   std::chrono::steady_clock::time_point start, end;

//   start = std::chrono::steady_clock::now();

//   while (isRunning) {
//     end = std::chrono::steady_clock::now();
//     dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

//     isRunning &= window->OnUpdate(dt);
//     window->DrawContent();

//     isRunning &= logic.OnUpdate(dt);
//     start = end;
//   }

//   window->OnShutdown();
//   return 0;
// }
