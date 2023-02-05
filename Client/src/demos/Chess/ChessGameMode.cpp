#include "ChessGameMode.h"
#include "IGame.h"

ChessGameMode::ChessGameMode(IWorld& world)
	: IGameMode(world)
{	
	auto& hud = IGame::GetHUD();
	IHUDWindow& debugWindow = hud.AddWindow();
	ImGuiHUDWindow& debug = static_cast<ImGuiHUDWindow&>(debugWindow);
	debug.SetTitle("Debug Window");

	text = new ImGuiTextEl("0");
	debug.AddElement(text);

	boardState.assign(64, ChessBoardState());

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			ChessBoardState state;
			if (y == 1) {
				state.team = 1;
				state.type = PissActor::Type::PAWN;
				boardState.insert(boardState.begin() + x + y * 8, state);
			}
		}
	}
}


std::vector<std::pair<int, int>> ChessGameMode::getPossibleMove(PissActor& actor) {
	switch (actor.getId()) {
		//case PissActor::Type::ROOK:
	case PissActor::Type::PAWN: {
		std::pair<int, int> pos = actor.getPosition();
		int x = pos.first;
		int y = pos.second;
		int team = boardState.at(x + 8 * y).team;
		std::vector<std::pair<int, int>> moves;
		if (team == 1) { // white pawn
			if (y == 1 && boardState.at(x + 8 * (y + 2)).type == PissActor::EMPTY) {
				moves.push_back({ x, y + 2 });
			}
			if (y < 7 && boardState.at(x + 8 * (y + 1)).type == PissActor::EMPTY) {
				moves.push_back({ x, y + 1 });
			}
			if (x > 0 && boardState.at(x - 1 + 8 * (y + 1)).type != PissActor::EMPTY && boardState.at(x - 1 + 8 * (y + 1)).type != team) {
				moves.push_back({ x - 1, y + 1 });
			}
			if (x < 7 && boardState.at(x + 1 + 8 * (y + 1)).type != PissActor::EMPTY && boardState.at(x + 1 + 8 * (y + 1)).type != team) {
				moves.push_back({ x + 1, y + 1 });
			}
		}
		else { // black pawn
			if (y == 6 && boardState.at(x + 8 * (y - 2)).type == PissActor::EMPTY) {
				moves.push_back({ x, y - 2 });
			}
			if (y > 0 && boardState.at(x + 8 * (y - 1)).type == PissActor::EMPTY) {
				moves.push_back({ x, y - 1 });
			}
			if (x > 0 && boardState.at(x - 1 + 8 * (y - 1)).type != PissActor::EMPTY && boardState.at(x - 1 + 8 * (y - 1)).type != team) {
				moves.push_back({ x - 1, y - 1 });
			}
			if (x < 7 && boardState.at(x + 1 + 8 * (y + 1)).type != PissActor::EMPTY && boardState.at(x + 1 + 8 * (y + 1)).type != team) {
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
	return std::vector<std::pair<int, int>>();
}

void ChessGameMode::OnTick(float)
{
	//LOG_COLOR("Chess on tick", COLOR::BLUE, COLOR::BLACK);
	auto& input = IGame::GetInput();
	int actorID = input.GetCursorHoveringActor().GetID();
	auto stringID = std::to_string(actorID);
	text->updateString("Current actor by ID: " + stringID);
	auto& actor = input.GetCursorHoveringActor();
	input.BindAction("Select Piss", )
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