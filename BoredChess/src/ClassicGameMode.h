#pragma once
#include "Core.h"
#include "Pieces.h"

USE_GAME_MODULES

struct Player {
  std::vector<std::shared_ptr<Bored::Actor>> pieces;
};

class ClassicGamemode : public Bored::Gamemode {
  // Just for render purpose
  std::shared_ptr<Bored::Actor> board;

  Player white;
  Player black;
  
  Player& turn = white;
  std::shared_ptr<Piece> selecting = nullptr;

  //std::array<std::array<Bored::Actor, 8>, 8> board;

  std::shared_ptr<Bored::Actor> whiteCam = nullptr;
  std::shared_ptr<Bored::Actor> blackCam = nullptr;

  std::shared_ptr<Bored::Actor> myqueen;
  std::shared_ptr<Bored::Actor> myking;

  void SetupBoard();
  void SetupCamera();
  void SetupPieces(Player &p, bool flip = false);

public:
  virtual void OnSetup() override;
  virtual void OnImGui() override;
};


