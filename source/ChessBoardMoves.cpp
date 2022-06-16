#include"ChessBoard.h"

using std::min;


template<bool white>
void ChessBoard::getPawnMoves(const BoardPosition pos, vector<BoardPosition>& moves) {

	const int row = pos.row;
	const int col = pos.col;

	// if on first rank
	if constexpr (white) {

		// pawns can capture pieces one space diagonally
		if (row != 7) {
			BoardPosition takeRight(row + 1, col + 1);
			if (pieceIsBlack(get(takeRight))) {
				moves.push_back(takeRight);
			}
			BoardPosition takeLeft(row + 1, col - 1);
			if (pieceIsBlack(get(takeLeft))) {
				moves.push_back(takeLeft);
			}
		}

		// else if there is a en passantable

		// if on second rank
		if (row == 1) {
			// if place in front 
			if (isEmpty(row + 1, col)) {
				moves.emplace_back(BoardPosition(row + 1, col));
				if (isEmpty(row + 2, col)) {
					moves.emplace_back(BoardPosition(row + 2, col));
				}
			}
		}
	}
	else {

		// pawns can capture pieces one space diagonally
		if (row != 0) {
			BoardPosition takeRight(row - 1, col + 1);
			if (pieceIsWhite(get(takeRight))) {
				moves.push_back(takeRight);
			}
			BoardPosition takeLeft(row - 1, col - 1);
			if (pieceIsWhite(get(takeLeft))) {
				moves.push_back(takeLeft);
			}
		}

		// if on second to last rank
		if (row == 6) {
			// if place in front
			if (isEmpty(row - 1, col)) {
				moves.emplace_back(BoardPosition(row - 1, col));

				if (isEmpty(row - 2, col)) {
					moves.emplace_back(BoardPosition(row - 2, col));
				}
			}
		}
	}

}

template<bool white>
void ChessBoard::getRookMoves(const BoardPosition pos, vector<BoardPosition>& moves) {

	const int row = pos.row;
	const int col = pos.col;

	for (int i = 1; i < (int(Constants::BOARD_SIDE_LENGTH) - col); i++) {
		BoardPosition temp(row, col + i);
		if (isEmpty(temp)) {
			moves.push_back(temp);
		}
		else {
			break;
		}
	}

	for (int i = 1; i <= col; i++) {
		BoardPosition temp(row, col - i);
		if (isEmpty(temp)) {
			moves.push_back(temp);
		}
		else {
			break;
		}
	}

	for (int i = 1; i < (int(Constants::BOARD_SIDE_LENGTH) - row); i++) {
		BoardPosition temp(row + i, col);
		if (isEmpty(temp)) {
			moves.push_back(temp);
		}
		else {
			break;
		}
	}

	for (int i = 1; i <= row; i++) {
		BoardPosition temp(row - i, col);
		if (isEmpty(temp)) {
			moves.push_back(temp);
		}
		else {
			break;
		}
	}

}

template<bool white>
void ChessBoard::getKnightMoves(const BoardPosition pos, vector<BoardPosition>& moves) {

	const int row = pos.row;
	const int col = pos.col;

	BoardPosition temp(0, 0);

	temp = BoardPosition(row + 1, col + 2);
	if (isEmpty(temp)) {
		moves.push_back(temp);
	}

	temp = BoardPosition(row + 2, col + 1);
	if (isEmpty(temp)) {
		moves.push_back(temp);
	}

	temp = BoardPosition(row + 1, col - 2);
	if (isEmpty(temp)) {
		moves.push_back(temp);
	}

	temp = BoardPosition(row + 2, col - 1);
	if (isEmpty(temp)) {
		moves.push_back(temp);
	}

	temp = BoardPosition(row - 1, col + 2);
	if (isEmpty(temp)) {
		moves.push_back(temp);
	}

	temp = BoardPosition(row - 2, col + 1);
	if (isEmpty(temp)) {
		moves.push_back(temp);
	}

	temp = BoardPosition(row - 1, col - 2);
	if (isEmpty(temp)) {
		moves.push_back(temp);
	}

	temp = BoardPosition(row - 2, col - 1);
	if (isEmpty(temp)) {
		moves.push_back(temp);
	}

}

