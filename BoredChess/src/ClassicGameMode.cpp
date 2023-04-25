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
    whiteCam = actorManager->Create<Bored::Actor>();
    auto& c = whiteCam->AddComponent<Bored::Camera>();
    auto& t = whiteCam->Get<Bored::Transform>();
    t.pos = { 3.5f, 3.5f, 20.0f };
    c.pitch = -89.9f;
    c.yaw = 90.0f;
    renderer->UseCamera(whiteCam);

    std::shared_ptr<Bored::Input::Context> con = std::make_shared<Bored::Input::Context>();
    con->AddRangeMapping(Bored::Input::KEY_A, 0, "MOVE_CAM_HOR", -1.0f);
    con->AddRangeMapping(Bored::Input::KEY_D, 0, "MOVE_CAM_HOR", 1.0f);
    con->AddRangeMapping(Bored::Input::KEY_W, 0, "MOVE_CAM_VERT", 1.0f);
    con->AddRangeMapping(Bored::Input::KEY_S, 0, "MOVE_CAM_VERT", -1.0f);
    con->AddRangeMapping(Bored::Input::MOUSE_POS_X, 0, "MOVE_CAM_YAW", 0.1f);
    // In GLFW, top-left corner is 0,0, so the weight is  negative
    con->AddRangeMapping(Bored::Input::MOUSE_POS_Y, 0, "MOVE_CAM_PITCH", -0.1f);
    input->AddContext(con);

    input->BindRange("MOVE_CAM_HOR",
      [this](Bored::Input::Action action, float val) {
        auto& t = whiteCam->Get<Bored::Transform>();
        t.pos.x += val;
      }, 1.0f);

    input->BindRange("MOVE_CAM_VERT",
      [this](Bored::Input::Action action, float val) {
        auto& t = whiteCam->Get<Bored::Transform>();
        t.pos.y += val;
      }, 1.0f);

    input->BindRange("MOVE_CAM_YAW",
      [this](Bored::Input::Action action, float val) {
        float dx = val - mouse_x;
        std::cout << dx << std::endl;
    if (dx > 2.0f)
      dx = 2.0f;
    if (dx < -2.0f)
      dx = -2.0f;

    auto& c = whiteCam->Get<Bored::Camera>();
    c.yaw += dx;

    if (c.yaw >= 360.0f)
      c.yaw = 0.0f;
    if (c.yaw <= -360.f)
      c.yaw = 0.0f;
    mouse_x = val;
      }, 1.0f);

    input->BindRange("MOVE_CAM_PITCH",
      [this](Bored::Input::Action action, float val) {
        //std::cout << val << std::endl;
        float dy = val - mouse_y;
    if (dy > 2.0f)
      dy = 2.0f;
    if (dy < -2.0f)
      dy = -2.0f;

    auto& c = whiteCam->Get<Bored::Camera>();
    c.pitch += dy;

    if (c.pitch >= 89.0f)
      c.pitch = 89.0f;
    if (c.pitch <= -89.f)
      c.pitch = -89.0f;
        mouse_y = val;
      }, 1.0f);
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
