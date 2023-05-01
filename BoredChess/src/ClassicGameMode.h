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

  std::shared_ptr<Player> white, black, turn;
  std::shared_ptr<Player> selecting = nullptr;
  std::shared_ptr<Bored::Input::Context> inGame;

  //std::array<std::array<Bored::Actor, 8>, 8> board;
  float mouse_x = -1.0f, mouse_y = -1.0f;
  std::shared_ptr<Bored::Actor> whiteCam = nullptr;
  std::shared_ptr<Bored::Actor> blackCam = nullptr;

  std::shared_ptr<Bored::Actor> myqueen;
  std::shared_ptr<Bored::Actor> myking;

  void SetupBoard();
  void SetupCamera();
  void SetupPieces(std::shared_ptr<Player> p, bool flip = false);

public:
  virtual void OnSetup() override;
  virtual bool OnTick(double dt) override;
  virtual void OnImGui() override;
};


