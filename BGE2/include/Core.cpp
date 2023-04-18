#include "pch.h"
#include "Core.h"

namespace Bored {
std::string title = "Bored game";
std::shared_ptr<Renderer> renderer;
std::shared_ptr<Audio> audio;
std::shared_ptr<Input> input;

std::shared_ptr<ActorManager> actorManager = std::make_shared<ActorManager>();
std::shared_ptr<Gamemode> gamemode;
std::shared_ptr<Timer> timer = std::make_shared<Timer>();

//std::shared_ptr

std::vector<std::shared_ptr<Module>> addons;

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
  for (auto mod : addons)
    mod->OnSetup();

  bool should_close = false;
  while (!should_close) {
    const double dt = timer->GetDt();

    // Poll input
    should_close = input->OnTick(dt) || should_close;

    // Logic - backend

    should_close = timer->OnTick(dt) || should_close;
    should_close = actorManager->OnTick(dt) || should_close;
    should_close = gamemode->OnTick(dt) || should_close;
    for (auto mod : addons)
      should_close = mod->OnTick(dt) || should_close;

    // Output - frontend
    should_close = renderer->OnTick(dt) || should_close;
    should_close = audio->OnTick(dt) || should_close;
  }
}

/**
 * @brief Stop the game. Call the OnStop method of all modules.
 */
void Stop() {
  for (auto mod : addons)
    mod->OnStop();
  renderer->OnStop();
  input->OnStop();
  audio->OnStop();
  gamemode->OnStop();
  actorManager->OnStop();
  timer->OnStop();
}

void ActorManager::Delete(entt::entity id) { actor_registry.destroy(id); }
bool ActorManager::OnTick(double dt) {
  bool shouldClose = false;
  auto view = actor_registry.view<IDToPtr>();
  for (auto id : view) {
    std::shared_ptr<Actor> a = view.get<IDToPtr>(id);
    if (a->OnTick(dt))
      return true;
  }
  return false;
}
void ActorManager::OnImGui() {
  auto view = actor_registry.view<IDToPtr>();
  for (auto id : view) {
    std::shared_ptr<Actor> a = view.get<IDToPtr>(id);
    a->OnImGui();
  }
}
const glm::mat4 &Renderer::GetProjMat() const { return projector->GetMat(); }

const glm::mat4 Transform::GetMat() const {
  //auto ans = glm::mat4(1.0f);
  auto m = glm::mat4(1.0f);
  //model
  m= glm::translate(glm::mat4(1.0f), pos);
  m= glm::rotate(m, glm::radians(glm::length(rotation)),
                    glm::normalize(rotation));
  m= glm::scale(m, scale);
  return m;
}

void Renderer::UseCamera(std::shared_ptr<Actor> cam)
{
  // TODO check cam component exists
  active_cam = cam;
}

} // namespace Bored
