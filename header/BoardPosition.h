#ifndef BOARD_POSITION_H
#define BOARD_POSITION_H
#include<ostream>


struct BoardPosition {
	int row = 0;
	int col = 0;

	BoardPosition(int row_, int col_) : row(row_), col(col_) {}

	BoardPosition(const BoardPosition& boardPosition) :
		row(boardPosition.row), col(boardPosition.col) {}

	bool isAt(int row_, int col_) const {
		return (row == row_) && (col == col_);
	}

	void operator=(const BoardPosition& boardPosition) {
		row = boardPosition.row;
		col = boardPosition.col;
	}

};

inline BoardPosition operator+(const BoardPosition& p1, const BoardPosition& p2) {
	return BoardPosition(p1.row + p2.row, p1.col + p2.col);
}

inline BoardPosition operator-(const BoardPosition& p1, const BoardPosition& p2) {
	return BoardPosition(p1.row - p2.row, p1.col - p2.col);
}

inline bool operator==(const BoardPosition& p1, const BoardPosition& p2) {
	return (p1.row == p2.row) && (p1.col == p2.col);
}

inline bool operator!=(const BoardPosition& p1, const BoardPosition& p2) {
	return (p1.row != p2.row) || (p1.col != p2.col);
}

inline std::ostream& operator<<(std::ostream& os, const BoardPosition& piece) {
	os << piece.row << ", " << piece.col;
	return os;
}

inline BoardPosition readAlgebraicNotation(char colChar, char rowChar) {
	colChar = static_cast<char>(std::tolower(static_cast<unsigned char>(colChar)));
	BoardPosition boardCoord((rowChar - '0') - 1, colChar - 'a');
	return boardCoord;
}


#endif // !BOARD_POSITION_H