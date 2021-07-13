#ifndef BOARD_H
#define BOARD_H
#include<cstdint>
#include<cmath>

// global constants for chess
// global usually isn't good practice, 
// but chess has had the same board dimensions for over 1000 years
// and I don't usually name variables like this, so I think I'm fine
const unsigned int BOARD_SIZE = 64;
const unsigned int BOARD_SIDE_LENGTH = 8;


// return the sign of integer x
int sign(int x) {
	if (x == 0) {
		return 0;
	}
	else {
		return (x > 0) ? 1 : -1;
	}
}


enum class Piece {
	EMPTY = 0,
	WHITE_PAWN,
	WHITE_EN_PASSANTABLE_PAWN,
	WHITE_ROOK,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_QUEEN,
	WHITE_KING,
	BLACK_PAWN,
	BLACK_EN_PASSANTABLE_PAWN,
	BLACK_ROOK,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_QUEEN,
	BLACK_KING,
	NUM_PIECE_TYPES
};


bool pieceIsWhite(Piece p) {
	return (Piece::WHITE_PAWN <= p) && (p <= Piece::WHITE_KING);
}

bool pieceIsBlack(Piece p) {
	return (Piece::BLACK_PAWN <= p) && (p <= Piece::BLACK_KING);
}


/**
* Board class:
*	contains the pieces
*/
struct Board {
	
	// array with all board locations in it
	// contiguous for efficiency
	// row major format
	Piece board[64];

	//_____________________
	// row       black
	// [7]  r n b q k b n r
	// [6]  p p p p p p p p
	// [5]  0 0 0 0 0 0 0 0
	// [4]  0 0 0 0 0 0 0 0
	// [3]  0 0 0 0 0 0 0 0
	// [2]  0 0 0 0 0 0 0 0
	// [1]  p p p p p p p p
	// [0]  r n b q k b n r
	//           white
	// col |0|1|2|3|4|5|6|7|
	//_____________________

	bool wKingHasMoved = false;
	bool wkRookHasMoved = false;
	bool wqRookHasMoved = false;
	bool bKingHasMoved = false;
	bool bkRookHasMoved = false;
	bool bqRookHasMoved = false;



	// internally access the val at board location
	__forceinline Piece& get(unsigned row, unsigned col) {
		return *(board + (row * BOARD_SIDE_LENGTH) + col);
	}



	// FUNCTIONS FOR CHECKING LEGAL MOVES

