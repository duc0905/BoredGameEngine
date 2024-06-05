#include "ChessLogic.hpp"
#include <iostream>

void ChessLogic::OnSetup() {}

bool ChessLogic::OnUpdate(double dt)  {
  std::cout << "Hello from ChessLogic!" << std::endl;
  return true;
}

void ChessLogic::OnShutdown() {}

void ChessLogic::HandleClick() {
  std::cout << "Clicked" << std::endl;
}
