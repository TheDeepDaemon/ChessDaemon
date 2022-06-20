#include"ChessBoard.h"


using std::abs;


int sign(const int x) {
	return (x > 0) - (x < 0);
}




inline bool ChessBoard::isLegalPawn(
	const BoardPosition& from, const BoardPosition& to,
	const BoardPosition& move, const bool white,
	const ChessPiece& pieceTo) {

	if (white) {
		if (move.row == 1) {
			if (move.col == 0 && pieceTo.is(EMPTY)) {
				return true;
			}
			else if (abs(move.col) == 1) {

				if (pieceTo.isNot(EMPTY)) {
					return true;
				}
				else if (BoardPosition(to.row - 1, to.col) == enpassPawn) {
					return true;
				}
			}
		}
		else if (
			(move.row == 2) &&
			(from.row == 1) &&
			(move.col == 0) &&
			pieceTo.is(EMPTY) &&
			get(from.row + 1, from.col).is(EMPTY)) {
			return true;
		}
	}
	else {
		if (move.row == -1) {
			if (move.col == 0 && pieceTo.is(EMPTY)) {
				return true;
			}
			else if (abs(move.col) == 1) {
				if (pieceTo.isNot(EMPTY)) {
					return true;
				}
				else if (BoardPosition(to.row + 1, to.col) == enpassPawn) {
					return true;
				}
			}
		}
		else if (
			(move.row == -2) &&
			(from.row == (BOARD_SIZE - 2)) &&
			(move.col == 0) &&
			pieceTo.is(EMPTY) &&
			get(from.row - 1, from.col).is(EMPTY)) {
			return true;
		}
	}
	return false;
}

inline bool ChessBoard::isLegalRook(
	const BoardPosition& from, const BoardPosition& to,
	const BoardPosition& move) {

	if (move.row == 0) {

		int dir = sign(move.col);

		for (int i = dir; i != move.col; i += dir) {
			if (get(from.row, from.col + i).isNot(EMPTY)) {
				return false;
			}
		}

		return true;
	}
	else if (move.col == 0) {

		int dir = sign(move.row);

		for (int i = dir; i != move.row; i += dir) {
			if (get(from.row + i, from.col).isNot(EMPTY)) {
				return false;
			}
		}

		return true;
	}
	return false;
}

inline bool ChessBoard::isLegalKnight(
	const BoardPosition& from, const BoardPosition& to,
	const BoardPosition& move) {

	const int absR = abs(move.row);
	const int absC = abs(move.col);

	if ((absR + absC) == 3) {
		if (absR != 0 && absC != 0) {
			return true;
		}
	}

	return false;
}

inline bool ChessBoard::isLegalBishop(
	const BoardPosition& from, const BoardPosition& to,
	const BoardPosition& move) {

	if (abs(move.row) == abs(move.col)) {
		const int colDir = sign(move.col);
		const int rowDir = sign(move.row);

		for (int i = rowDir, j = colDir;
			(i != move.row) && (j != move.col); i += rowDir, j += colDir) {
			if (get(from.row + i, from.col + j).isNot(EMPTY)) {
				return false;
			}
		}

		return true;
	}
	return false;
}

inline bool ChessBoard::isLegalQueen(
	const BoardPosition& from, const BoardPosition& to,
	const BoardPosition& move) {
	return isLegalBishop(from, to, move) || isLegalRook(from, to, move);
}

