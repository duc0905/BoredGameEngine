#pragma once
#include <memory>
#include <Engine/Scene.hpp>

class ChessLogic : public Bored::Module
{
  public:
    ChessLogic() = default;
    ChessLogic(const ChessLogic&) = default;
    ChessLogic(ChessLogic&&) = delete;
    ChessLogic& operator=(const ChessLogic&) = default;
    ChessLogic& operator=(ChessLogic&&) = delete;
    virtual ~ChessLogic() = default;

    virtual void OnSetup() override;
    virtual void OnSwitchScene() override;
    virtual bool OnUpdate(double dt) override;
    virtual void OnShutdown() override;

  public:
    void HandleClick();

  private:
    std::shared_ptr<Bored::Actor> cube;
    std::shared_ptr<Bored::Actor> triangle;
};
