#ifndef CONSTANTS_H
#define CONSTANTS_H
#include"headers.h"


// global constants for chess
namespace Constants
{
	const unsigned int SCREEN_WIDTH = 1600;
	const unsigned int SCREEN_HEIGHT = 1200;

	// the length of the square board in cells
    const unsigned int BOARD_SIDE_LENGTH = 8;

	// the number of squares total in the board
	const unsigned int BOARD_SIZE = BOARD_SIDE_LENGTH * BOARD_SIDE_LENGTH;

	// what fraction of the board sprite is the edge
    const double BOARD_EDGE = 78.0 / 700.0;
}


// all possible pieces
enum class ChessPiece {
	EMPTY = 0,
	WHITE_PAWN,
	WHITE_ROOK,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_QUEEN,
	WHITE_KING,
	BLACK_PAWN,
	BLACK_ROOK,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_QUEEN,
	BLACK_KING,
	NUM_PIECE_TYPES
};


#endif