template<bool white>
void ChessBoard::getBishopMoves(const BoardPosition pos, vector<BoardPosition>& moves) {

	const int row = pos.row;
	const int col = pos.col;

	int rowsTop = Constants::BOARD_SIDE_LENGTH - row;
	int colsRight = Constants::BOARD_SIDE_LENGTH - col;

	for (int i = 1; i < min(rowsTop, colsRight); i++) {
		BoardPosition temp(row + i, col + i);
		if (isEmpty(temp)) {
			moves.push_back(temp);
		}
	}

	for (int i = 1; i < min(row, col); i++) {
		BoardPosition temp(row - i, col - i);
		if (isEmpty(temp)) {
			moves.push_back(temp);
		}
	}

	for (int i = 1; i < min(rowsTop, col); i++) {
		BoardPosition temp(row + i, col - i);
		if (isEmpty(temp)) {
			moves.push_back(temp);
		}
	}

	for (int i = 1; i < min(row, colsRight); i++) {
		BoardPosition temp(row - i, col + i);
		if (isEmpty(temp)) {
			moves.push_back(temp);
		}
	}


}

template<bool white>
void ChessBoard::getQueenMoves(const BoardPosition pos, vector<BoardPosition>& moves) {
	getRookMoves<white>(pos, moves);
	getBishopMoves<white>(pos, moves);
}

template<bool white>
void ChessBoard::getKingMoves(const BoardPosition pos, vector<BoardPosition>& moves) {

	const int row = pos.row;
	const int col = pos.col;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i != j) {
				BoardPosition pos(row + i, col + j);
				if (isEmpty(pos)) {
					moves.push_back(pos);
				}
			}
		}
	}

	if constexpr (white) {
		if (!wKingHasMoved) {
			if (!wkRookHasMoved) {
				if (isEmpty(0, 5) && isEmpty(0, 6)) {
					moves.push_back(BoardPosition(0, 6));
				}
			}
			if (!wqRookHasMoved) {
				if (isEmpty(0, 1) && isEmpty(0, 2) && isEmpty(0, 3)) {
					moves.push_back(BoardPosition(0, 2));
				}
			}
		}
	}
	else {
		if (!bKingHasMoved) {
			if (!bkRookHasMoved) {
				if (isEmpty(7, 5) && isEmpty(7, 6)) {
					moves.push_back(BoardPosition(7, 6));
				}
			}
			if (!bqRookHasMoved) {
				if (isEmpty(7, 1) && isEmpty(7, 2) && isEmpty(7, 3)) {
					moves.push_back(BoardPosition(7, 2));
				}
			}
		}
	}

}




void ChessBoard::getMoves(const BoardPosition pos, vector<BoardPosition>& moves) {

	ChessPiece pFrom = get(pos);

	switch (pFrom)
	{
	case ChessPiece::WHITE_PAWN:
		getPawnMoves<true>(pos, moves);
		break;
	case ChessPiece::WHITE_ROOK:
		getRookMoves<true>(pos, moves);
		break;
	case ChessPiece::WHITE_KNIGHT:
		getKnightMoves<true>(pos, moves);
		break;
	case ChessPiece::WHITE_BISHOP:
		getBishopMoves<true>(pos, moves);
		break;
	case ChessPiece::WHITE_QUEEN:
		getQueenMoves<true>(pos, moves);
		break;
	case ChessPiece::WHITE_KING:
		getKingMoves<true>(pos, moves);
		break;
	case ChessPiece::BLACK_PAWN:
		getPawnMoves<false>(pos, moves);
		break;
	case ChessPiece::BLACK_ROOK:
		getRookMoves<false>(pos, moves);
		break;
	case ChessPiece::BLACK_KNIGHT:
		getKnightMoves<false>(pos, moves);
		break;
	case ChessPiece::BLACK_BISHOP:
		getBishopMoves<false>(pos, moves);
		break;
	case ChessPiece::BLACK_QUEEN:
		getQueenMoves<false>(pos, moves);
		break;
	case ChessPiece::BLACK_KING:
		getKingMoves<false>(pos, moves);
		break;
	default:
		break;
	}

}