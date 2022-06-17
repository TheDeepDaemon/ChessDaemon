#include"System.h"
#include<array>



void System::start() {

	board.setupBoard();

	board.setCenteredPosition();

}


void System::update() {

	board.display(&displaySystem, heldPiece.getBoardPosition());

	if (upgradedPawn.isUpgrading) {
		showPawnUpgrades(upgradedPawn.posTo.row, upgradedPawn.posTo.col, true, 2);
	}
	else {
		if (holdingPiece) {
			showHoldingPiece();
		}
		else if (inputSystem.mouseDown) {
			pickupPiece();
		}
	}

}


void System::pickupPiece() {
	BoardPosition pieceCoord = board.screenToBoard(inputSystem.getMousePos());
	if (ChessBoard::isValid(pieceCoord)) {
		ChessPiece piece = board.get(pieceCoord);
		if (board.whiteToMove == piece.white) {
			heldPiece = ChessPiecePosition(piece, pieceCoord);

			availableMoves.clear();
			board.getPossibleMoves(pieceCoord, availableMoves);

			holdingPiece = true;
		}
	}
}


void System::showHoldingPiece() {
	board.displayPossibleMoves(
		&displaySystem, heldPiece.getBoardPosition(), availableMoves);
	board.debugLegalMoves(heldPiece.getBoardPosition());

	displaySystem.drawCentered(getPieceName(heldPiece), inputSystem.getMousePos());

	if (inputSystem.mouseUp) {

		BoardPosition from = heldPiece.getBoardPosition();

		BoardPosition to = board.screenToBoard(inputSystem.getMousePos());

		if (board.isLegal(from, to)) {

			if (heldPiece.is(PAWN) &&
				((to.row == 0 && !heldPiece.white) ||
					(to.row == 7 && heldPiece.white))) {
				upgradedPawn.isUpgrading = true;
				upgradedPawn.posFrom = heldPiece.getBoardPosition();
				upgradedPawn.posTo = to;
				upgradedPawn.color = heldPiece.white;
			}
			else {
				dropPiece(from, to);
			}

		}

		heldPiece = ChessPiecePosition(ChessPiece(EMPTY, false), BoardPosition(-1, -1));

		availableMoves.clear();
		holdingPiece = false;
	}
}


void System::dropPiece(const BoardPosition& from, const BoardPosition& to) {

	for (const ChessBoard::Move& move : availableMoves) {
		if (move.from == from && move.to == to) {
			if (!board.wouldBeInCheck(board.get(from).white, move)) {
				board.makeMove(move);
			}
			break;
		}
	}

	printGameState();
}

void System::printGameState() {
	if (board.isKingInCheck(true)) {
		cout << "white in check\n";
		if (board.isCheckMated(true)) {
			cout << "white check mated\n";
		}
	}
	else if (board.isDraw(true)) {
		cout << "Game is drawn, white cannot move\n";
	}

	if (board.isKingInCheck(false)) {
		cout << "black in check\n";
		if (board.isCheckMated(false)) {
			cout << "black check mated\n";
		}
	}
	else if (board.isDraw(false)) {
		cout << "Game is drawn, black cannot move\n";
	}
}


bool isInBounds(const sf::FloatRect& rect, const Vector2f& pos) {
	return (rect.left < pos.x) &&
		(pos.x < rect.left + rect.width) &&
		(rect.top < pos.y) &&
		(pos.y < rect.top + rect.height);
}


void System::showPawnUpgrades(const int row, const int col, const bool white, const float padding) {

	const std::array<ChessPieceType, 4> upgrades = { QUEEN, ROOK, BISHOP, KNIGHT };

	const Vector2f squareSize = board.squareSize;

	const float rectWidth = squareSize.x + (padding * 2);
	const float rectHeight = (squareSize.y * upgrades.size()) + (padding * 2);

	const Vector2f posOnBoard = Vector2f(
		squareSize.x * col,
		squareSize.y * (BOARD_SIZE - row - 1));
	const Vector2f posOnScreen = board.boardPosition + posOnBoard;

	sf::RectangleShape rectangle(Vector2f(rectWidth, rectHeight));
	rectangle.setPosition(posOnScreen - Vector2f(padding, padding));
	rectangle.setFillColor(sf::Color::White);
	rectangle.setOutlineColor(sf::Color::Black);
	rectangle.setOutlineThickness(4);

	displaySystem.draw(rectangle);

	const Vector2f offset(0, squareSize.y);

	// draw the different options: queen, rook, bishop, knight

	for (unsigned i = 0; i < upgrades.size(); i++) {
		const string name = getPieceName(ChessPiece(upgrades[i], white));
		displaySystem.draw(name, posOnScreen + (offset * (float)i), squareSize.x, squareSize.y);
	}

	if (inputSystem.mouseDown) {
		const Vector2f mousePos = inputSystem.getMousePos();
		for (unsigned i = 0; i < upgrades.size(); i++) {
			if (isInBounds(sf::FloatRect(posOnScreen + (offset * (float)i), squareSize), mousePos)) {

				ChessBoard::Move move(upgradedPawn.posFrom, upgradedPawn.posTo);

				move.promotionTo = upgrades[i];

				if (!board.wouldBeInCheck(white, move)) {
					board.makeMove(move);
				}

				upgradedPawn.isUpgrading = false;
				upgradedPawn.posFrom = BoardPosition(-1, -1);
				upgradedPawn.posTo = BoardPosition(-1, -1);
				break;
			}
		}
	}

}

