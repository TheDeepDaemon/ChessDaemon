#include"ChessBoard.h"






// map enum to character for displaying to console
char pieceMap(const ChessPiece p) {
	switch (p) {
	case ChessPiece::EMPTY:
		return ' ';
	case ChessPiece::WHITE_PAWN:
		return 'p';
	case ChessPiece::WHITE_ROOK:
		return 'r';
	case ChessPiece::WHITE_KNIGHT:
		return 'n';
	case ChessPiece::WHITE_BISHOP:
		return 'b';
	case ChessPiece::WHITE_QUEEN:
		return 'q';
	case ChessPiece::WHITE_KING:
		return 'k';
	case ChessPiece::BLACK_PAWN:
		return 'p';
	case ChessPiece::BLACK_ROOK:
		return 'r';
	case ChessPiece::BLACK_KNIGHT:
		return 'n';
	case ChessPiece::BLACK_BISHOP:
		return 'b';
	case ChessPiece::BLACK_QUEEN:
		return 'q';
	case ChessPiece::BLACK_KING:
		return 'k';
	case ChessPiece::NUM_PIECE_TYPES:
		return '-';
	default:
		return ' ';
	}
}



std::ostream& operator<<(std::ostream& os, const ChessPiece p) {
	os << pieceMap(p);
	return os;
}


string toString(const bool b) {
	return b ? "true" : "false";
}


bool pieceIsWhite(ChessPiece p) {
	return (ChessPiece::WHITE_PAWN <= p) && (p <= ChessPiece::WHITE_KING);
}

bool pieceIsBlack(ChessPiece p) {
	return (ChessPiece::BLACK_PAWN <= p) && (p <= ChessPiece::BLACK_KING);
}






// FUNCTIONS FOR CHECKING LEGAL MOVES

template<bool white>
bool ChessBoard::isPawnAttacking(const int rowFrom, const int colFrom) {
	const int rowDir = white ? 1 : -1;
	const ChessPiece p = white ? ChessPiece::BLACK_PAWN : ChessPiece::WHITE_PAWN;

	return (isPiece(rowDir, 1, p) || isPiece(rowDir, -1, p));
}


template<bool white>
bool ChessBoard::isLegalPawn(
	const int rowFrom, const int colFrom, 
	const int rMove, const int cMove, const bool landingOnOpposite) {
	// white pawns can only move up one row
	// black pawns can only move down one row
	const int dir = white ? 1 : -1;

	// if moving 1 row
	if (rMove == dir) {
		// and not moving sideways
		if (cMove == 0) {
			return !landingOnOpposite;
		}
		else { // if moving sideways
			if (landingOnOpposite) {
				if (cMove == 1 || cMove == -1) {
					return true;
				}
			}
			else {

				if constexpr (white) {
					if (cMove == 1 && movedBlackPawn.is(rowFrom, colFrom + 1)) {
						return true;
					}
					else if (cMove == -1 && movedBlackPawn.is(rowFrom, colFrom - 1)) {
						return true;
					}
				}
				else {
					if (cMove == 1 && movedWhitePawn.is(rowFrom, colFrom + 1)) {
						return true;
					}
					else if (cMove == -1 && movedWhitePawn.is(rowFrom, colFrom - 1)) {
						return true;
					}
				}

			}

		}
	}
	// first pawn move means you can move two squares
	else {
		if (
			(rMove == (dir * 2)) &&
			((white && rowFrom == 1) || (!white && rowFrom == 6)) &&
			(cMove == 0) && !landingOnOpposite) {
			return true;
		}
	}

	return false;
}




// looks horizontally or vertically and 
// returns the first piece in that direction
// used to detect if the king is in check
// not used for regular rook moves because you might intercept the wrong piece
template<bool vertical, int dir_>
ChessPiece ChessBoard::firstPieceInRookDir(const int rowFrom, const int colFrom) {
	// make sure you are only incrementing one at a time
	const int dir = int(0 < dir_) - int(dir_ < 0);

	// set the proper range
	const int boundary = (dir == 1) ? 8 : -1;

	// up and down rook movements
	if (vertical) {
		for (int i = rowFrom + dir; i != boundary; i += dir) {
			ChessPiece p = get(i, colFrom);
			if (p != ChessPiece::EMPTY) {
				return p;
			}
		}
	}
	// left and right rook movements
	else {
		for (int i = colFrom + dir; i != boundary; i += dir) {
			ChessPiece p = get(rowFrom, i);
			if (p != ChessPiece::EMPTY) {
				return p;
			}
		}
	}
	return ChessPiece::EMPTY;
}

