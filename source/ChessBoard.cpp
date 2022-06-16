#include"ChessBoard.h"



ChessBoard::ChessBoard() : BoardState() {}


void ChessBoard::addWhitePiece(const ChessPieceType type, const int row, const int col) {
	get(row, col) = ChessPiece(type, true);
	if (type == KING) {
		whiteKing = BoardPosition(row, col);
	}
}

void ChessBoard::addBlackPiece(const ChessPieceType type, const int row, const int col) {
	get(row, col) = ChessPiece(type, false);
	if (type == KING) {
		blackKing = BoardPosition(row, col);
	}
}

void ChessBoard::addPiece(const ChessPiece piece, const int row, const int col) {
	get(row, col) = piece;
	if (piece.type == KING) {
		if (piece.white) {
			whiteKing = BoardPosition(row, col);
		}
		else {
			blackKing = BoardPosition(row, col);
		}
	}
}


void ChessBoard::addWhitePiece(const ChessPieceType type, const string& algebraicNot) {
	const BoardPosition pos = readAlgebraicNotation(algebraicNot[0], algebraicNot[1]);
	addWhitePiece(type, pos.row, pos.col);
}


void ChessBoard::addBlackPiece(const ChessPieceType type, const string& algebraicNot) {
	const BoardPosition pos = readAlgebraicNotation(algebraicNot[0], algebraicNot[1]);
	addBlackPiece(type, pos.row, pos.col);
}


void ChessBoard::setupBoard() {

	for (int i = 0; i < BOARD_SIZE; i++) {
		addWhitePiece(PAWN, 1, i);
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		addBlackPiece(PAWN, 6, i);
	}

	addWhitePiece(ROOK, 0, 0);
	addWhitePiece(KNIGHT, 0, 1);
	addWhitePiece(BISHOP, 0, 2);
	addWhitePiece(QUEEN, 0, 3);
	addWhitePiece(KING, 0, 4);
	addWhitePiece(BISHOP, 0, 5);
	addWhitePiece(KNIGHT, 0, 6);
	addWhitePiece(ROOK, 0, 7);

	addBlackPiece(ROOK, 7, 0);
	addBlackPiece(KNIGHT, 7, 1);
	addBlackPiece(BISHOP, 7, 2);
	addBlackPiece(QUEEN, 7, 3);
	addBlackPiece(KING, 7, 4);
	addBlackPiece(BISHOP, 7, 5);
	addBlackPiece(KNIGHT, 7, 6);
	addBlackPiece(ROOK, 7, 7);

	whiteKingHasMoved = false;
	wqRookHasMoved = false;
	wkRookHasMoved = false;

	blackKingHasMoved = false;
	bqRookHasMoved = false;
	bkRookHasMoved = false;

}



void ChessBoard::display(DisplaySystem* disp, const BoardPosition mask) {
	float squareWidth = squareSize.x;
	float squareHeight = squareSize.y;

	Vector2f bSprSize = boardSizeOnScreen + (boardEdge * 2.0f);
	disp->draw(sprName, boardPosition - boardEdge, bSprSize.x, bSprSize.y);

	if constexpr (GRID_IS_VISIBLE) {
		const string whiteSquare = "white square";
		const string blackSquare = "black square";
		disp->setSpriteSize(whiteSquare, squareWidth, squareHeight);
		disp->setSpriteSize(blackSquare, squareWidth, squareHeight);

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				int row = j;
				int col = i;
				Vector2f screenPos = boardToScreen(BoardPosition(row, col));
				if ((i + j) % 2 == 1) {
					disp->draw(whiteSquare, screenPos);
				}
				else {
					disp->draw(blackSquare, screenPos);
				}
			}
		}
	}



	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			const int row = i;
			const int col = j;
			if (mask != BoardPosition(row, col)) {
				ChessPiece piece = get(row, col);
				if (piece.type != EMPTY) {

					Vector2f screenPos = boardToScreen(BoardPosition(row, col));

					string pieceName = getPieceName(piece);

					Vector2f sprSize = squareSize * PIECE_SCALE;
					disp->setSpriteSize(pieceName, sprSize.x, sprSize.y);

					disp->drawCentered(pieceName, screenPos + (squareSize / 2.0f));
				}
			}
		}
	}

}



void ChessBoard::displayPossibleMoves(
	DisplaySystem* disp, const BoardPosition from, const std::list<Move>& possibleMoves) {

	const string greenDot = "green dot";
	Vector2f sprSize = squareSize * GREEN_DOT_SCALE;
	disp->setSpriteSize(greenDot, sprSize.x, sprSize.y);

	bool white = get(from).white;

	for (const Move& move : possibleMoves) {
		if (!wouldBeInCheck(white, move)) {
			BoardPosition to = move.to;
			disp->drawCentered(greenDot, boardToScreen(to) + (squareSize / 2.0f));
		}
	}

}


