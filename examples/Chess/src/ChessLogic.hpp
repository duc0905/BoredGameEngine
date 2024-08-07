#pragma once
#include "Bored.hpp"
#include <memory>

class ChessLogic : public Bored::Module
{
  public:
    void OnSetup() override;
    bool OnUpdate(double dt) override;
    void OnShutdown() override;

  public:
    void HandleClick();

  private:
    std::shared_ptr<Bored::Actor> cube1, cube2;
};
