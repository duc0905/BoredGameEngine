#include "bpch.h"
#include "ClassicGameMode.h"

void ClassicGamemode::OnSetup() {
  SetupCamera();
  SetupBoard();
  SetupPieces(white);
  SetupPieces(black, true);
}

void ClassicGamemode::SetupBoard() {
  board = actorManager->Create<Bored::Actor>();
  // TODO find model for board
  Bored::Render::Model m;
  board->AddComponent<Bored::Render::Model>(m);
}

void ClassicGamemode::SetupCamera()
{
    auto cam = actorManager->Create<Bored::Actor>();
    auto& c = cam->AddComponent<Bored::Camera>();
    auto& t = cam->Get<Bored::Transform>();
    t.pos = { 3.5f, 3.5f, 20.0f };
    c.pitch = -89.9f;
    c.yaw = 90.0f;
    renderer->UseCamera(cam);
}

void ClassicGamemode::SetupPieces(Player &p, bool flip)
{
  p.pieces.clear();
  // TODO Load model for each piece
  // TODO Maybe use factory pattern to create pieces
  // IMPORTANT: DONT ADD COMPONENT INSIDE CONSTRUCTOR OF PIECE
  // 8 pawns
  for (int i = 0; i < 8; i++) {
    auto pawn = actorManager->Create<Bored::Actor>();
    auto& piece = pawn->AddComponent<Piece>(&p, new Pawn());
    piece.pos = {i, 1};
    piece.name = (&p == &white ? "White" : "Black");
    piece.name += "Pawn";
    piece.name += i + '1';
    Bored::Render::Model m = Bored::Helper::Load("./res/models/pawn.gltf");
    pawn->AddComponent<Bored::Render::Model>(m);

    p.pieces.push_back(pawn);
  }

  for (int i = 0; i < 2; i++) {
    {
      // 2 rooks
      auto rook = actorManager->Create<Bored::Actor>();
      auto& piece = rook->AddComponent<Piece>(&p, new Rook());
      piece.pos = { i * 7, 0 };
      piece.name = (&p == &white ? "White" : "Black");
      piece.name += "Rook";
      piece.name += i + '1';
      Bored::Render::Model m = Bored::Helper::Load("./res/models/rook.gltf");
      rook->AddComponent<Bored::Render::Model>(m);
      p.pieces.push_back(rook);
    }

    {
      // 2 knights
      auto knight = actorManager->Create<Bored::Actor>();
      auto& piece = knight->AddComponent<Piece>(&p, new Knight());
      piece.pos = { 1 + i * 5, 0 };
      piece.name = (&p == &white ? "White" : "Black");
      piece.name += "Knight";
      piece.name += i + '1';
      Bored::Render::Model m = Bored::Helper::Load("./res/models/knight.gltf");
      knight->AddComponent<Bored::Render::Model>(m);
      p.pieces.push_back(knight);
    }

    {
      // 2 bishops
      auto bishop = actorManager->Create<Bored::Actor>();
      auto& piece = bishop->AddComponent<Piece>(&p, new Bishop());
      piece.pos = { 2 + i * 3, 0 };
      piece.name = (&p == &white ? "White" : "Black");
      piece.name += "Bishop";
      piece.name += i + '1';
      Bored::Render::Model m = Bored::Helper::Load("./res/models/bishop.gltf");
      bishop->AddComponent<Bored::Render::Model>(m);
      p.pieces.push_back(bishop);
    }
  }

  {
    // 1 queen
    auto queen = actorManager->Create<Bored::Actor>();
    auto& piece = queen->AddComponent<Piece>(&p, new Queen());
    piece.pos = { 3, 0 };
    piece.name = (&p == &white ? "White" : "Black");
    piece.name += "Queen";
    Bored::Render::Model m = Bored::Helper::Load("./res/models/queen.gltf");
    queen->AddComponent<Bored::Render::Model>(m);
    p.pieces.push_back(queen);
    myqueen = queen;
  }

  {
    // 1 king
    auto king = actorManager->Create<Bored::Actor>();
    auto& piece = king->AddComponent<Piece>(&p, new King());
    piece.pos = { 4, 0 };
    piece.name = (&p == &white ? "White" : "Black");
    piece.name += "King";
    Bored::Render::Model m = Bored::Helper::Load("./res/models/king.gltf");
    king->AddComponent<Bored::Render::Model>(m);
    p.pieces.push_back(king);
    myking = king;
  }

  if (flip)
    for (auto& a : p.pieces)
    {
      auto& piece = a->Get<Piece>();
      piece.pos = { piece.pos.x, 7 - piece.pos.y };
    }

  for (auto& a : p.pieces)
  {
    auto& piece = a->Get<Piece>();
    auto& t = a->Get<Bored::Transform>();
    t.pos = { piece.pos.x, piece.pos.y, 0.0f };
    t.scale = { 0.3f, 0.3f, 0.3f };
  }
}

void ClassicGamemode::OnImGui() {
  auto& t = myqueen->Get<Bored::Transform>();

  ImGui::Begin("My Queen");
  ImGui::DragFloat3("Position", (float*)&t.pos, 0.3f, -10.0f, 10.0f, "%.2f");
  ImGui::DragFloat3("Rotation", (float*)&t.rotation, 0.1f, -90.0f, 90.0f, "%.2f");
  ImGui::DragFloat3("Scale", (float*)&t.scale, 0.3f, 0.1f, 10.0f, "%.2f");
  ImGui::End();

  auto& t2 = myking->Get<Bored::Transform>();
  ImGui::Begin("My King");
  ImGui::DragFloat3("Position", (float*)&t2.pos, 0.3f, -10.0f, 10.0f, "%.2f");
  ImGui::DragFloat3("Rotation", (float*)&t2.rotation, 0.1f, -90.0f, 90.0f, "%.2f");
  ImGui::DragFloat3("Scale", (float*)&t2.scale, 0.3f, 0.1f, 10.0f, "%.2f");
  ImGui::End();
}
