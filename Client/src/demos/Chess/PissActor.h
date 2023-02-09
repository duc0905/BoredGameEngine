#pragma once
#include "Actor/Actor.h"
#include "Components/MeshComponent.h"

class PissActor : public Actor {
protected:
	int _x, _y, _team; // position of a chess piss on the board
	bool _dead = false;

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


	PissActor(int x = -1, int y = -1, int team = 1, bool dead = false)
		: _x(x), _y(y), _team(team), _dead(dead)
	{
		CreateComponent<MeshComponent>();
	}

	virtual Type getType() = 0;

	//virtual std::vector<std::pair<int, int>> getPossileMoves() = 0;

	void updatePosition(int x, int y) {
		_x = x;
		_y = y;
	};

	std::pair<int, int> getPosition() {
		return { _x, _y };
	}

	void removeChessPiss() {
		_dead = true;
	};
};