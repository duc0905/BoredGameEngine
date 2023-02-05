#pragma once
#include "IGame.h"
#include "DemoUtils.h"

std::shared_ptr<PerspectiveCamera> cam;

void BasicCameraDemo()
{
	SYSTEM_ACCESS

	cam = std::make_shared<PerspectiveCamera>(glm::vec4(70.f, 1.f, 0.01f, 1000.f));
	world.AddActor(cam);
	renderer.UseCamera(cam);

	std::string actionKey = "buonce";

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

	input.AddContext(cameraContext);
	input.AddContext(myContext);

	/*input.BindRange("WS", [&](KeyInput::Action action, float val) . void {
			  auto direction = cam.FindComponent<OrthoCameraComponent>().GetDir();
			  auto up = cam.FindComponent<OrthoCameraComponent>().GetUp();
			  cam.FindComponent<TransformComponent>().Translate(glm::normalize(direction) * val); });

	input.BindRange("AD", [&](KeyInput::Action action, float val) . void {
		  auto direction = cam.FindComponent<OrthoCameraComponent>().GetDir();
		  auto up = cam.FindComponent<OrthoCameraComponent>().GetUp();
		  auto cross = glm::cross(up, direction);
		  cam.FindComponent<TransformComponent>().Translate(glm::normalize(cross) * val); });

	input.BindRange("Space_Ctrl", [&](KeyInput::Action action, float val) . void {
			  auto direction = cam.FindComponent<OrthoCameraComponent>().GetDir();
			  auto up = cam.FindComponent<OrthoCameraComponent>().GetUp();
			  cam.FindComponent<TransformComponent>().Translate(glm::normalize(up)* val); });

	input.BindRange("EQ", [&](KeyInput::Action action, float val) . void {
		  auto dir = cam.FindComponent<OrthoCameraComponent>().GetDir();
		  glm::mat4 rotationMat(1);
		  rotationMat = glm::rotate(rotationMat, glm::pi<float>() / 100 * val, glm::vec3(0.f, 0.f, 1.f));
		  auto newUp = glm::vec3(rotationMat * glm::vec4(dir, 1.f));
		  cam.FindComponent<OrthoCameraComponent>().SetDir(newUp); });

	input.BindRange("12", [&](KeyInput::Action action, float val) . void {
		  auto up = cam.FindComponent<OrthoCameraComponent>().GetUp();
		  auto dir = cam.FindComponent<OrthoCameraComponent>().GetDir();
		  glm::mat4 rotationMat(1);
		  rotationMat = glm::rotate(rotationMat, glm::pi<float>() / 100 * val, glm::vec3(0.f, 1.f, 1.f));
		  auto newDir = glm::vec3(rotationMat * glm::vec4(dir, 1.f));
		  auto newUp = glm::vec3(rotationMat * glm::vec4(up, 1.f));
		  cam.FindComponent<OrthoCameraComponent>().SetDir(newDir);
		  cam.FindComponent<OrthoCameraComponent>().SetUp(newUp); });*/


	input.BindRange("WS", [&](KeyInput::Action action, float val) -> void {
		auto direction = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
	cam->FindComponent<TransformComponent>()->Translate(glm::normalize(direction) * val); });

	input.BindRange("AD", [&](KeyInput::Action action, float val) -> void {
		auto direction = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
	auto cross = glm::cross(up, direction);
	cam->FindComponent<TransformComponent>()->Translate(glm::normalize(cross) * val); });

	input.BindRange("Space_Ctrl", [&](KeyInput::Action action, float val) -> void {
		auto direction = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
	cam->FindComponent<TransformComponent>()->Translate(glm::normalize(up) * val); });

	input.BindRange("EQ", [&](KeyInput::Action action, float val) -> void {
		auto dir = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	glm::mat4 rotationMat(1);
	rotationMat = glm::rotate(rotationMat, glm::pi<float>() / 100 * val, glm::vec3(0.f, 0.f, 1.f));
	auto newUp = glm::vec3(rotationMat * glm::vec4(dir, 1.f));
	cam->FindComponent<PerspectiveCameraComponent>()->SetDir(newUp); });

	input.BindRange("12", [&](KeyInput::Action action, float val) -> void {
		auto up = cam->FindComponent<PerspectiveCameraComponent>()->GetUp();
	auto dir = cam->FindComponent<PerspectiveCameraComponent>()->GetDir();
	glm::mat4 rotationMat(1);
	rotationMat = glm::rotate(rotationMat, glm::pi<float>() / 100 * val, glm::vec3(0.f, 1.f, 0.f));
	auto newDir = glm::vec3(rotationMat * glm::vec4(dir, 1.f));
	auto newUp = glm::vec3(rotationMat * glm::vec4(up, 1.f));
	cam->FindComponent<PerspectiveCameraComponent>()->SetDir(newDir);
	cam->FindComponent<PerspectiveCameraComponent>()->SetUp(newUp); });

	cam->FindComponent<TransformComponent>()->Translate(glm::vec3(-10.0f, 4.0f, 4.0f));
}