inline bool ChessBoard::isLegalKing(
	const BoardPosition& from, const BoardPosition& to,
	const BoardPosition& move) {
	if (abs(move.col) <= 1 && abs(move.row) <= 1) {
		return true;
	}

	// ignore this branch if not a castling move
	if (abs(move.col) == 2 && (move.row == 0)) {
		if (get(from).white) {
			// if the king hasn't moved
			if (!whiteKingHasMoved) {
				// if in the positive direction, kingside
				if (move.col == 2) {
					// if the kingside rook has not moved
					if (!wkRookHasMoved) {
						return clearToCastle(true, from, sign(move.col));
					}
				}
				else {
					// if the queenside rook has not moved
					if (!wqRookHasMoved) {
						return clearToCastle(true, from, sign(move.col));
					}
				}
			}
		}
		else {
			if (!blackKingHasMoved) {
				// if in the positive direction, kingside
				if (move.col == 2) {
					// if the kingside rook has not moved
					if (!bkRookHasMoved) {
						return clearToCastle(false, from, sign(move.col));
					}
				}
				else {
					// if the queenside rook has not moved
					if (!bqRookHasMoved) {
						return clearToCastle(false, from, sign(move.col));
					}
				}
			}
		}
	}

	return false;
}



bool ChessBoard::isLegal(const BoardPosition& from, const BoardPosition& to) {
	// only continue if both are valid positions
	if (!(isValid(from) && isValid(to))) {
		return false;
	}

	// you can't move to the same square
	if (from == to) {
		return false;
	}

	ChessPiece pieceFrom = get(from);

	// if trying to pick up empty, skip the rest
	if (pieceFrom.is(EMPTY)) {
		return false;
	}

	ChessPiece pieceTo = get(to);

	// make sure it is moving onto an opposite color piece
	// or an empty square
	if (pieceTo.isNot(EMPTY)) {
		if (pieceFrom.white == pieceTo.white) {
			return false;
		}
	}

	bool white = pieceFrom.white;

	BoardPosition move = to - from;

	switch (pieceFrom.type)
	{
	case PAWN:
		return isLegalPawn(from, to, move, white, pieceTo);
	case ROOK:
		return isLegalRook(from, to, move);
	case KNIGHT:
		return isLegalKnight(from, to, move);
	case BISHOP:
		return isLegalBishop(from, to, move);
	case QUEEN:
		return isLegalQueen(from, to, move);
	case KING:
		return isLegalKing(from, to, move);
	default:
		return false;
	}
}


template<int rowDir, int colDir>
ChessPiece ChessBoard::lookInDirection(const BoardPosition& from) {
	for (int i = 1; i < BOARD_SIZE; i++) {
		BoardPosition bCoord(from.row + (i * rowDir), from.col + (i * colDir));
		if (ChessBoard::isValid(bCoord)) {
			ChessPiece piece = get(bCoord);
			if (piece.isNot(EMPTY)) {
				return piece;
			}
		}
		else {
			break;
		}
	}
	return ChessPiece();
}



bool isEither(const ChessPiece piece, const ChessPiece p1, const ChessPiece p2) {
	return piece.is(p1) || piece.is(p2);
}


bool ChessBoard::isPiece(const int row, const int col, const ChessPiece piece) {
	return isValid(row, col) && get(row, col).is(piece);
}


bool ChessBoard::isPiece(const BoardPosition& pos, const ChessPiece piece) {
	return isValid(pos) && get(pos).is(piece);
}


template<int rowDir, int colDir>
bool ChessBoard::isKnight(const BoardPosition& from, const ChessPiece& oppositeKnight) {
	BoardPosition pos1 = from + BoardPosition(1 * rowDir, 2 * colDir);
	BoardPosition pos2 = from + BoardPosition(2 * rowDir, 1 * colDir);
	if (isPiece(pos1, oppositeKnight)) {
		return true;
	}
	if (isPiece(pos2, oppositeKnight)) {
		return true;
	}
	return false;
}





