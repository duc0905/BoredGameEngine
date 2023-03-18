#pragma once
#include "pch.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define USE_GAME_MODULES     \
  using Bored::renderer;     \
  using Bored::audio;        \
  using Bored::input;        \
  using Bored::actorManager; \
  using Bored::gamemode;     \
  using Bored::addons;

#include "Actor.h"
#include "ActorManager.h"


// Just define the general interfaces here
namespace Bored {
/**
 * @brief Component only contains data. No external methods, except for OnTick!
*/
struct Component : public Tickable {
  virtual const char* GetName() = 0;
};

/**
 * @brief Holds the logics of the game
*/
class Gamemode : public Module { };

/**
 * @brief Provide chrono functionalities
*/
class Timer : public Module {
  std::chrono::system_clock::time_point last_frame;

 public:
  virtual void OnSetup() override {
    last_frame = std::chrono::system_clock::now();
  }

  double GetDt() const
  {
    return std::chrono::duration<double>(std::chrono::system_clock::now() -
                                              last_frame)
                    .count();
  }

  virtual bool OnTick(double) override {
    last_frame = std::chrono::system_clock::now();
    return false;
  }
};

// Output classes - depends on the device. Implement these for each platform.
class Renderer : public Module {
 protected:
  std::shared_ptr<Actor> active_cam;
  std::vector<std::shared_ptr<Actor>> light_srcs;
 public:
  unsigned int width = 1200, height = 800;
}; 
class Audio : public Module {};
class Input : public Module {};

/* ===================== VARIABLES ==================== */
std::string title = "Bored game";
std::shared_ptr<Renderer> renderer;
std::shared_ptr<Audio> audio;
std::shared_ptr<Input> input;

std::shared_ptr<ActorManager> actorManager = std::make_shared<ActorManager>();
std::shared_ptr<Gamemode> gamemode;
std::shared_ptr<Timer> timer = std::make_shared<Timer>();

std::vector<std::shared_ptr<Module>> addons;

/* ====================== FUNCTIONNS =====================*/
/**
 * @brief Run the game
*/
void Run() {
  timer->OnSetup();
  actorManager->OnSetup();
  gamemode->OnSetup();
  renderer->OnSetup();
  input->OnSetup();
  audio->OnSetup();
  for (auto mod : addons) mod->OnSetup();

  bool should_close = false;
  while (!should_close) {
    const double dt = timer->GetDt();

    // Poll input
    should_close = input->OnTick(dt) || should_close;
    
    // Logic - backend

    should_close = timer->OnTick(dt) || should_close;
    should_close = actorManager->OnTick(dt) || should_close;
    should_close = gamemode->OnTick(dt) || should_close;
    for (auto mod : addons) should_close = mod->OnTick(dt) || should_close;

    // Output - frontend
    should_close = renderer->OnTick(dt) || should_close;
    should_close = audio->OnTick(dt) || should_close;
  }
}

/**
 * @brief Stop the game. Call the OnStop method of all modules.
 */
void Stop() {
  for (auto mod : addons) mod->OnStop();
  renderer->OnStop();
  input->OnStop();
  audio->OnStop();
  gamemode->OnStop();
  actorManager->OnStop();
  timer->OnStop();
}

}  // namespace Bored