void ChessBoard::setCenteredPosition(const float x, const float y) {
	boardPosition = Vector2f(x - (boardSizeOnScreen.x / 2.0f), y - (boardSizeOnScreen.y / 2.0f));
}


BoardPosition ChessBoard::screenToBoard(const sf::Vector2f& screenPos) {
	int col = (int)((screenPos.x - boardPosition.x) / squareSize.x);
	int row = (int)((boardPosition.y + boardSizeOnScreen.y - screenPos.y) / squareSize.y);
	return BoardPosition(row, col);
}


sf::Vector2f ChessBoard::boardToScreen(const BoardPosition& boardPos) {
	int row = BOARD_SIZE - 1 - boardPos.row;
	float x = boardPosition.x + (boardPos.col * squareSize.x);
	float y = boardPosition.y + (row * squareSize.y);
	return sf::Vector2f(x, y);
}


void ChessBoard::movePiece(const BoardPosition& from, const BoardPosition& to) {

	ChessPiece pieceFrom = get(from);

	switch (pieceFrom.type)
	{
	case KING:
		if (pieceFrom.white) {
			whiteKingHasMoved = true;
			whiteKing = to;
		}
		else {
			blackKingHasMoved = true;
			blackKing = to;
		}
		break;
	case ROOK:
		if (pieceFrom.white) {
			if (from.isAt(0, 0)) {
				wqRookHasMoved = true;
			}
			else if (from.isAt(0, 7)) {
				wkRookHasMoved = true;
			}
		}
		else {
			if (from.isAt(7, 0)) {
				bqRookHasMoved = true;
			}
			else if (from.isAt(7, 7)) {
				bkRookHasMoved = true;
			}
		}
		break;
	default:
		break;
	}
	get(to) = get(from);
	get(from) = ChessPiece(EMPTY, false);
}


void ChessBoard::makeMove(const Move& move) {
	const BoardPosition from = move.from;
	const BoardPosition to = move.to;

	enpassPawn = BoardPosition(-1, -1);
	const ChessPiece pieceFrom = get(from);
	if (pieceFrom.is(PAWN)) {
		if (pieceFrom.white) {
			// if on the second rank
			if (from.row == 1) {
				// if jump two spaces
				if ((to.row - from.row) == 2) {
					enpassPawn = to;
				}
			}
			else if (move.enpassant) {
				get(move.to.row - 1, move.to.col) = ChessPiece(EMPTY, false);
			}
		}
		else {
			// if on the 7th rank
			if (from.row == (BOARD_SIZE - 2)) {
				// if jump two spaces
				if ((from.row - to.row) == 2) {
					enpassPawn = to;
				}
			}
			else if (move.enpassant) {
				get(move.to.row + 1, move.to.col) = ChessPiece(EMPTY, false);
			}
		}

		if (move.promotionTo != EMPTY) {
			get(to) = ChessPiece(move.promotionTo, pieceFrom.white);
			get(from) = ChessPiece(EMPTY, false);
			return;
		}

	}
	else if (pieceFrom.is(KING)) {
		if (pieceFrom.white) {
			if (move.castlingKingSide) {
				movePiece(BoardPosition(0, 7), move.to + BoardPosition(0, -1));
			}
			else if (move.castlingQueenSide) {
				movePiece(BoardPosition(0, 0), move.to + BoardPosition(0, 1));
			}
		}
		else {
			if (move.castlingKingSide) {
				movePiece(BoardPosition(7, 7), move.to + BoardPosition(0, -1));
			}
			else if (move.castlingQueenSide) {
				movePiece(BoardPosition(7, 0), move.to + BoardPosition(0, 1));
			}
		}

	}
	movePiece(from, to);
}


void ChessBoard::debugLegalMoves(const BoardPosition& from) {
	std::list<Move> possibleMoves;
	getPossibleMoves(from, possibleMoves);

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			BoardPosition to(i, j);
			if (isLegal(from, to)) {

				bool foundOne = false;

				for (const Move& possibleMove : possibleMoves) {
					if (possibleMove.to == to) {
						foundOne = true;
					}
				}

				if (!foundOne) {
					cout << "Found a mismatch in allowed moves.\n";
					cout << "name: " << getPieceName(get(from)) << "\n";
					cout << "from: " << from << "\n";
					cout << "to:   " << to << "\n";
					cout << "\n";
				}

			}
		}
	}

}