bool ChessBoard::isKingInCheck(const BoardPosition& pos) {
	const bool isWhite = get(pos).white;
	const bool opposite = !isWhite;

	// look for bishop
	ChessPiece oppositeQueen(QUEEN, opposite);
	ChessPiece oppositeBishop(BISHOP, opposite);
	if (isEither(lookInDirection<1, 1>(pos), oppositeQueen, oppositeBishop)) {
		return true;
	}
	if (isEither(lookInDirection<-1, 1>(pos), oppositeQueen, oppositeBishop)) {
		return true;
	}
	if (isEither(lookInDirection<1, -1>(pos), oppositeQueen, oppositeBishop)) {
		return true;
	}
	if (isEither(lookInDirection<-1, -1>(pos), oppositeQueen, oppositeBishop)) {
		return true;
	}

	// look for rook
	ChessPiece oppositeRook(ROOK, opposite);
	if (isEither(lookInDirection<0, 1>(pos), oppositeQueen, oppositeRook)) {
		return true;
	}
	if (isEither(lookInDirection<1, 0>(pos), oppositeQueen, oppositeRook)) {
		return true;
	}
	if (isEither(lookInDirection<0, -1>(pos), oppositeQueen, oppositeRook)) {
		return true;
	}
	if (isEither(lookInDirection<-1, 0>(pos), oppositeQueen, oppositeRook)) {
		return true;
	}

	// look for knight
	ChessPiece oppositeKnight(KNIGHT, opposite);
	if (isKnight<1, 1>(pos, oppositeKnight)) {
		return true;
	}
	if (isKnight<-1, 1>(pos, oppositeKnight)) {
		return true;
	}
	if (isKnight<1, -1>(pos, oppositeKnight)) {
		return true;
	}
	if (isKnight<-1, -1>(pos, oppositeKnight)) {
		return true;
	}

	// look for pawn
	ChessPiece oppositePawn(PAWN, opposite);
	if (isWhite) {
		if (isPiece(pos.row + 1, pos.col + 1, oppositePawn) ||
			isPiece(pos.row + 1, pos.col - 1, oppositePawn)) {
			return true;
		}
	}
	else {
		if (isPiece(pos.row - 1, pos.col + 1, oppositePawn) ||
			isPiece(pos.row - 1, pos.col - 1, oppositePawn)) {
			return true;
		}
	}

	// look for king
	if (isValid(whiteKing) && isValid(blackKing)) {
		BoardPosition distToKing = whiteKing - blackKing;
		if ((abs(distToKing.col) <= 1) &&
			(abs(distToKing.row) <= 1) &&
			(whiteKing != blackKing)) {
			return true;
		}
	}

	return false;
}


bool ChessBoard::isKingInCheck(const bool white) {
	if (white) {
		if (isValid(whiteKing)) {
			return isKingInCheck(whiteKing);
		}
		return false;
	}
	else {
		if (isValid(blackKing)) {
			return isKingInCheck(blackKing);
		}
		return false;
	}
}




inline bool ChessBoard::isColorPiece(const int row, const int col, const bool isWhite) {
	if (isValid(row, col)) {
		ChessPiece piece = get(row, col);
		return piece.isNot(EMPTY) && piece.white == isWhite;
	}
	return false;
}


inline bool ChessBoard::isWhitePiece(const int row, const int col) {
	return isColorPiece(row, col, true);
}

inline bool ChessBoard::isBlackPiece(const int row, const int col) {
	return isColorPiece(row, col, false);
}

inline bool ChessBoard::isEmpty(const int row, const int col) {
	if (ChessBoard::isValid(row, col)) {
		return get(row, col).is(EMPTY);
	}
	return false;
}


inline void ChessBoard::addIfIsColor(
	const BoardPosition& from, const int row, const int col, const bool isWhite, std::list<Move>& moves) {
	if (isColorPiece(row, col, isWhite)) {
		moves.push_back(Move(from, BoardPosition(row, col)));
	}
}




inline void ChessBoard::addIfIsColorOrEmpty(
	const BoardPosition& from, const int row, const int col, const bool isWhite, std::list<Move>& moves) {
	if (ChessBoard::isValid(row, col)) {
		ChessPiece piece = get(row, col);
		if (piece.is(EMPTY) || (piece.white == isWhite)) {
			moves.push_back(Move(from, BoardPosition(row, col)));
		}
	}
}


