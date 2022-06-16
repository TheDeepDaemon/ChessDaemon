#ifndef BOARD_POSITION_H
#define BOARD_POSITION_H


struct BoardPosition {
	int row = 0;
	int col = 0;
	BoardPosition(const int row, const int col);
	bool operator==(const BoardPosition& pos);
	bool operator!=(const BoardPosition& pos);
	bool is(const int row, const int col);
	static BoardPosition null;
};






#endif // !BOARD_POSITION_H

