#include "ChessGameMode.h"
#include "IGame.h"

ChessGameMode::ChessGameMode(IWorld& world)
	: IGameMode(world)
{	
	auto& hud = IGame::GetHUD();
	auto& input = IGame::GetInput();
	IHUDWindow& debugWindow = hud.AddWindow();
	ImGuiHUDWindow& debug = static_cast<ImGuiHUDWindow&>(debugWindow);
	debug.SetTitle("Debug Window");

	text = new ImGuiTextEl("0");
	debug.AddElement(text);


	ChessBoardState chessDefaultBoard[8][8] = {
	{ {0, PissActor::Type::ROOK}, {0, PissActor::Type::KNIGHT}, {0, PissActor::Type::BISHOP}, {0, PissActor::Type::QUEEN}, {0, PissActor::Type::KING}, {0, PissActor::Type::BISHOP}, {0,  PissActor::Type::KNIGHT}, {0,  PissActor::Type::ROOK} },
	{ {0, PissActor::Type::PAWN}, {0, PissActor::Type::PAWN}, {0, PissActor::Type::PAWN}, {0, PissActor::Type::PAWN}, {0, PissActor::Type::PAWN}, {0, PissActor::Type::PAWN}, {0,  PissActor::Type::PAWN}, {0,  PissActor::Type::PAWN} },
	{ {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY} },
	{ {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY} },
	{ {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY} },
	{ {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1, PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY}, {-1,  PissActor::Type::EMPTY} },
	{ {1, PissActor::Type::PAWN}, {1, PissActor::Type::PAWN}, {1, PissActor::Type::PAWN}, {1, PissActor::Type::PAWN}, {1, PissActor::Type::PAWN}, {1,  PissActor::Type::PAWN}, {1,  PissActor::Type::PAWN}, {1,  PissActor::Type::PAWN} },
	{ {1, PissActor::Type::ROOK}, {1, PissActor::Type::KNIGHT}, {1, PissActor::Type::BISHOP}, {1, PissActor::Type::QUEEN}, {1, PissActor::Type::KING}, {1,  PissActor::Type::BISHOP}, {1,  PissActor::Type::KNIGHT}, {1,  PissActor::Type::ROOK} }
	};


	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			ChessBoardState state;
			boardState[x][y] = chessDefaultBoard[7-x][7-y];
		}
	}
	std::shared_ptr<Context> chessContext = std::make_shared<Context>();
	chessContext->AddActionMapping(KeyInput::KEY_MB_2, 0, "Select Actor");


	input.BindAction("Select Actor", [&](KeyInput::Action action) -> void {
		if (action != KeyInput::PRESS) return;
		auto actor = input.GetCursorHoveringActor();
		auto pis = std::dynamic_pointer_cast<PissActor>(actor);
		LOG(pis->getPosition().first);
		LOG(pis->getPosition().second);
		LOG(actor->GetID());
		auto selectingTile = std::dynamic_pointer_cast<TileActor>(actor);
		auto selectingActor = std::dynamic_pointer_cast<PissActor>(actor);
		if (selectingActor != nullptr) {
			currentSelectedActor = selectingActor;
			nextMove = getPossibleMove(selectingActor);
			for (auto& el : nextMove) {
				std::cout << el.first << " " << el.second << std::endl;
			}
		}
		if (selectingTile != nullptr) {
			if (currentSelectedActor != nullptr) {
				auto pos = selectingTile->GetBoardPosition();
				auto it = std::find(nextMove.begin(), nextMove.end(), pos);
				if (it != nextMove.end()) {
					auto transComp = currentSelectedActor->FindComponent<TransformComponent>();
					auto oldPos = currentSelectedActor->getPosition();
					currentSelectedActor->updatePosition(pos.first, pos.second);
					ChessBoardState newObj;
					newObj.team = -1;
					newObj.type = PissActor::EMPTY;
					boardState[oldPos.first][oldPos.second] = newObj;
					transComp->Translate({ 0, pos.first - oldPos.first, pos.second - oldPos.second });
					newObj.type = PissActor::PAWN;
					newObj.team = 1;
					boardState[pos.first][pos.second] = newObj;
				}
			}
			currentSelectedActor = nullptr;
			nextMove = std::vector<std::pair<int, int>>();
		}
		//LOG("Clicked");
		});

	input.AddContext(chessContext);
}