// check if it is a legal rook move
bool ChessBoard::isLegalRook(
	const int rowFrom, const int colFrom, const int rMove, const int cMove) {

	// rooks only move col wise or row wise, not both
	if (rMove != 0) {
		if (cMove == 0) {
			int dir = sign(rMove);
			int n = rowFrom + rMove; // inefficient, change later
			for (int r = rowFrom+dir; r != n; r+=dir) {
				if (get(r, colFrom) != ChessPiece::EMPTY) {
					return false;
				}
			}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (cMove != 0) {
			int dir = sign(cMove);
			int n = colFrom + cMove; // inefficient, change later
			for (int c = colFrom+dir; c != n; c+=dir) {
				if (get(rowFrom, c) != ChessPiece::EMPTY) {
					return false;
				}
			}
			return true;
		}
		else {
			return false;
		}
	}


}

// return whether a knight is attacking this square
template<bool white>
bool ChessBoard::knightIsAttacking(const int rowFrom, const int colFrom) {
	const ChessPiece knight = white ? ChessPiece::BLACK_KNIGHT : ChessPiece::WHITE_KNIGHT;

	// look for L shapes
	return
		(isPiece(rowFrom + 1, colFrom + 2, knight)) ||
		(isPiece(rowFrom + 2, colFrom + 1, knight)) ||
		(isPiece(rowFrom + 2, colFrom - 1, knight)) ||
		(isPiece(rowFrom + 1, colFrom - 2, knight)) ||
		(isPiece(rowFrom - 1, colFrom - 2, knight)) ||
		(isPiece(rowFrom - 2, colFrom - 1, knight)) ||
		(isPiece(rowFrom - 2, colFrom + 1, knight)) ||
		(isPiece(rowFrom - 1, colFrom + 2, knight));
}

// check if this move is a legal knight move
bool ChessBoard::isLegalKnight(const int rMove, const int cMove) {

	// knights move in both horizontal and vertical
	// directions no matter what
	if (rMove == 0 || cMove == 0) {
		return false;
	}

	// legal knight moves will have horizontal
	// and vertical numbers of moves that
	// add up to three squares total
	// the case of 3 + 0 is ruled out already
	return (abs(rMove) + abs(cMove) == 3);
}


// looks diagonally and returns the first piece in that direction
// used to detect if the king is in check
// not used for regular bishop moves because you might intercept the wrong piece
template<int rDir_, int cDir_>
ChessPiece ChessBoard::firstPieceInBishopDir(const int rowFrom, const int colFrom) {
	// make sure you are only incrementing one at a time
	const int rDir = int(0 < rDir_) - int(rDir_ < 0);
	const int cDir = int(0 < cDir_) - int(cDir_ < 0);

	// set the proper range
	const int rBoundary = rDir == 1 ? 8 : -1;
	const int cBoundary = cDir == 1 ? 8 : -1;

	for (int r = rowFrom + rDir, c = colFrom + cDir; r != rBoundary && c != cBoundary; r += rDir, c += cDir) {
		ChessPiece p = get(r, c);
		if (p != ChessPiece::EMPTY) {
			return p;
		}
	}

	return ChessPiece::EMPTY;
}

// is this a legal bishop move
bool ChessBoard::isLegalBishop(
	const int rowFrom, const int colFrom, const int rMove, const int cMove) {

	// find if the number of moves
	// horizontally is the same
	// as the number of moves vertically
	int rMoveAbs = abs(rMove);
	if (rMoveAbs == abs(cMove)) {
		
		// get the direction of movement
		// horizontally and vertically
		const int r = sign(rMove);
		const int c = sign(cMove);

		// search through spaces between the original and target spots
		// return false if the path is not clear
		// (if one of the squares is not empty)
		for (int i = 1; i < rMoveAbs; i++) {

			// check if the current square is not empty
			if (get(rowFrom + (i * r), colFrom + (i * c)) != ChessPiece::EMPTY) {
				return false;
			}
		}

		// if the path is clear, the move is legal
		return true;
	}
	else {

		// bishops only move diagonally
		// return false if they don't
		return false;
	}
}

bool ChessBoard::isLegalQueen(const int rowFrom, const int colFrom, const int rMove, const int cMove) {

	// the queen moves like the bishop and the rook combined
	return isLegalBishop(rowFrom, colFrom, rMove, cMove) || isLegalRook(rowFrom, colFrom, rMove, cMove);
}

template<bool white>
bool ChessBoard::isLegalKing(const int rowFrom, const int colFrom, const int rMove, const int cMove) {

	// kings only move one space horizontally
	// or vertically.
	// the case of <rMove, cMove> == <0, 0>
	// is ruled out already
	if (abs(rMove) <= 1 && abs(cMove) <= 1) {
		return true;
	}

	const bool kingMoved = white ? wKingHasMoved : bKingHasMoved;

	// castling
	if (!kingMoved && rMove == 0) {
		const ChessPiece kingPiece = white ? ChessPiece::WHITE_KING : ChessPiece::BLACK_KING;
		const ChessPiece rookPiece = white ? ChessPiece::WHITE_ROOK : ChessPiece::BLACK_ROOK;

		// kingside castle
		if (cMove == 2) {
			const bool rookHasMoved = white ? wkRookHasMoved : bkRookHasMoved;

			if (!rookHasMoved) {
				return true;
			}
		}
		// queenside castle
		else if (cMove == -2) {
			const bool rookHasMoved = white ? wqRookHasMoved : bqRookHasMoved;

			if (!rookHasMoved) {
				return true;
			}
		}
	}

	return false;
}



/**
	* Put all of the pieces in their starting positions.
	* Should look like this:
	*         black
	*    r n b q k b n r
	*    p p p p p p p p
	*    0 0 0 0 0 0 0 0
	*    0 0 0 0 0 0 0 0
	*    0 0 0 0 0 0 0 0
	*    0 0 0 0 0 0 0 0
	*    p p p p p p p p
	*    r n b q k b n r
	*         white
	* This function doesn't use
	* loops for the sake of efficiency.
	*/
void ChessBoard::startingPosition() {

	for (int i = 0; i < BOARD_SIZE; i++) {
		board[i] = ChessPiece::EMPTY;
	}

	// set the pawns
	for (int i = 0; i < 8; i++) {
		get(1, i) = ChessPiece::WHITE_PAWN;
	}

	for (int i = 0; i < 8; i++) {
		get(6, i) = ChessPiece::BLACK_PAWN;
	}


	// set the pieces
	get(0, 0) = ChessPiece::WHITE_ROOK;
	get(0, 1) = ChessPiece::WHITE_KNIGHT;
	get(0, 2) = ChessPiece::WHITE_BISHOP;
	get(0, 3) = ChessPiece::WHITE_QUEEN;
	get(0, 4) = ChessPiece::WHITE_KING;
	get(0, 5) = ChessPiece::WHITE_BISHOP;
	get(0, 6) = ChessPiece::WHITE_KNIGHT;
	get(0, 7) = ChessPiece::WHITE_ROOK;

	get(7, 0) = ChessPiece::BLACK_ROOK;
	get(7, 1) = ChessPiece::BLACK_KNIGHT;
	get(7, 2) = ChessPiece::BLACK_BISHOP;
	get(7, 3) = ChessPiece::BLACK_QUEEN;
	get(7, 4) = ChessPiece::BLACK_KING;
	get(7, 5) = ChessPiece::BLACK_BISHOP;
	get(7, 6) = ChessPiece::BLACK_KNIGHT;
	get(7, 7) = ChessPiece::BLACK_ROOK;


	whiteKingPos = BoardPosition(0, 4);
	blackKingPos = BoardPosition(7, 4);

}






// construct the board, set it in it's
// starting position
ChessBoard::ChessBoard() {
	whiteToMove = true;
	memset(board, (int)ChessPiece::EMPTY, BOARD_SIZE);
	// set the board in starting position
	startingPosition();
}


void ChessBoard::displayToConsole() {
	for (int i = BOARD_SIDE_LENGTH - 1; i >= 0; i--) {
		for (int j = 0; j < BOARD_SIDE_LENGTH; j++) {
			cout << get(i, j) << '|';
		}
		cout << '\n';
	}
}


/**
	* Check if a move is legal.
	* Doesn't check to see if the king is in check.
	*
	* whiteToMove: whether it is white's turn. 1 = white, 0 = black
	* rowFrom: the current row of the piece
	* colFrom: the current column of the piece
	* rMove: how many rows to move
	* cMove: how many columns to move
	*/
bool ChessBoard::isLegalMoveNoCheck(
	const int rowFrom, const int colFrom, const int rowTo, const int colTo) {

	const int rMove = rowTo - rowFrom;
	const int cMove = colTo - colFrom;

	// return false if no move is being made
	if (rMove == 0 && cMove == 0) {
		return false;
	}

	// make sure the piece locations exist within the 8x8 range
	if (correctRange((unsigned)rowFrom) && correctRange((unsigned)colFrom) &&
		correctRange((unsigned)rowTo) && correctRange((unsigned)colTo)) {

		ChessPiece pFrom = get(rowFrom, colFrom);

		// make sure you are not 
		// moving from an empty square
		if (ChessPiece::EMPTY == pFrom) {
			return false;
		}

		bool fromWhite = pieceIsWhite(pFrom);
		
		// turn is the same as piece type
		if (whiteToMove != fromWhite) {
			return false;
		}

		// make sure piece is not being placed on same color
		if (get(rowTo, colTo) != ChessPiece::EMPTY) {
			bool toWhite = pieceIsWhite(get(rowTo, colTo));
			if (toWhite == fromWhite) {
				return false;
			}
		}


		// check piece type,
		// then map to function that
		// handles specific piece rules
		switch (pFrom)
		{
		case ChessPiece::WHITE_PAWN:
			return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, pieceIsBlack(get(rowTo, colTo)));
		case ChessPiece::WHITE_ROOK:
			return isLegalRook(rowFrom, colFrom, rMove, cMove);
		case ChessPiece::WHITE_KNIGHT:
			return isLegalKnight(rMove, cMove);
		case ChessPiece::WHITE_BISHOP:
			return isLegalBishop(rowFrom, colFrom, rMove, cMove);
		case ChessPiece::WHITE_QUEEN:
			return isLegalQueen(rowFrom, colFrom, rMove, cMove);
		case ChessPiece::WHITE_KING:
			return isLegalKing<true>(rowFrom, colFrom, rMove, cMove);
		case ChessPiece::BLACK_PAWN:
			return isLegalPawn<false>(rowFrom, colFrom, rMove, cMove, pieceIsWhite(get(rowTo, colTo)));
		case ChessPiece::BLACK_ROOK:
			return isLegalRook(rowFrom, colFrom, rMove, cMove);
		case ChessPiece::BLACK_KNIGHT:
			return isLegalKnight(rMove, cMove);
		case ChessPiece::BLACK_BISHOP:
			return isLegalBishop(rowFrom, colFrom, rMove, cMove);
		case ChessPiece::BLACK_QUEEN:
			return isLegalQueen(rowFrom, colFrom, rMove, cMove);
		case ChessPiece::BLACK_KING:
			return isLegalKing<false>(rowFrom, colFrom, rMove, cMove);
		default:
			return false;
		}

	}
	return false;
}


