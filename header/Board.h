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
class Board {
private:
	
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


	// internally access the val at board location
	__forceinline Piece& get(unsigned row, unsigned col) {
		return *(board + (row * BOARD_SIDE_LENGTH) + col);
	}


	//FUNCTIONS FOR CHECKING LEGAL MOVES
	template<bool white>
	bool isLegalPawn(int rMove, int cMove) {
		// white pawns can only move up one row
		// black pawns can only move down one row
		const int dir = white ? 1 : -1;
		if (rMove == dir) {
			// either left one, right one or no column move
			return (cMove == 0 || 
				((cMove == 1 || cMove == -1) && ()));
		}
		// first pawn move means you can move two squares
		else if (rMove == (dir * 2)) {
			return cMove == 0;
		}
		return false;
	}

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

	bool isLegalKnight(int rMove, int cMove) {

		// knights move in both directions no matter what
		if (rMove == 0 || cMove == 0) {
			return false;
		}
		
		// 3 = 1 + 2
		// or 3 = 3 + 0
		// and 0 is ruled out
		// so it must be
		// 2 rows and 1 col or 1 row and 2 cols
		return (abs(rMove) + abs(cMove) == 3);
	}

	bool isLegalBishop(int rowFrom, int colFrom, int rMove, int cMove) {
		// find if the number of moves
		// horizontally is the same 
		// as the number of moves vertically
		return abs(rMove) == abs(cMove);
	}

	bool isLegalQueen(int rowFrom, int colFrom, int rMove, int cMove) {
		// the queen moves like the bishop and the rook
		return isLegalBishop(rowFrom, colFrom, rMove, cMove) || isLegalRook(rowFrom, colFrom, rMove, cMove);
	}

	bool isLegalKing(int rMove, int cMove) {
		// only one space
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



public:

	bool correctRange(unsigned x) {
		return (x < BOARD_SIDE_LENGTH);
	}

	
	inline Piece& operator()(unsigned row, unsigned col) {
		return get(row, col);
	}


	
	Board() {
		memset(board, (int)Piece::EMPTY, BOARD_SIZE);
		// set the board in starting position
		startingPosition();
	}

	


	/**
	* Check if a move is legal
	* whiteToMove: whether it is white's turn. 1 = white, 0 = black
	* rowFrom: the current row of the piece
	* colFrom: the current column of the piece
	* rMove: how many rows to move
	* cMove: how many columns to move
	*/
	bool isLegalMove(bool whiteToMove,
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
				return isLegalPawn<true>(rMove, cMove);
			case Piece::WHITE_EN_PASSANTABLE_PAWN:
				return isLegalPawn<true>(rMove, cMove);
			case Piece::WHITE_ROOK:
				return isLegalRook(rowFrom, colFrom, rMove, cMove);
			case Piece::WHITE_KNIGHT:
				return isLegalKnight(rMove, cMove);
			case Piece::WHITE_BISHOP:
				return isLegalBishop(rowFrom, colFrom, rMove, cMove);
			case Piece::WHITE_QUEEN:
				return isLegalQueen(rowFrom, colFrom, rMove, cMove);
			case Piece::WHITE_KING:
				return isLegalKing(rMove, cMove);
			case Piece::BLACK_PAWN:
				return isLegalPawn<true>(rMove, cMove);
			case Piece::BLACK_EN_PASSANTABLE_PAWN:
				return isLegalPawn<true>(rMove, cMove);
			case Piece::BLACK_ROOK:
				return isLegalRook(rowFrom, colFrom, rMove, cMove);
			case Piece::BLACK_KNIGHT:
				return isLegalKnight(rMove, cMove);
			case Piece::BLACK_BISHOP:
				return isLegalBishop(rowFrom, colFrom, rMove, cMove);
			case Piece::BLACK_QUEEN:
				return isLegalQueen(rowFrom, colFrom, rMove, cMove);
			case Piece::BLACK_KING:
				return isLegalKing(rMove, cMove);
			default:
				return false;
			}
			
		}
	}

	/**
	* Make a move on this board
	* whiteToMove: whether it is white's turn. 1 = white, 0 = black
	* rowFrom: the current row of the piece
	* colFrom: the current column of the piece
	* rMove: how many rows to move
	* cMove: how many columns to move
	*/
	void makeMove(bool whiteToMove, 
		int rowFrom, int colFrom, int rMove, int cMove, Piece promotionType = Piece::EMPTY) {
		// check special rules like two pawn move and en passant
		// check promotion

		// move piece
	}

	// makeMoveWithCheck

	// makeMoveWithCheckReturnTaken


};


#endif // !BOARD_H