std::vector<std::pair<int, int>> ChessGameMode::getPossibleMove(std::shared_ptr<PissActor> actor) {
	if (actor == nullptr) {
		return std::vector<std::pair<int, int>>();
	}
	else {
		PissActor::Type t = actor->getType();
		switch (t) {
			//case PissActor::Type::ROOK:
		case PissActor::Type::EMPTY:
			std::cout << "What";
			break;
		case PissActor::Type::PAWN: {
			std::pair<int, int> pos = actor->getPosition();
			int x = pos.first;
			int y = pos.second;
			int team = boardState[x][y].team;
			std::vector<std::pair<int, int>> moves;
			if (team == 1) { // white pawn
				if (y == 1 && boardState[x][y + 2].type == PissActor::EMPTY) {
					moves.push_back({ x, y + 2 });
				}
				if (y < 7 && boardState[x][y + 1].type == PissActor::EMPTY) {
					moves.push_back({ x, y + 1 });
				}
				if (x > 0 && boardState[x - 1][y + 1].type != PissActor::EMPTY &&
					boardState[x - 1][y + 1].team == 0) {
					moves.push_back({ x - 1, y + 1 });
				}
				if (x < 7 && boardState[x + 1][y + 1].type != PissActor::EMPTY &&
					boardState[x + 1][y + 1].team == 0) {
					moves.push_back({ x + 1, y + 1 });
				}
			}
			else if (team == 0) { // black pawn
				if (y == 6 && boardState[x][y - 2].type == PissActor::EMPTY) {
					moves.push_back({ x, y - 2 });
				}
				if (y > 0 && boardState[x][y - 1].type == PissActor::EMPTY) {
					moves.push_back({ x, y - 1 });
				}
				if (x > 0 && boardState[x - 1][y - 1].type != PissActor::EMPTY &&
					boardState[x][y - 1].team == 1) {
					moves.push_back({ x - 1, y - 1 });
				}
				if (x < 7 && boardState[x + 1][y - 1].type != PissActor::EMPTY &&
					boardState[x + 1][y - 1].team == 1) {
					moves.push_back({ x + 1, y - 1 });
				}
			}
			return moves;
			break;
		}
								  //case PissActor::Type::BISHOP:
								  //case PissActor::Type::KNIGHT:
								  //case PissActor::Type::QUEEN:
								  //case PissActor::Type::KING:
		default:
			std::cout << "This is empty";
		}
	}
	return std::vector<std::pair<int, int>>();
}

void ChessGameMode::OnTick(float)
{
	//LOG_COLOR("Chess on tick", COLOR::BLUE, COLOR::BLACK);
	auto& input = IGame::GetInput();
	auto a = input.GetCursorHoveringActor();
	if (a != nullptr)
	{
		auto stringID = (a != nullptr) ? std::to_string(a->GetID()) : 0;
		text->updateString("Current actor by ID: " + stringID);
	}
	//auto& actor = input.GetCursorHoveringActor();
	//input.BindAction("Select Piss", )
	//LOG(input.GetCusorHoveringActor().GetID());
}

void ChessGameMode::OnLevelStart()
{
	LOG_COLOR("Chess on level start", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnLevelPause()
{
	LOG_COLOR("Chess on pause", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnLevelResume()
{
	LOG_COLOR("Chess on resume", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnLevelEnd()
{
	LOG_COLOR("Chess on end", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnAttach(IWorld& world)
{
	LOG_COLOR("Chess on attach", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnDetach()
{
	LOG_COLOR("Chess on detach", COLOR::BLUE, COLOR::BLACK);
}