template<int rowDir, int colDir>
void ChessBoard::addMovesInDirection(
	const BoardPosition& from, const bool isWhite, std::list<Move>& moves) {

	for (int i = 1; i < BOARD_SIZE; i++) {
		BoardPosition to(from.row + (i * rowDir), from.col + (i * colDir));
		if (ChessBoard::isValid(to)) {

			ChessPiece piece = get(to);

			if (piece.is(EMPTY)) {
				moves.push_back(Move(from, to));
			}
			else {
				if (isWhite != piece.white) {
					moves.push_back(Move(from, to));
				}
				return;
			}
		}
		else {
			return;
		}
	}
}


void addPawnPromotions(const Move& move, std::list<Move>& moves) {
	Move queenPromotion = move;
	queenPromotion.promotionTo = QUEEN;
	Move rookPromotion = move;
	rookPromotion.promotionTo = ROOK;
	Move bishopPromotion = move;
	bishopPromotion.promotionTo = BISHOP;
	Move knightPromotion = move;
	knightPromotion.promotionTo = KNIGHT;

	moves.push_back(queenPromotion);
	moves.push_back(rookPromotion);
	moves.push_back(bishopPromotion);
	moves.push_back(knightPromotion);
}


template<bool isWhite>
void addPawnMove(const Move& move, std::list<Move>& moves) {
	const int promotionRow = isWhite ? (BOARD_SIZE - 1) : 0;
	if (move.to.row == promotionRow) {
		addPawnPromotions(move, moves);
	}
	else {
		moves.push_back(move);
	}
}


void ChessBoard::getPawnMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves) {

	if (isWhite) {
		const int forwardRow = pos.row + 1;

		// pawn capture
		if (isBlackPiece(forwardRow, pos.col + 1)) {
			addPawnMove<true>(Move(pos, BoardPosition(forwardRow, pos.col + 1)), moves);
		}
		if (isBlackPiece(forwardRow, pos.col - 1)) {
			addPawnMove<true>(Move(pos, BoardPosition(forwardRow, pos.col - 1)), moves);
		}

		// one or two spaces forward
		if (isEmpty(forwardRow, pos.col)) {
			addPawnMove<true>(Move(pos, BoardPosition(forwardRow, pos.col)), moves);
			if ((pos.row == 1) && isEmpty(pos.row + 2, pos.col)) {
				addPawnMove<true>(Move(pos, BoardPosition(pos.row + 2, pos.col)), moves);
			}
		}

		// en passant
		if (enpassPawn.isAt(pos.row, pos.col + 1)) {
			Move move(pos, BoardPosition(forwardRow, pos.col + 1));
			move.enpassant = true;
			moves.push_back(move);
		}
		else if (enpassPawn.isAt(pos.row, pos.col - 1)) {
			Move move(pos, BoardPosition(forwardRow, pos.col - 1));
			move.enpassant = true;
			moves.push_back(move);
		}
	}
	else {
		const int forwardRow = pos.row - 1;

		// pawn capture
		if (isWhitePiece(forwardRow, pos.col + 1)) {
			addPawnMove<false>(Move(pos, BoardPosition(forwardRow, pos.col + 1)), moves);
		}
		if (isWhitePiece(forwardRow, pos.col - 1)) {
			addPawnMove<false>(Move(pos, BoardPosition(forwardRow, pos.col - 1)), moves);
		}

		// one or two spaces forward
		if (isEmpty(forwardRow, pos.col)) {
			addPawnMove<false>(Move(pos, BoardPosition(forwardRow, pos.col)), moves);
			if ((pos.row == (BOARD_SIZE - 2)) && isEmpty(pos.row - 2, pos.col)) {
				addPawnMove<false>(Move(pos, BoardPosition(pos.row - 2, pos.col)), moves);
			}
		}

		// en passant
		if (enpassPawn.isAt(pos.row, pos.col + 1)) {
			Move move(pos, BoardPosition(forwardRow, pos.col + 1));
			move.enpassant = true;
			moves.push_back(move);
		}
		else if (enpassPawn.isAt(pos.row, pos.col - 1)) {
			Move move(pos, BoardPosition(forwardRow, pos.col - 1));
			move.enpassant = true;
			moves.push_back(move);
		}
	}

}

