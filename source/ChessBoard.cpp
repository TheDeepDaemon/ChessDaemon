#include"ChessBoard.h"



// map enum to character for displaying to console
char pieceMap(ChessPiece p) {
	switch (p) {
	case ChessPiece::EMPTY:
		return ' ';
	case ChessPiece::WHITE_PAWN:
		return 'p';
	case ChessPiece::WHITE_EN_PASSANTABLE_PAWN:
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
	case ChessPiece::BLACK_EN_PASSANTABLE_PAWN:
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


bool pieceIsWhite(ChessPiece p) {
	return (ChessPiece::WHITE_PAWN <= p) && (p <= ChessPiece::WHITE_KING);
}

bool pieceIsBlack(ChessPiece p) {
	return (ChessPiece::BLACK_PAWN <= p) && (p <= ChessPiece::BLACK_KING);
}



// access the piece type of a location on the board
// by row and column
__forceinline ChessPiece& ChessBoard::get(unsigned row, unsigned col) {
	return *(board + ((uint64_t)row * BOARD_SIDE_LENGTH) + (uint64_t)col);
}



// FUNCTIONS FOR CHECKING LEGAL MOVES

template<bool white>
bool ChessBoard::isPawnAttacking(int rowFrom, int colFrom) {
	const int rowDir = white ? 1 : -1;
	const ChessPiece p = white ? ChessPiece::BLACK_PAWN : ChessPiece::WHITE_PAWN;

	return (get(rowDir, 1) == p || get(rowDir, -1) == p);
}


template<bool white>
bool ChessBoard::isLegalPawn(int rowFrom, int colFrom, int rMove, int cMove, bool landingOnOpposite) {
	// white pawns can only move up one row
	// black pawns can only move down one row
	const int dir = white ? 1 : -1;

	// if moving 1 row
	if (rMove == dir) {
		// and not moving sideways
		if (cMove == 0) {
			return true;
		}
		else { // if moving sideways
			if (landingOnOpposite) {
				if (cMove == 1 || cMove == -1) {
					return true;
				}
			}
			else {

				const ChessPiece enPas = white ?
					ChessPiece::BLACK_EN_PASSANTABLE_PAWN : ChessPiece::WHITE_EN_PASSANTABLE_PAWN;

				if ((cMove == 1 && get(rowFrom, colFrom + 1) == enPas) ||
					(cMove == -1 && get(rowFrom, colFrom - 1) == enPas)) {
					return true;
				}
			}

		}
	}
	// first pawn move means you can move two squares
	else if (rMove == (dir * 2)) {
		return cMove == 0;
	}

	return false;
}

// looks horizontally or vertically and 
// returns the first piece in that direction
// used to detect if the king is in check
// not used for regular rook moves because you might intercept the wrong piece
template<bool vertical, int dir_>
ChessPiece ChessBoard::firstPieceInRookDir(int rowFrom, int colFrom) {
	// make sure you are only incrementing one at a time
	const int dir = int(0 < dir_) - int(dir_ < 0);

	// set the proper range
	const int boundary = dir == 1 ? 8 : -1;

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
bool ChessBoard::isLegalRook(int rowFrom, int colFrom, int rMove, int cMove) {

	// rooks only move col wise or row wise, not both
	if (rMove != 0) {
		if (cMove == 0) {
			int n = rowFrom + rMove; // inefficient, change later
			for (int r = rowFrom; r < n; r++) {
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
			int n = colFrom + cMove; // inefficient, change later
			for (int c = colFrom; c < n; c++) {
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
bool ChessBoard::knightIsAttacking(int rowFrom, int colFrom) {
	ChessPiece knight = white ? ChessPiece::BLACK_KNIGHT : ChessPiece::WHITE_KNIGHT;

	// look for L shapes
	return
		(get(rowFrom + 1, colFrom + 2) == knight) ||
		(get(rowFrom + 2, colFrom + 1) == knight) ||
		(get(rowFrom + 2, colFrom - 1) == knight) ||
		(get(rowFrom + 1, colFrom - 2) == knight) ||
		(get(rowFrom - 1, colFrom - 2) == knight) ||
		(get(rowFrom - 2, colFrom - 1) == knight) ||
		(get(rowFrom - 2, colFrom + 1) == knight) ||
		(get(rowFrom - 1, colFrom + 2) == knight);
}

// check if this move is a legal knight move
bool ChessBoard::isLegalKnight(int rMove, int cMove) {

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
ChessPiece ChessBoard::firstPieceInBishopDir(int rowFrom, int colFrom) {
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
bool ChessBoard::isLegalBishop(int rowFrom, int colFrom, int rMove, int cMove) {

	// find if the number of moves
	// horizontally is the same 
	// as the number of moves vertically
	if (abs(rMove) == abs(cMove)) {

		// get the direction of movement
		// horizontally and vertically
		int r = sign(rMove);
		int c = sign(cMove);

		// search through spaces between the original and target spots
		// return false if the path is not clear
		// (if one of the squares is not empty)
		for (int i = 1; i < rMove; i++) {

			// check if the current square is empty
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

bool ChessBoard::isLegalQueen(int rowFrom, int colFrom, int rMove, int cMove) {

	// the queen moves like the bishop and the rook combined
	return isLegalBishop(rowFrom, colFrom, rMove, cMove) || isLegalRook(rowFrom, colFrom, rMove, cMove);
}

template<bool white>
bool ChessBoard::isLegalKing(int rowFrom, int colFrom, int rMove, int cMove) {

	const bool kingMoved = white ? wKingHasMoved : bKingHasMoved;

	// castling
	if (!kingMoved) {
		const ChessPiece kingPiece = white ? ChessPiece::WHITE_KING : ChessPiece::BLACK_KING;
		const ChessPiece rookPiece = white ? ChessPiece::WHITE_ROOK : ChessPiece::BLACK_ROOK;
		const int rowToCheck = white ? 0 : 7;

		// kingside castle
		if (cMove == 2) {
			const bool rookHasMoved = white ? wkRookHasMoved : bkRookHasMoved;

			return !rookHasMoved &&
				((get(rowToCheck, 3) == kingPiece) && (get(rowToCheck, 7) == rookPiece));
		}
		// queenside castle
		else if (cMove == -2) {
			const bool rookHasMoved = white ? wqRookHasMoved : bqRookHasMoved;

			return !rookHasMoved &&
				((get(rowToCheck, 3) == kingPiece) && (get(rowToCheck, 0) == rookPiece));
		}
	}

	// kings only move one space horizontally
	// or vertically.
	// the case of <rMove, cMove> == <0, 0> 
	// is ruled out already
	return abs(rMove) <= 1 && abs(cMove) <= 1;
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
	get(1, 0) = ChessPiece::WHITE_PAWN;
	get(1, 1) = ChessPiece::WHITE_PAWN;
	get(1, 2) = ChessPiece::WHITE_PAWN;
	get(1, 3) = ChessPiece::WHITE_PAWN;
	get(1, 4) = ChessPiece::WHITE_PAWN;
	get(1, 5) = ChessPiece::WHITE_PAWN;
	get(1, 6) = ChessPiece::WHITE_PAWN;
	get(1, 7) = ChessPiece::WHITE_PAWN;

	get(6, 0) = ChessPiece::BLACK_PAWN;
	get(6, 1) = ChessPiece::BLACK_PAWN;
	get(6, 2) = ChessPiece::BLACK_PAWN;
	get(6, 3) = ChessPiece::BLACK_PAWN;
	get(6, 4) = ChessPiece::BLACK_PAWN;
	get(6, 5) = ChessPiece::BLACK_PAWN;
	get(6, 6) = ChessPiece::BLACK_PAWN;
	get(6, 7) = ChessPiece::BLACK_PAWN;


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

}




// can x be a real, valid board coordinate?
bool ChessBoard::correctRange(unsigned x) {
	return (x < BOARD_SIDE_LENGTH);
}


// construct the board, set it in it's
// starting position
ChessBoard::ChessBoard() {
	memset(board, (int)ChessPiece::EMPTY, BOARD_SIZE);
	// set the board in starting position
	startingPosition();
}


void ChessBoard::displayToConsole() {
	for (int i = BOARD_SIDE_LENGTH - 1; i >= 0; i--) {
		for (int j = 0; j < BOARD_SIDE_LENGTH; j++) {
			cout << pieceMap(get(i, j)) << '|';
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
bool ChessBoard::isLegalMoveNoCheck(bool whiteToMove,
	int rowFrom, int colFrom, int rMove, int cMove) {
	int rowTo = rowFrom + rMove;
	int colTo = colFrom + cMove;

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
		bool toWhite = pieceIsWhite(get(rowTo, colTo));

		// make sure piece is not being placed on same color
		// and turn is the same as piece type
		if ((fromWhite == toWhite) || (whiteToMove != fromWhite)) {
			return false;
		}

		// check piece type, 
		// then map to function that
		// handles specific piece rules
		switch (pFrom)
		{
		case ChessPiece::WHITE_PAWN:
			return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, fromWhite != toWhite);
		case ChessPiece::WHITE_EN_PASSANTABLE_PAWN:
			return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, fromWhite != toWhite);
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
			return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, fromWhite != toWhite);
		case ChessPiece::BLACK_EN_PASSANTABLE_PAWN:
			return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, fromWhite != toWhite);
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
bool ChessBoard::isOneOfThese(ChessPiece pToCheck, ChessPiece p1, ChessPiece p2) {
	return (pToCheck == p1 || pToCheck == p2);
}


template<bool white>
bool ChessBoard::isKingInCheck(int kingRow, int kingCol) {

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
void ChessBoard::makeLegalMove(bool whiteToMove,
	int rowFrom, int colFrom, int rMove, int cMove, ChessPiece promotionType) {
	ChessPiece pieceType = get(rowFrom, colFrom);

	// switch for handling special rules
	switch (pieceType) {
	case ChessPiece::WHITE_PAWN:
		// en passant
		if (cMove == 1 &&
			get(rowFrom, colFrom + 1) == ChessPiece::BLACK_EN_PASSANTABLE_PAWN) {
			get(rowFrom + 1, colFrom + 1) = ChessPiece::WHITE_PAWN;
			get(rowFrom, colFrom + 1) = ChessPiece::EMPTY;
			return;
		}
		if (cMove == -1 &&
			get(rowFrom, colFrom - 1) == ChessPiece::BLACK_EN_PASSANTABLE_PAWN) {
			get(rowFrom + 1, colFrom - 1) = ChessPiece::WHITE_PAWN;
			get(rowFrom, colFrom - 1) = ChessPiece::EMPTY;
			return;
		}

		// promote
		if (rowFrom == 7 && rMove == 1) {
			get(rowFrom, colFrom) = ChessPiece::EMPTY;
			get(rowFrom + rMove, colFrom + cMove) = promotionType;
			return;
		}
		break;

	case ChessPiece::BLACK_PAWN:
		// en passant
		if (cMove == 1 &&
			get(rowFrom, colFrom + 1) == ChessPiece::WHITE_EN_PASSANTABLE_PAWN) {
			get(rowFrom - 1, colFrom + 1) = ChessPiece::BLACK_PAWN;
			get(rowFrom, colFrom + 1) = ChessPiece::EMPTY;
			return;
		}
		if (cMove == -1 &&
			get(rowFrom, colFrom - 1) == ChessPiece::WHITE_EN_PASSANTABLE_PAWN) {
			get(rowFrom - 1, colFrom - 1) = ChessPiece::BLACK_PAWN;
			get(rowFrom, colFrom - 1) = ChessPiece::EMPTY;
			return;
		}

		// promote
		if (rowFrom == 1 && rMove == -1) {
			get(rowFrom, colFrom) = ChessPiece::EMPTY;
			get(rowFrom + rMove, colFrom + cMove) = promotionType;
			return;
		}
		break;

	case ChessPiece::WHITE_KING:

		// kingside castle
		if (cMove == 2) {

			// king didn't move and king's rook didn't move
			if (!(wKingHasMoved || wkRookHasMoved)) {
				wKingHasMoved = true;
				wkRookHasMoved = true;
				get(0, 4) = ChessPiece::EMPTY;
				get(0, 5) = ChessPiece::WHITE_ROOK;
				get(0, 6) = ChessPiece::WHITE_KING;
				get(0, 7) = ChessPiece::EMPTY;
				return;
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
				get(0, 2) = ChessPiece::WHITE_KING;
				get(0, 3) = ChessPiece::WHITE_ROOK;
				get(0, 4) = ChessPiece::EMPTY;
				return;
			}
		}
		wKingHasMoved = true;
		break;

	case ChessPiece::BLACK_KING:

		// kingside castle
		if (cMove == 2) {

			// king didn't move and king's rook didn't move
			if (!(bKingHasMoved || bkRookHasMoved)) {
				bKingHasMoved = true;
				bkRookHasMoved = true;
				get(7, 4) = ChessPiece::EMPTY;
				get(7, 5) = ChessPiece::BLACK_ROOK;
				get(7, 6) = ChessPiece::BLACK_KING;
				get(7, 7) = ChessPiece::EMPTY;
				return;
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
				get(7, 2) = ChessPiece::BLACK_KING;
				get(7, 3) = ChessPiece::BLACK_ROOK;
				get(7, 4) = ChessPiece::EMPTY;
				return;
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