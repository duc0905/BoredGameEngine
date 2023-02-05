#pragma once
#include "IGame.h"
#include "../DemoUtils.h"
#include "TileActor.h"
#include "Pawn.h"
#include "ChessGameMode.h"

void ChessDemo()
{
	SYSTEM_ACCESS;

	world.UseGameMode<ChessGameMode>(world);

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
			transComp->Translate({ -0.5f, x , y });
		}
	}

	for (int i = 0; i < 8; i++) {
		std::shared_ptr<Actor> pawn = std::make_shared<Pawn>();
		pawnStorage.push_back(pawn);
		auto transComp = pawn->FindComponent<TransformComponent>();
		transComp->Translate({ -0.5f, i, 3.0f });
	}

	//world->AddActor(cube);
	for (auto p : pawnStorage) {
		world.AddActor(p);
	}

	for (auto t : tileStorage) {
		world.AddActor(t);
	}
}
