#pragma once
#include "Actor/Actor.h"
#include "Components/MeshComponent.h"

class PissActor : public Actor {
private:
	int _x, _y, id, team; // position of a chess piss on the board
	bool dead = false;

public:
	enum Type {
		PAWN,
		BISHOP,
		KNIGHT,
		ROOK,
		KING,
		QUEEN,
		EMPTY
	};

	PissActor() {
		_x = -1;
		_y = -1;
		CreateComponent<MeshComponent>();
	}

	virtual Type getId() = 0;

	//virtual std::vector<std::pair<int, int>> getPossileMoves() = 0;

	bool updatePosition(int x, int y) {
		_x = x;
		_y = y;
	};

	std::pair<int, int> getPosition() {
		return { _x, _y };
	}

	void removeChessPiss() {
		dead = true;
	};
};