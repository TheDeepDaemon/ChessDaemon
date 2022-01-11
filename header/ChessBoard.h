#ifndef BOARD_H
#define BOARD_H
#include"util.h"
#include"headers.h"






// map enum to character for displaying to console
char pieceMap(ChessPiece p);


bool pieceIsWhite(ChessPiece p);

bool pieceIsBlack(ChessPiece p);


/**
* Board class:
*	contains the pieces
*/
class ChessBoard {
private:
	
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

	bool wKingHasMoved = false;
	bool wkRookHasMoved = false;
	bool wqRookHasMoved = false;
	bool bKingHasMoved = false;
	bool bkRookHasMoved = false;
	bool bqRookHasMoved = false;


	// FUNCTIONS FOR CHECKING LEGAL MOVES

	template<bool white>
	bool isPawnAttacking(int rowFrom, int colFrom);

	
	template<bool white>
	bool isLegalPawn(int rowFrom, int colFrom,
		int rMove, int cMove, bool landingOnOpposite);

	// looks horizontally or vertically and 
	// returns the first piece in that direction
	// used to detect if the king is in check
	// not used for regular rook moves because you might intercept the wrong piece
	template<bool vertical, int dir_>
	ChessPiece firstPieceInRookDir(int rowFrom, int colFrom);

	// check if it is a legal rook move
	bool isLegalRook(int rowFrom, int colFrom, int rMove, int cMove);

	// return whether a knight is attacking this square
	template<bool white>
	bool knightIsAttacking(int rowFrom, int colFrom);

	// check if this move is a legal knight move
	bool isLegalKnight(int rMove, int cMove);


	// looks diagonally and returns the first piece in that direction
	// used to detect if the king is in check
	// not used for regular bishop moves because you might intercept the wrong piece
	template<int rDir_, int cDir_>
	ChessPiece firstPieceInBishopDir(int rowFrom, int colFrom);

	// is this a legal bishop move
	bool isLegalBishop(int rowFrom, int colFrom, int rMove, int cMove);

	bool isLegalQueen(int rowFrom, int colFrom, int rMove, int cMove);

	template<bool white>
	bool isLegalKing(int rowFrom, int colFrom, int rMove, int cMove);



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


public:


	// can x be a real, valid board coordinate?
	bool correctRange(unsigned x);

	// access the piece type of a location on the board
	// by row and column
	__forceinline ChessPiece& get(unsigned row, unsigned col);


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
	bool isLegalMoveNoCheck(bool whiteToMove,
		int rowFrom, int colFrom, int rMove, int cMove);

	// return if piece is one of two pieces
	bool isOneOfThese(ChessPiece pToCheck, ChessPiece p1, ChessPiece p2);

	template<bool white>
	bool isKingInCheck(int kingRow, int kingCol);

	
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
		int rowFrom, int colFrom, int rMove, int cMove, ChessPiece promotionType = ChessPiece::EMPTY);


	// makeMoveWithCheck

	// makeMoveWithCheckReturnTaken


};




#endif // !BOARD_H