// return if piece is one of two pieces
bool ChessBoard::isOneOfThese(const ChessPiece pToCheck, const ChessPiece p1, const ChessPiece p2) {
	return (pToCheck == p1 || pToCheck == p2);
}


template<bool white>
bool ChessBoard::isKingInCheck(const int kingRow, const int kingCol) {

	// look for bishop movement
	const ChessPiece bishop = white ? ChessPiece::BLACK_BISHOP : ChessPiece::WHITE_BISHOP;
	const ChessPiece queen = white ? ChessPiece::BLACK_QUEEN : ChessPiece::WHITE_QUEEN;

	// can you move a bishop from the king's
	// location to the location of a queen or bishop?
	// if so, the king is in check
	if (isOneOfThese(firstPieceInBishopDir<1, 1>(kingRow, kingCol), bishop, queen)) {
		return true;
	}
	else if (isOneOfThese(firstPieceInBishopDir<-1, 1>(kingRow, kingCol), bishop, queen)) {
		return true;
	}
	else if (isOneOfThese(firstPieceInBishopDir<-1, -1>(kingRow, kingCol), bishop, queen)) {
		return true;
	}
	else if (isOneOfThese(firstPieceInBishopDir<1, -1>(kingRow, kingCol), bishop, queen)) {
		return true;
	}


	// look for rook movement
	const ChessPiece rook = white ? ChessPiece::BLACK_ROOK : ChessPiece::WHITE_ROOK;


	if (isOneOfThese(firstPieceInBishopDir<true, 1>(kingRow, kingCol), rook, queen)) {
		return true;
	}
	else if (isOneOfThese(firstPieceInBishopDir<true, -1>(kingRow, kingCol), rook, queen)) {
		return true;
	}
	else if (isOneOfThese(firstPieceInBishopDir<false, 1>(kingRow, kingCol), rook, queen)) {
		return true;
	}
	else if (isOneOfThese(firstPieceInBishopDir<false, -1>(kingRow, kingCol), rook, queen)) {
		return true;
	}

	// look for knight
	if (knightIsAttacking<white>(kingRow, kingCol)) {
		return true;
	}
	
	// look for pawn
	if (isPawnAttacking<white>(kingRow, kingCol)) {
		return true;
	}

	return false;
}



