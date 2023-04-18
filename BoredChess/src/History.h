#pragma once
#include "Pieces.h"
#include "bpch.h"
class History;
class Command {
  friend class History;
  std::shared_ptr<Command> next;
  std::shared_ptr<Command> last;
  virtual void undo() = 0;
  virtual bool execute() = 0;

  std::shared_ptr<Piece> subject;
  glm::vec2 lastPos = {0,0};
public:
  Command() {}
  Command(std::shared_ptr<Piece> s) : subject(s), lastPos(s->pos) {}
};

class Move : public Command {
  glm::vec2 dest;
  // Inherited via Move
  virtual void undo() override {}
  virtual bool execute() override { return false;}
public:
  Move(std::shared_ptr<Piece> s, glm::vec2 d) : Command(s), dest(d) {}
};

class Eat : public Command {
  std::shared_ptr<Piece> object;
  // Inherited via Command
  virtual void undo() override {}
  virtual bool execute() override { return false;}

public:
  Eat(std::shared_ptr<Piece> s, std::shared_ptr<Piece> o)
      : Command(s), object(o) {}
};

class Castle : public Command {
  // Inherited via Command
  virtual void undo() override {}
  virtual bool execute() override { return false;}

public:
  Castle(std::shared_ptr<Piece> s) : Command(s) {}
};

class EnPassant : public Command {
  std::shared_ptr<Piece> object;
  // Inherited via Command
  virtual void undo() override {}
  virtual bool execute() override { return false;}

public:
  EnPassant(std::shared_ptr<Piece> s, std::shared_ptr<Piece> o)
      : Command(s), object(o) {}
};

/**
 * @brief Store history of the game
 * Implement Command pattern
 */
class History {
  std::shared_ptr<Command> lastMove;

public:
  void undo() {
    lastMove->undo();
    lastMove = lastMove->last;
  }

  bool push(std::shared_ptr<Command> c) {
    if (c->execute()) {
      c->last = lastMove;
      lastMove->next = c;
      lastMove = c;
      return true;
    }
    return false;
  }
};
