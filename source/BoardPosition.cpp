#include"BoardPosition.h"


BoardPosition::BoardPosition(const int row_, const int col_) :
	row(row_), col(col_) {
}

bool BoardPosition::operator==(const BoardPosition& pos) {
	return (this->row == pos.row) && (this->col == pos.col);
}

bool BoardPosition::operator!=(const BoardPosition& pos) {
	return (this->row != pos.row) || (this->col != pos.col);
}


bool BoardPosition::is(const int row_, const int col_) {
	return (this->row == row_) && (this->col == col_);
}


BoardPosition BoardPosition::null = BoardPosition(-1, -1);
