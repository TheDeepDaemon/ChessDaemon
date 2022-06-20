#ifndef GAME_TREE_H
#define GAME_TREE_H
#include"ChessBoard.h"
#include<cstdint>
#include<thread>


struct Node;


struct GameTree {

	Node* root = nullptr;
	Move chosenMove;

	GameTree();

	GameTree(const ChessBoard& board);

	~GameTree();

	void findBestMove(const int depth);

	bool makeMove(const Move& move);

	uint64_t getTotalNodes();

};


#endif // !GAME_TREE_H