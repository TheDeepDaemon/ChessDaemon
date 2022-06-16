#ifndef BOARD_H
#define BOARD_H
#include"util.h"
#include"headers.h"
#include"BoardPosition.h"





// map enum to character for displaying to console
char pieceMap(const ChessPiece p);

std::ostream& operator<<(std::ostream& os, const ChessPiece p);


bool pieceIsWhite(ChessPiece p);

bool pieceIsBlack(ChessPiece p);


/**
* Board class:
*	contains the pieces
*/
class ChessBoard {
private:

	// is it white's move?
	bool whiteToMove;
	
	// array with all board locations in it
	// contiguous for efficiency
	// row major format
	ChessPiece board[64];

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

	BoardPosition whiteKingPos = BoardPosition::null;
	BoardPosition blackKingPos = BoardPosition::null;
	BoardPosition movedWhitePawn = BoardPosition::null;
	BoardPosition movedBlackPawn = BoardPosition::null;

	bool wKingHasMoved = false;
	bool wkRookHasMoved = false;
	bool wqRookHasMoved = false;
	bool bKingHasMoved = false;
	bool bkRookHasMoved = false;
	bool bqRookHasMoved = false;


	// FUNCTIONS FOR CHECKING LEGAL MOVES

	template<bool white>
	bool isPawnAttacking(const int rowFrom, const int colFrom);

	
	template<bool white>
	bool isLegalPawn(const int rowFrom, const int colFrom,
		const int rMove, const int cMove, const bool landingOnOpposite);

	// looks horizontally or vertically and
	// returns the first piece in that direction
	// used to detect if the king is in check
	// not used for regular rook moves because you might intercept the wrong piece
	template<bool vertical, int dir_>
	ChessPiece firstPieceInRookDir(const int rowFrom, const int colFrom);

	// check if it is a legal rook move
	bool isLegalRook(const int rowFrom, const int colFrom, const int rMove, const int cMove);

	// return whether a knight is attacking this square
	template<bool white>
	bool knightIsAttacking(const int rowFrom, const int colFrom);

	// check if this move is a legal knight move
	bool isLegalKnight(const int rMove, const int cMove);


	// looks diagonally and returns the first piece in that direction
	// used to detect if the king is in check
	// not used for regular bishop moves because you might intercept the wrong piece
	template<int rDir_, int cDir_>
	ChessPiece firstPieceInBishopDir(const int rowFrom, const int colFrom);

	// is this a legal bishop move
	bool isLegalBishop(const int rowFrom, const int colFrom, const int rMove, const int cMove);

	bool isLegalQueen(const int rowFrom, const int colFrom, const int rMove, const int cMove);

	template<bool white>
	bool isLegalKing(const int rowFrom, const int colFrom, const int rMove, const int cMove);



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
	void startingPosition();

	void makeLegalMoveInternal(const int rowFrom, const int colFrom, const int rowTo, const int colTo, const ChessPiece promotionType);


	template<bool white>
	void getPawnMoves(const BoardPosition pos, vector<BoardPosition>& moves);

	template<bool white>
	void getRookMoves(const BoardPosition pos, vector<BoardPosition>& moves);

	template<bool white>
	void getKnightMoves(const BoardPosition pos, vector<BoardPosition>& moves);

	template<bool white>
	void getBishopMoves(const BoardPosition pos, vector<BoardPosition>& moves);

	template<bool white>
	void getQueenMoves(const BoardPosition pos, vector<BoardPosition>& moves);

	template<bool white>
	void getKingMoves(const BoardPosition pos, vector<BoardPosition>& moves);


	// can x be a real, valid board coordinate?
	bool correctRange(const unsigned x) {
		return (x < BOARD_SIDE_LENGTH);
	}

	bool correctRange(const BoardPosition pos) {
		return correctRange(pos.row) && correctRange(pos.col);
	}


public:

	
	// access the piece type of a location on the board
	// by row and column
	inline ChessPiece& get(const unsigned row, const unsigned col) {
		return board[((uint64_t)row * BOARD_SIDE_LENGTH) + (uint64_t)col];
	}

	inline ChessPiece& get(const BoardPosition pos) {
		return board[((uint64_t)pos.row * BOARD_SIDE_LENGTH) + (uint64_t)pos.col];
	}

	inline bool isOnBoard(const unsigned row, const unsigned col) {
		return correctRange(row) && correctRange(col);
	}

	// check if in the correct range, then return if the piece is of the given value
	inline bool isPiece(const unsigned row, const unsigned col, const ChessPiece p) {
		return isOnBoard(row, col) && (get(row, col) == p);
	}

	inline bool isPiece(const BoardPosition pos, const ChessPiece p) {
		return correctRange(pos) && (get(pos) == p);
	}

	// return if the square is empty, doesn't have a piece on it
	inline bool isEmpty(const unsigned row, const unsigned col) {
		return isPiece(row, col, ChessPiece::EMPTY);
	}

	inline bool isEmpty(const BoardPosition pos) {
		return isPiece(pos, ChessPiece::EMPTY);
	}


	// construct the board, set it in it's
	// starting position
	ChessBoard();

	
	void displayToConsole();



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
	bool isLegalMoveNoCheck(
		const int rowFrom, const int colFrom, const int rowTo, const int colTo);

	// return if piece is one of two pieces
	bool isOneOfThese(const ChessPiece pToCheck, const ChessPiece p1, const ChessPiece p2);

	template<bool white>
	bool isKingInCheck(const int kingRow, const int kingCol);

	template<bool white>
	bool isKingInCheck();

	
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
	void makeLegalMove(
		const int rowFrom, const int colFrom, const int rowTo, const int colTo, const ChessPiece promotionType = ChessPiece::EMPTY);


	void getMoves(const BoardPosition pos, vector<BoardPosition>& moves);


};


#endif // !BOARD_H