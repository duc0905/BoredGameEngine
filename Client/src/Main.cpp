#include "IGame.h"
#include "Window/GLFWWindow.h"
#include "World/World.h"
#include "Input/GLFWInput.h"
#include "MyRenderer.h"
#include "MyAudio.h"
#include "HUD/ImGuiHUD.h"
//#include "demos/ImGuiHUD.h"

#include "Actor/OrthoCamera.h"
#include "Actor/PerspectiveCamera.h"

#include "TileActor.h"
#include "Pawn.h"
#include "ChessBoardActor.h"
#include "ChessGameMode.h"

int main()
{
  auto window = std::make_shared<GLFWWindow>(800, 800, "Bored Chess");
  IGame::SetWindow(window);

  auto renderer = std::make_shared<Renderer>();
  IGame::SetRenderer(renderer);

  //// My design sucks so this happens
  auto input = GLFWInput::GetInstancePtr();
  IGame::SetInput(input);

  // HUD
  auto hud = std::make_shared<ImGuiHUD>();
  IGame::SetHUD(hud);

  auto world = std::make_shared<World>();
  IGame::SetWorld(world);

  auto audio = std::make_shared<MyAudio>();
  IGame::SetAudio(audio);

  world->UseGameMode<ChessGameMode>(*world);

  //std::shared_ptr<Actor> cube = std::make_shared<ChessBoardActor>();
  std::vector<std::shared_ptr<Actor>> pawnStorage;
  std::vector<std::shared_ptr<Actor>> tileStorage;

  for (int x = 0; x < 8; x++) {
	  for (int y = 0; y < 8; y++) {
		  std::shared_ptr<Actor> tile;
		  if (x % 2 == 0) {
			  if (y % 2 == 0) {
				  tile = std::make_shared<TileActor>(true);
				  tileStorage.push_back(tile);
			  }
			  else {
				  tile = std::make_shared<TileActor>(false);
				  tileStorage.push_back(tile);
			  }
		  }
		  else {
			  if (y % 2 == 0) {
				  tile = std::make_shared<TileActor>(false);
				  tileStorage.push_back(tile);
			  }
			  else {
				  tile = std::make_shared<TileActor>(true);
				  tileStorage.push_back(tile);
			  }
		  }
		  auto transComp = tile->FindComponent<TransformComponent>();
		  transComp->Translate({ -0.5f, x , y});
	  }
  }

  for (int i = 0; i < 8; i++) {
	  std::shared_ptr<Actor> pawn = std::make_shared<Pawn>();
	  pawnStorage.push_back(pawn);
	  auto transComp = pawn->FindComponent<TransformComponent>();
	  transComp->Translate({ -0.5f, i, 3.0f });
  }

  std::shared_ptr<Actor> ambientLight = std::make_shared<Actor>();
  //world->AddActor(cube);
  for (auto p : pawnStorage) {
	  world->AddActor(p);
  }

  for (auto t : tileStorage) {
	  world->AddActor(t);
  }
  world->AddActor(ambientLight);
  auto ligit = ambientLight->CreateComponent<AmbientLightComponent>();
  ligit->color_ = { 1.f, 1.f, 1.f };
  ligit->strength_ = 0.7f;

  //vector<string> 
   //std::shared_ptr<OrthoCamera> cam = std::make_shared<OrthoCamera>(glm::vec4(800, -800, 800, -800));
  std::shared_ptr<PerspectiveCamera> cam = std::make_shared<PerspectiveCamera>(glm::vec4(70.f, 1.f, 0.01f, 1000.f));
  world->AddActor(cam);
  renderer->UseCamera(cam);

  std::string actionKey = "buonce";

  // TODO Convert model to another format

  std::shared_ptr<Context> myContext = std::make_shared<Context>();
  myContext->AddActionMapping(KeyInput::KEY_B, 0, actionKey);
  myContext->AddRangeMapping(KeyInput::KEY_P, 0, "yeet_RANGE", 1);
  myContext->AddRangeMapping(KeyInput::KEY_X, 0, "rX", 1);
  myContext->AddRangeMapping(KeyInput::KEY_Y, 0, "rY", 1);
  myContext->AddRangeMapping(KeyInput::KEY_Z, 0, "rZ", 1);
  myContext->AddRangeMapping(KeyInput::KEY_X, KeyInput::CTRL, "rX", -1);
  myContext->AddRangeMapping(KeyInput::KEY_Y, 0, "-rY", 1);
  myContext->AddRangeMapping(KeyInput::KEY_Z, 0, "-rZ", 1);

  std::shared_ptr<Context> cameraContext = std::make_shared<Context>();
  cameraContext->AddRangeMapping(KeyInput::KEY_W, 0, "WS", 1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_S, 0, "WS", -1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_D, 0, "AD", -1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_A, 0, "AD", 1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_E, 0, "EQ", 1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_Q, 0, "EQ", -1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_1, 0, "12", 1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_2, 0, "12", -1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_SPACE, 0, "Space_Ctrl", 1.f);
  cameraContext->AddRangeMapping(KeyInput::KEY_SPACE, KeyInput::CTRL, "Space_Ctrl", -1.f);

  input->AddContext(cameraContext);
  input->AddContext(myContext);

  /*input->BindRange("WS", [&](KeyInput::Action action, float val) -> void {
			auto direction = cam->FindComponent<OrthoCameraComponent>()->GetDir();
			auto up = cam->FindComponent<OrthoCameraComponent>()->GetUp();
			cam->FindComponent<TransformComponent>()->Translate(glm::normalize(direction) * val); });

  input->BindRange("AD", [&](KeyInput::Action action, float val) -> void {
		auto direction = cam->FindComponent<OrthoCameraComponent>()->GetDir();
		auto up = cam->FindComponent<OrthoCameraComponent>()->GetUp();
		auto cross = glm::cross(up, direction);
		cam->FindComponent<TransformComponent>()->Translate(glm::normalize(cross) * val); });

  input->BindRange("Space_Ctrl", [&](KeyInput::Action action, float val) -> void {
			auto direction = cam->FindComponent<OrthoCameraComponent>()->GetDir();
			auto up = cam->FindComponent<OrthoCameraComponent>()->GetUp();
			cam->FindComponent<TransformComponent>()->Translate(glm::normalize(up)* val); });

  input->BindRange("EQ", [&](KeyInput::Action action, float val) -> void {
		auto dir = cam->FindComponent<OrthoCameraComponent>()->GetDir();
		glm::mat4 rotationMat(1);
		rotationMat = glm::rotate(rotationMat, glm::pi<float>() / 100 * val, glm::vec3(0.f, 0.f, 1.f));
		auto newUp = glm::vec3(rotationMat * glm::vec4(dir, 1.f));
		cam->FindComponent<OrthoCameraComponent>()->SetDir(newUp); });

  input->BindRange("12", [&](KeyInput::Action action, float val) -> void {
		auto up = cam->FindComponent<OrthoCameraComponent>()->GetUp();
		auto dir = cam->FindComponent<OrthoCameraComponent>()->GetDir();
		glm::mat4 rotationMat(1);
		rotationMat = glm::rotate(rotationMat, glm::pi<float>() / 100 * val, glm::vec3(0.f, 1.f, 1.f));
		auto newDir = glm::vec3(rotationMat * glm::vec4(dir, 1.f));
		auto newUp = glm::vec3(rotationMat * glm::vec4(up, 1.f));
		cam->FindComponent<OrthoCameraComponent>()->SetDir(newDir);
		cam->FindComponent<OrthoCameraComponent>()->SetUp(newUp); });*/


  input->BindRange("WS", [&](KeyInput::Action action, float val) -> void {
	  auto direction = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	  auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
	  cam->FindComponent<TransformComponent>()->Translate(glm::normalize(direction) * val); });

  input->BindRange("AD", [&](KeyInput::Action action, float val) -> void {
	  auto direction = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	  auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
	  auto cross = glm::cross(up, direction);
	  cam->FindComponent<TransformComponent>()->Translate(glm::normalize(cross) * val); });

  input->BindRange("Space_Ctrl", [&](KeyInput::Action action, float val) -> void {
	  auto direction = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	  auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
	  cam->FindComponent<TransformComponent>()->Translate(glm::normalize(up) * val); });

  input->BindRange("EQ", [&](KeyInput::Action action, float val) -> void {
	  auto dir = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	  glm::mat4 rotationMat(1);
	  rotationMat = glm::rotate(rotationMat, glm::pi<float>() / 100 * val, glm::vec3(0.f, 0.f, 1.f));
	  auto newUp = glm::vec3(rotationMat * glm::vec4(dir, 1.f));
	  cam->FindComponent<PerspectiveCameraComponent>()->SetDir(newUp); });

  input->BindRange("12", [&](KeyInput::Action action, float val) -> void {
	  auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
	  auto dir = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	  glm::mat4 rotationMat(1);
	  rotationMat = glm::rotate(rotationMat, glm::pi<float>() / 100 * val, glm::vec3(0.f, 1.f, 0.f));
	  auto newDir = glm::vec3(rotationMat * glm::vec4(dir, 1.f));
	  auto newUp = glm::vec3(rotationMat * glm::vec4(up, 1.f));
	  cam->FindComponent<PerspectiveCameraComponent>()->SetDir(newDir);
	  cam->FindComponent<PerspectiveCameraComponent>()->SetUp(newUp); });

  cam->FindComponent<TransformComponent>()->Translate(glm::vec3(-10.0f, 4.0f, 4.0f));
  // cam->FindComponent<TransformComponent>()->Rotate(glm::vec3(glm::pi<float>() / 2.f, 0.0f, 0.0f));

  IGame::Run();

  return 0;
}

void rotateHorizontal(KeyInput::Action action, float val)
{
}

void rotateVericle()
{
}
