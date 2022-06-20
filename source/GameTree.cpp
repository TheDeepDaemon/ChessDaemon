#include"GameTree.h"
#include<vector>
#include<random>


using std::vector;


string getAlgebraic(const BoardPosition& pos) {
	char row = '1' + pos.row;
	char col = 'a' + pos.col;

	string str;
	str += col;
	str += row;
	return str;
}



inline void print(const Move& move) {
	cout << "From: " << getAlgebraic(move.from) << ", To: " << getAlgebraic(move.to) << "\n";
}



size_t getIndexOfHighest(const vector<int>& vec) {
	size_t highest = 0;
	for (size_t i = 1; i < vec.size(); i++) {
		if (vec[highest] < vec[i]) {
			highest = i;
		}
	}
	return highest;
}


size_t getIndexOfLowest(const vector<int>& vec) {
	size_t lowest = 0;
	for (size_t i = 1; i < vec.size(); i++) {
		if (vec[lowest] > vec[i]) {
			lowest = i;
		}
	}
	return lowest;
}







struct Node {

	static uint64_t totalNodes;

	vector<std::pair<Move, Node*>> nextPositions;

	ChessBoard board;

	int positionValue = 0;

	bool terminal = false;

	bool expanded = false;

	Node(const ChessBoard& b) : board(b), positionValue(0) {
		if (board.isCheckMated(board.whiteToMove)) {
			terminal = true;

			if (board.whiteToMove) {
				positionValue = -(WIN_VALUE + 64);
			}
			else {
				positionValue = (WIN_VALUE + 64);
			}

		}
		totalNodes++;
	}

	~Node() {
		for (std::pair<Move, Node*> node : nextPositions) {
			if (node.second != nullptr) {
				delete node.second;
			}
		}
		totalNodes--;
	}

	Node* discardExcept(const Move& move) {
		Node* next = nullptr;
		for (size_t i = 0; i < nextPositions.size(); i++) {
			std::pair<Move, Node*> node = nextPositions[i];
			if (node.second != nullptr && node.first != move) {
				delete node.second;
				nextPositions[i].second = nullptr;
			}
			else if (node.first == move) {
				next = node.second;
			}
		}
		nextPositions.clear();
		return next;
	}

	void expand() {
		if (!expanded) {
			std::list<Move> moves = board.getAllPossibleMoves(board.whiteToMove);

			nextPositions.reserve(moves.size());

			for (const Move& move : moves) {
				if (!board.wouldBeInCheck(board.whiteToMove, move)) {
					Node* node = new Node(board.getNextPosition(move));
					nextPositions.push_back(std::pair<Move, Node*>(move, node));
				}
			}

			if constexpr (SHUFFLE_MINIMAX_OPTIONS) {
				std::random_device rd;
				std::mt19937 rand_gen(rd());
				std::shuffle(nextPositions.begin(), nextPositions.end(), rand_gen);
			}

			expanded = true;
		}
	}

	void minimax(const int n, int alpha, int beta) {

		if (terminal) {
			return;
		}

		expand();

		if (n > 0) {
			if (nextPositions.size() > 0) {
				int best = 0;

				if (board.whiteToMove) {
					best = INT_MIN;
				}
				else {
					best = INT_MAX;
				}

				for (const std::pair<Move, Node*>& next: nextPositions) {
					next.second->minimax(n - 1, alpha, beta);
					int val = next.second->positionValue;

					if (board.whiteToMove) {
						best = std::max(best, val);
						alpha = std::max(best, alpha);
					}
					else {
						best = std::min(best, val);
						beta = std::min(best, beta);
					}

					if (beta <= alpha) {
						break;
					}

				}

				if (board.whiteToMove) {
					if (best > WIN_VALUE) {
						best--;
					}
				}
				else {
					if (best < -WIN_VALUE) {
						best++;
					}
				}

				positionValue = best;
			}
			else {
				positionValue = 0;
			}
		}
		else {
			positionValue = board.totalPoints();
		}
	}

	Move getBestMove(const int depth);

};

uint64_t Node::totalNodes = 0;


void runMinimax(Node* node, int depth) {
	node->minimax(depth, INT_MIN, INT_MAX);
}


Move Node::getBestMove(const int depth) {
	expand();
	_ASSERT(nextPositions.size() > 0);

	cout << "next positions: " << nextPositions.size() << "\n";

	if constexpr (USES_MULTITHREADING) {
		const int numThreads = 8;

		const size_t n = nextPositions.size() / numThreads;

		for (size_t i = 0; i < n; i++) {
			std::list<std::thread*> threads;

			for (int j = 0; j < numThreads; j++) {
				size_t index = (i * numThreads) + j;
				threads.push_back(new std::thread(
					runMinimax, nextPositions[index].second, depth));
			}

			cout << "Waiting on " << threads.size() << " threads\n";

			for (std::thread* t : threads) {
				t->join();
				delete t;
			}
		}

		const size_t remainder = nextPositions.size() % numThreads;

		if (remainder > 0) {
			std::list<std::thread*> threads;

			const size_t start = n * numThreads;
			for (size_t i = 0; i < remainder; i++) {
				threads.push_back(new std::thread(
					runMinimax, nextPositions[start + i].second, depth));
			}

			cout << "Waiting on " << threads.size() << " threads\n";

			for (std::thread* t : threads) {
				t->join();
				delete t;
			}
		}
		
	}
	else {
		for (size_t i = 0; i < nextPositions.size(); i++) {
			nextPositions[i].second->minimax(depth, INT_MIN, INT_MAX);
		}
	}

	vector<int> moveValues(nextPositions.size(), 0);

	for (size_t i = 0; i < nextPositions.size(); i++) {
		moveValues[i] = nextPositions[i].second->positionValue;
		if constexpr (false) {
			print(nextPositions[i].first);
			cout << "Value: " << moveValues[i] << "\n\n";
		}
	}

	size_t index = 0;
	if (board.whiteToMove) {
		index = getIndexOfHighest(moveValues);
	}
	else {
		index = getIndexOfLowest(moveValues);
	}

	return nextPositions[index].first;
}



GameTree::GameTree() : 
	root(new Node(ChessBoard())),
	chosenMove(BoardPosition(-1, -1), BoardPosition(-1, -1)) {}

GameTree::GameTree(const ChessBoard& board) : 
	root(new Node(board)),
	chosenMove(BoardPosition(-1, -1), BoardPosition(-1, -1)) {}

GameTree::~GameTree() {
	delete root;
}

void GameTree::findBestMove(const int depth) {
	chosenMove = root->getBestMove(depth);
}

bool GameTree::makeMove(const Move& move) {
	root->expand();
	Node* node = nullptr;

	for (const std::pair<Move, Node*>& next: root->nextPositions) {
		if (next.first == move) {
			node = next.second;
		}
	}

	if (node != nullptr) {
		root->discardExcept(move);
		delete root;
		root = node;
	}

	return node != nullptr;
}

uint64_t GameTree::getTotalNodes() {
	return Node::totalNodes;
}