void ChessBoard::getRookMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves) {
	addMovesInDirection<1, 0>(pos, isWhite, moves);
	addMovesInDirection<0, 1>(pos, isWhite, moves);
	addMovesInDirection<-1, 0>(pos, isWhite, moves);
	addMovesInDirection<0, -1>(pos, isWhite, moves);
}

void ChessBoard::getKnightMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves) {
	const bool opposite = !isWhite;
	const int r = pos.row;
	const int c = pos.col;
	addIfIsColorOrEmpty(pos, r + 1, c + 2, opposite, moves);
	addIfIsColorOrEmpty(pos, r + 2, c + 1, opposite, moves);
	addIfIsColorOrEmpty(pos, r - 1, c + 2, opposite, moves);
	addIfIsColorOrEmpty(pos, r - 2, c + 1, opposite, moves);
	addIfIsColorOrEmpty(pos, r + 1, c - 2, opposite, moves);
	addIfIsColorOrEmpty(pos, r + 2, c - 1, opposite, moves);
	addIfIsColorOrEmpty(pos, r - 1, c - 2, opposite, moves);
	addIfIsColorOrEmpty(pos, r - 2, c - 1, opposite, moves);
}

void ChessBoard::getBishopMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves) {
	addMovesInDirection<1, 1>(pos, isWhite, moves);
	addMovesInDirection<-1, 1>(pos, isWhite, moves);
	addMovesInDirection<1, -1>(pos, isWhite, moves);
	addMovesInDirection<-1, -1>(pos, isWhite, moves);
}

void ChessBoard::getQueenMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves) {
	getRookMoves(pos, isWhite, moves);
	getBishopMoves(pos, isWhite, moves);
}

// can the king move through this square?
bool ChessBoard::isClearForKing(const bool isWhite, const BoardPosition& kingPos, const int row, const int col) {
	const BoardPosition to(row, col);
	// if this square is empty
	if (get(to).is(EMPTY)) {
		// and the king being there would not put it in check
		if (!wouldBeInCheck(isWhite, Move(kingPos, to))) {
			return true;
		}
	}
	return false;
}

// checks if the squares between the king and rook are clear, so you can castle
bool ChessBoard::clearToCastle(const bool isWhite, const BoardPosition& kingPos, const int direction) {
	if (isClearForKing(isWhite, kingPos, kingPos.row, kingPos.col + direction) &&
		isClearForKing(isWhite, kingPos, kingPos.row, kingPos.col + (2 * direction))) {
		return true;
	}
	return false;
}


void ChessBoard::getKingMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves) {
	const bool opposite = !isWhite;
	const int r = pos.row;
	const int c = pos.col;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i != 0 || j != 0) {
				addIfIsColorOrEmpty(pos, r + i, c + j, opposite, moves);
			}
		}
	}

	if (isWhite) {
		if (!whiteKingHasMoved) {
			if (!wkRookHasMoved) {
				if (clearToCastle(true, pos, 1)) {
					Move move(pos, pos + BoardPosition(0, 2));
					move.castlingKingSide = true;
					moves.push_back(move);
				}
			}
			if (!wqRookHasMoved) {
				if (clearToCastle(true, pos, -1)) {
					Move move(pos, pos + BoardPosition(0, -2));
					move.castlingQueenSide = true;
					moves.push_back(move);
				}
			}
		}
	}
	else {
		if (!blackKingHasMoved) {
			if (!bkRookHasMoved) {
				if (clearToCastle(false, pos, 1)) {
					Move move(pos, pos + BoardPosition(0, 2));
					move.castlingKingSide = true;
					moves.push_back(move);
				}
			}
			if (!bqRookHasMoved) {
				if (clearToCastle(false, pos, -1)) {
					Move move(pos, pos + BoardPosition(0, -2));
					move.castlingQueenSide = true;
					moves.push_back(move);
				}
			}
		}
	}

}