template<bool white>
bool ChessBoard::isKingInCheck() {
	if constexpr (white) {
		return isKingInCheck<true>(whiteKingPos.row, whiteKingPos.col);
	}
	else {
		return isKingInCheck<false>(blackKingPos.row, blackKingPos.col);
	}
	
}

template
bool ChessBoard::isKingInCheck<true>();

template
bool ChessBoard::isKingInCheck<false>();



/**
	* Make a move on this board.
	* It is assumed that the move is legal.
	*
	* whiteToMove: whether it is white's turn. 1 = white, 0 = black
	* rowFrom: the current row of the piece
	* colFrom: the current column of the piece
	* rMove: how many rows to move
	* cMove: how many columns to move
	*/
void ChessBoard::makeLegalMoveInternal(
	const int rowFrom, const int colFrom, const int rowTo, const int colTo, const ChessPiece promotionType) {
	const int rMove = rowTo - rowFrom;
	const int cMove = colTo - colFrom;

	// get the piece
	ChessPiece pieceType = get(rowFrom, colFrom);

	// switch for handling special rules
	switch (pieceType) {
	case ChessPiece::WHITE_PAWN:

		// en passant
		if (movedBlackPawn != BoardPosition::null) {
			if (cMove == 1 && movedBlackPawn.is(rowFrom, colFrom + 1)) {
				get(rowFrom, colFrom + 1) = ChessPiece::EMPTY;
				break;
			}
			if (cMove == -1 && movedBlackPawn.is(rowFrom, colFrom - 1)) {
				get(rowFrom, colFrom - 1) = ChessPiece::EMPTY;
				break;
			}
		}

		// promote
		if (rowFrom == 7 && rMove == 1) {
			get(rowFrom, colFrom) = ChessPiece::EMPTY;
			get(rowFrom + rMove, colFrom + cMove) = promotionType;
			break;
		}

		// move two spaces
		if (rMove == 2) {
			movedWhitePawn = BoardPosition(rowTo, colTo);
			break;
		}

		break;

	case ChessPiece::BLACK_PAWN:
		// en passant
		if (movedWhitePawn != BoardPosition::null) {
			if (cMove == 1 && movedBlackPawn.is(rowFrom, colFrom + 1)) {
				get(rowFrom, colFrom + 1) = ChessPiece::EMPTY;
				break;
			}
			if (cMove == -1 && movedBlackPawn.is(rowFrom, colFrom - 1)) {
				get(rowFrom, colFrom - 1) = ChessPiece::EMPTY;
				break;
			}
		}
		

		// promote
		if (rowFrom == 1 && rMove == -1) {
			get(rowFrom, colFrom) = ChessPiece::EMPTY;
			get(rowFrom + rMove, colFrom + cMove) = promotionType;
			break;
		}

		// move two spaces
		if (rMove == -2) {
			movedBlackPawn = BoardPosition(rowTo, colTo);
			break;
		}

		break;

	case ChessPiece::WHITE_KING:

		whiteKingPos = BoardPosition(rowTo, colTo);

		// kingside castle
		if (cMove == 2) {

			// king didn't move and king's rook didn't move
			if (!(wKingHasMoved || wkRookHasMoved)) {
				wKingHasMoved = true;
				wkRookHasMoved = true;
				get(0, 5) = ChessPiece::WHITE_ROOK;
				get(0, 7) = ChessPiece::EMPTY;
				break;
			}
		}
		// queenside castle
		else if (cMove == -2) {

			// king didn't move and queen's rook didn't move
			if (!(wKingHasMoved || wqRookHasMoved)) {
				wKingHasMoved = true;
				wqRookHasMoved = true;
				get(0, 0) = ChessPiece::EMPTY;
				get(0, 1) = ChessPiece::EMPTY;
				get(0, 3) = ChessPiece::WHITE_ROOK;
				break;
			}
		}
		wKingHasMoved = true;
		break;

	case ChessPiece::BLACK_KING:

		blackKingPos = BoardPosition(rowTo, colTo);

		// kingside castle
		if (cMove == 2) {

			// king didn't move and king's rook didn't move
			if (!(bKingHasMoved || bkRookHasMoved)) {
				bKingHasMoved = true;
				bkRookHasMoved = true;
				get(7, 5) = ChessPiece::BLACK_ROOK;
				get(7, 7) = ChessPiece::EMPTY;
				break;
			}
		}
		// queenside castle
		else if (cMove == -2) {

			// king didn't move and queen's rook didn't move
			if (!(bKingHasMoved || bqRookHasMoved)) {
				bKingHasMoved = true;
				bqRookHasMoved = true;
				get(7, 0) = ChessPiece::EMPTY;
				get(7, 1) = ChessPiece::EMPTY;
				get(7, 3) = ChessPiece::BLACK_ROOK;
				break;
			}
		}
		bKingHasMoved = true;
		break;

	case ChessPiece::WHITE_ROOK:
		if (rowFrom == 0) {
			if (colFrom == 7) {
				wkRookHasMoved = true;
			}
			else if (colFrom == 0) {
				wqRookHasMoved = true;
			}
		}
		break;

	case ChessPiece::BLACK_ROOK:
		if (rowFrom == 7) {
			if (colFrom == 7) {
				bkRookHasMoved = true;
			}
			else if (colFrom == 0) {
				bqRookHasMoved = true;
			}
		}
		break;

	}

	get(rowFrom + rMove, colFrom + cMove) = get(rowFrom, colFrom);
	get(rowFrom, colFrom) = ChessPiece::EMPTY;
}



void ChessBoard::makeLegalMove(
	const int rowFrom, const int colFrom,
	const int rowTo, const int colTo, const ChessPiece promotionType) {

	makeLegalMoveInternal(rowFrom, colFrom, rowTo, colTo, promotionType);

	// if a pawn is en passantable, reset it
	if (whiteToMove) {
		if (movedBlackPawn != BoardPosition::null) {
			movedBlackPawn = BoardPosition::null;
		}
	}
	else {
		if (movedWhitePawn != BoardPosition::null) {
			movedWhitePawn = BoardPosition::null;
		}
	}

	// change who's turn it is
	whiteToMove = !whiteToMove;
}




