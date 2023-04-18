#pragma once
#include "bpch.h"
#include "Core.h"

struct Player;

class MoveBehaviour
{
private:
  bool isMoved = false;
public:
  virtual std::vector<glm::vec2> GetRelativeMoves() const = 0;
  virtual std::vector<glm::vec2> GetRelativeEats() const = 0;
};

class Pawn : public MoveBehaviour {
public:
  virtual std::vector<glm::vec2> GetRelativeMoves() const override {
    return {{0, 1}, {0, 2}};
  };
  virtual std::vector<glm::vec2> GetRelativeEats() const override {
    return {{1, 1}, {-1, 1}};
  };
};
class Rook : public MoveBehaviour {
public:
  virtual std::vector<glm::vec2> GetRelativeMoves() const override {
      std::vector<glm::vec2> moves;
    for (int i = 1; i < 8; i++) {
      moves.push_back({ i, 0});
      moves.push_back({ -i, 0});
      moves.push_back({ 0, i});
      moves.push_back({ 0, -i});
    }
    return moves;
  };
  virtual std::vector<glm::vec2> GetRelativeEats() const override {
      std::vector<glm::vec2> eats;
    for (int i = 1; i < 8; i++) {
      eats.push_back({ i, 0});
      eats.push_back({ -i, 0});
      eats.push_back({ 0, i});
      eats.push_back({ 0, -i});
    }
    return eats;
  };
};
class Knight : public MoveBehaviour {
public:
  virtual std::vector<glm::vec2> GetRelativeMoves() const override {
    return {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
  };
  virtual std::vector<glm::vec2> GetRelativeEats() const override {
    return {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
  };
};
class Bishop : public MoveBehaviour {
public:
  virtual std::vector<glm::vec2> GetRelativeMoves() const override {
    std::vector<glm::vec2> moves;
    for (int i = 1; i < 8; i++) {
        moves.push_back({i, i});
        moves.push_back({-i, -i});
        moves.push_back({i, -i});
        moves.push_back({-i, i});
    }
    return moves;
  };
  virtual std::vector<glm::vec2> GetRelativeEats() const override {
    std::vector<glm::vec2> eats;
    for (int i = 1; i < 8; i++) {
        eats.push_back({i, i});
        eats.push_back({-i, -i});
        eats.push_back({i, -i});
        eats.push_back({-i, i});
    }
    return eats;
  };
};

class Queen : public MoveBehaviour {
public:
  virtual std::vector<glm::vec2> GetRelativeMoves() const override {
    std::vector<glm::vec2> moves;
    for (int i = 1; i < 8; i++) {
        moves.push_back({i, i});
        moves.push_back({-i, -i});
        moves.push_back({i, -i});
        moves.push_back({-i, i});
        moves.push_back({0, i});
        moves.push_back({i, 0});
        moves.push_back({-i, 0});
        moves.push_back({0, -i});
    }
    return moves;
  };
  virtual std::vector<glm::vec2> GetRelativeEats() const override {
    std::vector<glm::vec2> eats;
    for (int i = 1; i < 8; i++) {
        eats.push_back({i, i});
        eats.push_back({-i, -i});
        eats.push_back({i, -i});
        eats.push_back({-i, i});
        eats.push_back({0, i});
        eats.push_back({i, 0});
        eats.push_back({-i, 0});
        eats.push_back({0, -i});
    }
    return eats;
  };
};

class King : public MoveBehaviour {
public:
  virtual std::vector<glm::vec2> GetRelativeMoves() const override {
    return {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
  };
  virtual std::vector<glm::vec2> GetRelativeEats() const override {
    return {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
  };
};

struct Piece {
  // Position on the board
  glm::vec2 pos = {0, 0};
  std::string name = "";
  bool isDead = false;

  Player* side;
  std::unique_ptr<MoveBehaviour> mb;
  Piece() = default;
  Piece(Player* p, MoveBehaviour* m) : side(p), mb(m) {}

  std::vector<glm::vec2> GetRelativeMoves() const
  {
    return mb->GetRelativeMoves();
  }
  std::vector<glm::vec2> GetRelativeEats() const
  {
    return mb->GetRelativeEats();
  }
};