void ChessBoard::getPossibleMoves(const BoardPosition& pos, std::list<Move>& moves) {

	const ChessPiece piece = get(pos);

	switch (piece.type)
	{
	case PAWN:
		getPawnMoves(pos, piece.white, moves);
		return;
	case ROOK:
		getRookMoves(pos, piece.white, moves);
		return;
	case KNIGHT:
		getKnightMoves(pos, piece.white, moves);
		return;
	case BISHOP:
		getBishopMoves(pos, piece.white, moves);
		return;
	case QUEEN:
		getQueenMoves(pos, piece.white, moves);
		return;
	case KING:
		getKingMoves(pos, piece.white, moves);
		return;
	default:
		return;
	}
}



std::list<Move> ChessBoard::getAllPossibleMoves(const bool white) {
	std::list<Move> moves;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			const int row = i;
			const int col = j;
			ChessPiece piece = get(row, col);
			if (piece.isNot(EMPTY) && (piece.white == white)) {
				std::list<Move> thisMoves;
				BoardPosition from(row, col);
				getPossibleMoves(from, thisMoves);
				for (const Move& move : thisMoves) {
					moves.push_back(move);
				}
			}
		}
	}
	return moves;
}



bool ChessBoard::wouldBeInCheck(const bool white, const Move& move) {
	bool check;

	// keep copies of the pieces in their old positions
	const ChessPiece pieceTo = get(move.to);
	const ChessPiece pieceFrom = get(move.from);

	// keep a copy of the current state
	const BoardState currentState = *this;

	// make a move,
	// then see if the king is still in check
	makeMove(move);
	check = isKingInCheck(white);

	// restore pieces to their old positions
	get(move.to) = pieceTo;
	get(move.from) = pieceFrom;

	// restore special cases
	if (move.enpassant) {
		if (white) {
			get(move.to.row - 1, move.to.col) = ChessPiece(PAWN, false);
		}
		else {
			get(move.to.row + 1, move.to.col) = ChessPiece(PAWN, true);
		}
	}
	else if (move.castlingKingSide) {
		if (white) {
			movePiece(move.to + BoardPosition(0, -1), BoardPosition(0, 7));
		}
		else {
			movePiece(move.to + BoardPosition(0, -1), BoardPosition(7, 7));
		}
	}
	else if (move.castlingQueenSide) {
		if (white) {
			movePiece(move.to + BoardPosition(0, 1), BoardPosition(0, 0));
		}
		else {
			movePiece(move.to + BoardPosition(0, 1), BoardPosition(7, 0));
		}
	}

	// restore state
	*this = currentState;

	return check;
}


bool ChessBoard::isCheckMated(const bool white) {
	if (isKingInCheck(white)) {
		std::list<Move> moves = getAllPossibleMoves(white);
		
		// look through all possible moves that
		// can be made
		for (const Move& move : moves) {
			// if the king would not be in check if
			// this move was made, then there is a
			// way out of check, it is therfore not checkmate
			if (!wouldBeInCheck(white, move)) {
				return false;
			}
		}

		// if the king is in check 
		// and there is not a way out
		return true;
	}
	return false;
}


bool ChessBoard::isDraw(const bool white) {
	if (!isKingInCheck(white)) {
		std::list<Move> moves = getAllPossibleMoves(white);
		
		for (const Move& move : moves) {
			if (!wouldBeInCheck(white, move)) {
				return false;
			}
		}

		return true;
	}
	return false;
}