#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H
#include"BoardPosition.h"
#include<string>


enum ChessPieceType {
	EMPTY = 0,
	PAWN,
	ROOK,
	BISHOP,
	KNIGHT,
	QUEEN,
	KING,
	NUM_PIECE_TYPES
};




struct ChessPiece {
	ChessPieceType type = ChessPieceType::EMPTY;
	bool white = false;

	ChessPiece(ChessPieceType type_, bool white_) :
		type(type_), white(white_) {}

	ChessPiece() :
		type(EMPTY), white(false) {}

	void operator=(const ChessPiece& pFrom) {
		type = pFrom.type;
		white = pFrom.white;
	}

	bool is(const ChessPieceType& pieceType) const {
		return type == pieceType;
	}

	bool is(const ChessPiece& piece) const {
		return (type == piece.type) && (white == piece.white);
	}

	bool isNot(const ChessPieceType& pieceType) const {
		return type != pieceType;
	}

};


struct ChessPiecePosition : public ChessPiece {
	int row = -1;
	int col = -1;

	ChessPiecePosition(
		const ChessPieceType type_, const bool white_,
		const int row_, const int col_) :
		ChessPiece(type_, white_),
		row(row_), col(col_) {}

	ChessPiecePosition(
		const ChessPieceType type_, const bool white_,
		const BoardPosition& boardPosition) :
		ChessPiece(type_, white_),
		row(boardPosition.row), col(boardPosition.col) {}

	ChessPiecePosition() :
		ChessPiece(), row(-1), col(-1) {}

	bool isAt(const BoardPosition& pos) const {
		return (row == pos.row) && (col == pos.col);
	}

	void moveTo(const BoardPosition& pos) {
		row = pos.row;
		col = pos.col;
	}

	BoardPosition getBoardPosition() {
		return BoardPosition(row, col);
	}

};



inline std::string getPieceName(const ChessPiece& piece) {
	if (piece.white) {
		switch (piece.type) {
		case PAWN:
			return "white pawn";
		case ROOK:
			return "white rook";
		case BISHOP:
			return "white bishop";
		case KNIGHT:
			return "white knight";
		case QUEEN:
			return "white queen";
		case KING:
			return "white king";
		default:
			return "";
		}
	}
	else {
		switch (piece.type) {
		case PAWN:
			return "black pawn";
		case ROOK:
			return "black rook";
		case BISHOP:
			return "black bishop";
		case KNIGHT:
			return "black knight";
		case QUEEN:
			return "black queen";
		case KING:
			return "black king";
		default:
			return "";
		}
	}
}



#endif // !CHESS_PIECE_H