	template<bool white>
	bool isPawnAttacking(int rowFrom, int colFrom) {
		const int rowDir = white ? 1 : -1;
		const Piece p = white ? Piece::BLACK_PAWN : Piece::WHITE_PAWN;

		return (get(rowDir, 1) == p || get(rowDir, -1) == p);
	}

	
	template<bool white>
	bool isLegalPawn(int rowFrom, int colFrom, 
		int rMove, int cMove, bool landingOnOpposite) {
		// white pawns can only move up one row
		// black pawns can only move down one row
		const int dir = white ? 1 : -1;

		if (rMove == dir) {
			// stright unless taking a piece
			if (cMove == 0) {
				return true;
			}
			else {
				if (landingOnOpposite) {
					if (cMove == 1 || cMove == -1) {
						return true;
					}
				}
				else {

					const Piece enPas = white ? 
						Piece::BLACK_EN_PASSANTABLE_PAWN : Piece::WHITE_EN_PASSANTABLE_PAWN;

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
	Piece firstPieceInRookDir(int rowFrom, int colFrom) {
		// make sure you are only incrementing one at a time
		const int dir = int(0 < dir_) - int(dir_ < 0);

		// set the proper range
		const int boundary = dir == 1 ? 8 : -1;

		// up and down rook movements
		if (vertical) {
			for (int i = rowFrom + dir; i != boundary; i += dir) {
				Piece p = get(i, colFrom);
				if (p != Piece::EMPTY) {
					return p;
				}
			}
		}
		// left and right rook movements
		else {
			for (int i = colFrom + dir; i != boundary; i += dir) {
				Piece p = get(rowFrom, i);
				if (p != Piece::EMPTY) {
					return p;
				}
			}
		}
		return Piece::EMPTY;
	}

	// check if it is a legal rook move
	bool isLegalRook(int rowFrom, int colFrom, int rMove, int cMove) {

		// rooks only move col wise or row wise, not both
		if (rMove != 0) {
			if (cMove == 0) {
				int n = rowFrom + rMove; // inefficient, change later
				for (int r = rowFrom; r < n; r++) {
					if (get(r, colFrom) != Piece::EMPTY) {
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
					if (get(rowFrom, c) != Piece::EMPTY) {
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
	bool knightIsAttacking(int rowFrom, int colFrom) {
		Piece knight = white ? Piece::BLACK_KNIGHT : Piece::WHITE_KNIGHT;

		// look for L shapes
		return 
			(get(rowFrom+1, colFrom+2) == knight) ||
			(get(rowFrom+2, colFrom+1) == knight) || 
			(get(rowFrom+2, colFrom-1) == knight) || 
			(get(rowFrom+1, colFrom-2) == knight) || 
			(get(rowFrom-1, colFrom-2) == knight) || 
			(get(rowFrom-2, colFrom-1) == knight) || 
			(get(rowFrom-2, colFrom+1) == knight) || 
			(get(rowFrom-1, colFrom+2) == knight);
	}

	// check if this move is a legal knight move
	bool isLegalKnight(int rMove, int cMove) {

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
	Piece firstPieceInBishopDir(int rowFrom, int colFrom) {
		// make sure you are only incrementing one at a time
		const int rDir = int(0 < rDir_) - int(rDir_ < 0);
		const int cDir = int(0 < cDir_) - int(cDir_ < 0);

		// set the proper range
		const int rBoundary = rDir == 1 ? 8 : -1;
		const int cBoundary = cDir == 1 ? 8 : -1;

		for (int r = rowFrom + rDir, c = colFrom + cDir; r != rBoundary && c != cBoundary; r+=rDir, c+=cDir) {
			Piece p = get(r, c);
			if (p != Piece::EMPTY) {
				return p;
			}
		}

		return Piece::EMPTY;
	}

	// is this a legal bishop move
	bool isLegalBishop(int rowFrom, int colFrom, int rMove, int cMove) {

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
				if (get(rowFrom + (i * r), colFrom + (i * c)) != Piece::EMPTY) {
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

	bool isLegalQueen(int rowFrom, int colFrom, int rMove, int cMove) {

		// the queen moves like the bishop and the rook combined
		return isLegalBishop(rowFrom, colFrom, rMove, cMove) || isLegalRook(rowFrom, colFrom, rMove, cMove);
	}

	template<bool white>
	bool isLegalKing(int rowFrom, int colFrom, int rMove, int cMove) {

		const bool kingMoved = white ? wKingHasMoved : bKingHasMoved;

		// castling
		if (!kingMoved) {
			const bool kingPiece = white ? Piece::WHITE_KING : Piece::BLACK_KING;
			const bool rookPiece = white ? Piece::WHITE_ROOK : Piece::BLACK_ROOK;
			const int rowToCheck = white ? 0 : 7;

			// kingside castle
			if (cMove == 2) {
				const bool rookHasMoved = white ? wkRookHasMoved : bkRookHasMoved;

				return !rookHasMoved &&
					(get(rowToCheck, 3) == kingPiece && get(rowToCheck, 7) == rookPiece)
			}
			// queenside castle
			else if (cMove == -2) {
				const bool rookHasMoved = white ? wqRookHasMoved : bqRookHasMoved;

				return !rookHasMoved &&
					(get(rowToCheck, 3) == kingPiece && get(rowToCheck, 0) == rookPiece);
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
	void startingPosition() {

		// set the pawns
		get(1, 0) = Piece::WHITE_PAWN;
		get(1, 1) = Piece::WHITE_PAWN;
		get(1, 2) = Piece::WHITE_PAWN;
		get(1, 3) = Piece::WHITE_PAWN;
		get(1, 4) = Piece::WHITE_PAWN;
		get(1, 5) = Piece::WHITE_PAWN;
		get(1, 6) = Piece::WHITE_PAWN;
		get(1, 7) = Piece::WHITE_PAWN;

		get(6, 0) = Piece::BLACK_PAWN;
		get(6, 1) = Piece::BLACK_PAWN;
		get(6, 2) = Piece::BLACK_PAWN;
		get(6, 3) = Piece::BLACK_PAWN;
		get(6, 4) = Piece::BLACK_PAWN;
		get(6, 5) = Piece::BLACK_PAWN;
		get(6, 6) = Piece::BLACK_PAWN;
		get(6, 7) = Piece::BLACK_PAWN;


		// set the pieces
		get(0, 0) = Piece::WHITE_ROOK;
		get(0, 1) = Piece::WHITE_KNIGHT;
		get(0, 2) = Piece::WHITE_BISHOP;
		get(0, 3) = Piece::WHITE_QUEEN;
		get(0, 4) = Piece::WHITE_KING;
		get(0, 5) = Piece::WHITE_BISHOP;
		get(0, 6) = Piece::WHITE_KNIGHT;
		get(0, 7) = Piece::WHITE_ROOK;

		get(7, 0) = Piece::BLACK_ROOK;
		get(7, 1) = Piece::BLACK_KNIGHT;
		get(7, 2) = Piece::BLACK_BISHOP;
		get(7, 3) = Piece::BLACK_QUEEN;
		get(7, 4) = Piece::BLACK_KING;
		get(7, 5) = Piece::BLACK_BISHOP;
		get(7, 6) = Piece::BLACK_KNIGHT;
		get(7, 7) = Piece::BLACK_ROOK;

	}


	// public


	// can x be a real, valid board coordinate?
	bool correctRange(unsigned x) {
		return (x < BOARD_SIDE_LENGTH);
	}

	// access the piece type of a location on the board
	// by row and column
	inline Piece& operator()(unsigned row, unsigned col) {
		return get(row, col);
	}


	// construct the board, set it in it's
	// starting position
	Board() {
		memset(board, (int)Piece::EMPTY, BOARD_SIZE);
		// set the board in starting position
		startingPosition();
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
	bool isLegalMoveNoCheck(bool whiteToMove,
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

			Piece pFrom = get(rowFrom, colFrom);

			// make sure you are not 
			// moving from an empty square
			if (Piece::EMPTY == pFrom) {
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
			case Piece::WHITE_PAWN:
				return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, fromWhite != toWhite);
			case Piece::WHITE_EN_PASSANTABLE_PAWN:
				return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, fromWhite != toWhite);
			case Piece::WHITE_ROOK:
				return isLegalRook(rowFrom, colFrom, rMove, cMove);
			case Piece::WHITE_KNIGHT:
				return isLegalKnight(rMove, cMove);
			case Piece::WHITE_BISHOP:
				return isLegalBishop(rowFrom, colFrom, rMove, cMove);
			case Piece::WHITE_QUEEN:
				return isLegalQueen(rowFrom, colFrom, rMove, cMove);
			case Piece::WHITE_KING:
				return isLegalKing<true>(rowFrom, colFrom, rMove, cMove);
			case Piece::BLACK_PAWN:
				return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, fromWhite != toWhite);
			case Piece::BLACK_EN_PASSANTABLE_PAWN:
				return isLegalPawn<true>(rowFrom, colFrom, rMove, cMove, fromWhite != toWhite);
			case Piece::BLACK_ROOK:
				return isLegalRook(rowFrom, colFrom, rMove, cMove);
			case Piece::BLACK_KNIGHT:
				return isLegalKnight(rMove, cMove);
			case Piece::BLACK_BISHOP:
				return isLegalBishop(rowFrom, colFrom, rMove, cMove);
			case Piece::BLACK_QUEEN:
				return isLegalQueen(rowFrom, colFrom, rMove, cMove);
			case Piece::BLACK_KING:
				return isLegalKing<false>(rowFrom, colFrom, rMove, cMove);
			default:
				return false;
			}
			
		}
	}

	// return if piece is one of two pieces
	bool isOneOfThese(Piece pToCheck, Piece p1, Piece p2) {
		return (pToCheck == p1 || pToCheck == p2);
	}

	template<bool white>
	bool isKingInCheck(int kingRow, int kingCol) {

		// look for bishop movement
		const Piece bishop = white ? Piece::BLACK_BISHOP : Piece::WHITE_BISHOP;
		const Piece queen = white ? Piece::BLACK_QUEEN : Piece::WHITE_QUEEN;

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
		const Piece rook = white ? Piece::BLACK_ROOK : Piece::WHITE_ROOK;

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
	void makeLegalMove(bool whiteToMove, 
		int rowFrom, int colFrom, int rMove, int cMove, Piece promotionType = Piece::EMPTY) {
		Piece pieceType = get(rowFrom, colFrom);

		// switch for handling special rules
		switch (pieceType) {
		case Piece::WHITE_PAWN:
			// en passant
			if (cMove == 1 &&
				get(rowFrom, colFrom + 1) == Piece::BLACK_EN_PASSANTABLE_PAWN) {
				get(rowFrom + 1, colFrom + 1) = Piece::WHITE_PAWN;
				get(rowFrom, colFrom + 1) = Piece::EMPTY;
				return;
			}
			if (cMove == -1 &&
				get(rowFrom, colFrom - 1) == Piece::BLACK_EN_PASSANTABLE_PAWN) {
				get(rowFrom + 1, colFrom - 1) = Piece::WHITE_PAWN;
				get(rowFrom, colFrom - 1) = Piece::EMPTY;
				return;
			}

			// promote
			if (rowFrom == 7 && rMove == 1) {
				get(rowFrom, colFrom) = Piece::EMPTY;
				get(rowFrom + rMove, colFrom + cMove) = promotionType;
				return;
			}
			break;

		case Piece::BLACK_PAWN:
			// en passant
			if (cMove == 1 &&
				get(rowFrom, colFrom + 1) == Piece::WHITE_EN_PASSANTABLE_PAWN) {
				get(rowFrom - 1, colFrom + 1) = Piece::BLACK_PAWN;
				get(rowFrom, colFrom + 1) = Piece::EMPTY;
				return;
			}
			if (cMove == -1 &&
				get(rowFrom, colFrom - 1) == Piece::WHITE_EN_PASSANTABLE_PAWN) {
				get(rowFrom - 1, colFrom - 1) = Piece::BLACK_PAWN;
				get(rowFrom, colFrom - 1) = Piece::EMPTY;
				return;
			}

			// promote
			if (rowFrom == 1 && rMove == -1) {
				get(rowFrom, colFrom) = Piece::EMPTY;
				get(rowFrom + rMove, colFrom + cMove) = promotionType;
				return;
			}
			break;

		case Piece::WHITE_KING:

			// kingside castle
			if (cMove == 2) {

				// king didn't move and king's rook didn't move
				if (!(wKingHasMoved || wkRookHasMoved)) {
					wKingHasMoved = true;
					wkRookHasMoved = true;
					get(0, 4) = Piece::EMPTY;
					get(0, 5) = Piece::WHITE_ROOK;
					get(0, 6) = Piece::WHITE_KING;
					get(0, 7) = Piece::EMPTY;
					return;
				}
			}
			// queenside castle
			else if (cMove == -2) {

				// king didn't move and queen's rook didn't move
				if (!(wKingHasMoved || wqRookHasMoved)) {
					wKingHasMoved = true;
					wqRookHasMoved = true;
					get(0, 0) = Piece::EMPTY;
					get(0, 1) = Piece::EMPTY;
					get(0, 2) = Piece::WHITE_KING;
					get(0, 3) = Piece::WHITE_ROOK;
					get(0, 4) = Piece::EMPTY;
					return;
				}
			}
			wKingHasMoved = true;
			break;

		case Piece::BLACK_KING:

			// kingside castle
			if (cMove == 2) {

				// king didn't move and king's rook didn't move
				if (!(bKingHasMoved || bkRookHasMoved)) {
					bKingHasMoved = true;
					bkRookHasMoved = true;
					get(7, 4) = Piece::EMPTY;
					get(7, 5) = Piece::BLACK_ROOK;
					get(7, 6) = Piece::BLACK_KING;
					get(7, 7) = Piece::EMPTY;
					return;
				}
			}
			// queenside castle
			else if (cMove == -2) {

				// king didn't move and queen's rook didn't move
				if (!(bKingHasMoved || bqRookHasMoved)) {
					bKingHasMoved = true;
					bqRookHasMoved = true;
					get(7, 0) = Piece::EMPTY;
					get(7, 1) = Piece::EMPTY;
					get(7, 2) = Piece::BLACK_KING;
					get(7, 3) = Piece::BLACK_ROOK;
					get(7, 4) = Piece::EMPTY;
					return;
				}
			}
			bKingHasMoved = true;
			break;

		case Piece::WHITE_ROOK:
			if (rowFrom == 0) {
				if (colFrom == 7) {
					wkRookHasMoved = true;
				}
				else if (colFrom = 0) {
					wqRookHasMoved = true;
				}
			}
			break;

		case Piece::BLACK_ROOK:
			if (rowFrom == 7) {
				if (colFrom == 7) {
					bkRookHasMoved = true;
				}
				else if (colFrom = 0) {
					bqRookHasMoved = true;
				}
			}
			break;

		}

		get(rowFrom + rMove, colFrom + cMove) = get(rowFrom, colFrom);
		get(rowFrom, colFrom) = Piece::EMPTY;
	}

	// makeMoveWithCheck

	// makeMoveWithCheckReturnTaken


};


#endif // !BOARD_H