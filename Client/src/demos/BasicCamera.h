#pragma once
#include "DemoUtils.h"
#include "HUD/HUDComponents/ImGuiTextEl.h"
#include "IGame.h"

std::shared_ptr<PerspectiveCamera> cam;
std::shared_ptr<Context> cameraContext = std::make_shared<Context>();
std::shared_ptr<Context> controlContext = std::make_shared<Context>();
float prevX = 0.0f, prevY = 0.0f;

void BasicCameraDemo() {
  SYSTEM_ACCESS

  GLFWWindow* w = dynamic_cast<GLFWWindow*>(&IGame::GetWindow());

  unsigned char pixels[16 * 16 * 4];
  memset(pixels, 0xff, sizeof(pixels));

  GLFWimage image;
  image.width = 16;
  image.height = 16;
  image.pixels = pixels;

  GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
  glfwSetCursor(w->GetGLFWWindow(), cursor);
  glfwSetInputMode(w->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  cam = std::make_shared<PerspectiveCamera>(glm::vec4{70.0f, 1.0f, 0.5f, 700.0f},
                                            glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f});
  world.AddActor(cam);
  renderer.UseCamera(cam);

  cameraContext->AddRangeMapping(KeyInput::KEY_W, 0, "WS", 1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_S, 0, "WS", -1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_D, 0, "AD", -1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_A, 0, "AD", 1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_LEFT_SHIFT, 0, "Space_Ctrl",
                                 -1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_SPACE, 0, "Space_Ctrl", 1.f);
  cameraContext->AddRangeMapping(KeyInput::MOUSE_POS_X, 0, "CAM_HOR", 1.0f);
  cameraContext->AddRangeMapping(KeyInput::MOUSE_POS_Y, 0, "CAM_VERT", 1.0f);

  controlContext->AddActionMapping(KeyInput::KEY_LEFT_CONTROL, 0,
                                   "TOGGLE_CURSOR");
  controlContext->AddActionMapping(KeyInput::KEY_LEFT_CONTROL, KeyInput::CTRL,
                                   "TOGGLE_CURSOR");

  input.AddContext(controlContext);
  input.AddContext(cameraContext);

  input.BindRange("WS", [&](KeyInput::Action action, float val) -> void {
    auto direction = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
    auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
    cam->FindComponent<TransformComponent>()->Translate(
        glm::normalize(direction) * val);
  });

  input.BindRange("AD", [&](KeyInput::Action action, float val) -> void {
    auto direction = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
    auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
    auto cross = glm::cross(up, direction);
    cam->FindComponent<TransformComponent>()->Translate(glm::normalize(cross) *
                                                        val);
  });
  input.BindRange(
      "Space_Ctrl", [&](KeyInput::Action action, float val) -> void {
        auto direction =
            cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
        auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
        cam->FindComponent<TransformComponent>()->Translate(glm::normalize(up) *
                                                            val);
      });
  input.BindRange("CAM_HOR", [&](KeyInput::Action a, float val) -> void {
    if (prevX == 0.0f) {
      prevX = val;
      return;
    }
    auto camComp = cam->FindComponent<PerspectiveCameraComponent>();
    float dx = (val - prevX) * 0.01f;
    prevX = val;
    camComp->AddPitch(-dx);
    if (camComp->pitch > 2*glm::pi<float>()) camComp->pitch -= 2*glm::pi<float>();
    if (camComp->pitch < -2*glm::pi<float>()) camComp->pitch += 2*glm::pi<float>();
  });

  input.BindRange("CAM_VERT", [&](KeyInput::Action a, float val) -> void {
    if (prevY == 0.0f) {
      prevY = val;
      return;
    }
    auto camComp = cam->FindComponent<PerspectiveCameraComponent>();
    float dy = (val - prevY) * 0.01f;
    prevY = val;

    if (camComp->yaw - dy >= glm::pi<float>() / 2 - 0.01f) return;
    if (camComp->yaw - dy <= -glm::pi<float>() / 2 + 0.01f) return;
    camComp->AddYaw(-dy);
  });

  input.BindAction("TOGGLE_CURSOR", [&](KeyInput::Action a) -> void {
    if (a == KeyInput::REPEAT) return;
    /*if (a == KeyInput::PRESS)
      input.EnableCursor();
    else if (a == KeyInput::RELEASE) {
      auto pos = input.GetMousePosition();
      prevX = pos.first;
      prevY = pos.second;
      input.DisableCursor();
      input.ActivateContext(cameraContext);
    }*/
    if (a == KeyInput::PRESS) {
        if (input.isContextActivate(cameraContext)) {
            input.DeactivateContext(cameraContext);
            input.EnableCursor();
        }
        else {
            input.ActivateContext(cameraContext);
            input.DisableCursor();
        }
    } 
  });

  cam->FindComponent<TransformComponent>()->Translate(
      glm::vec3(-10.0f, 0.0f, 2.0f));
}
