#pragma once

#define SYSTEM_ACCESS \
	auto& window = IGame::GetWindow();\
	auto& input = IGame::GetInput();\
	auto& renderer = IGame::GetRenderer();\
	auto& hud = IGame::GetHUD();\
	auto& world = IGame::GetWorld();\
	auto& audio = IGame::GetAudio();\
	
