#ifndef SYSTEM_H
#define SYSTEM_H
#include"DisplaySystem.h"
#include"ChessBoard.h"
#include"InputSystem.h"
#include"GameTree.h"
#include<thread>


struct System {

	InputSystem inputSystem;
	DisplaySystem displaySystem;

	ChessBoard board;

	GameTree* tree = nullptr;
	const int depth = 2;

	std::thread* evaluatorThread = nullptr;
	bool doneEvaluating = false;


	bool holdingPiece = false;
	ChessPiecePosition heldPiece;

	struct {
		bool isUpgrading = false;
		BoardPosition posFrom = BoardPosition(-1, -1);
		BoardPosition posTo = BoardPosition(-1, -1);
		bool color = true;
	} upgradedPawn;

	std::list<Move> availableMoves;

	System(const string& imagesPath) : displaySystem(imagesPath) {}

	~System();

	void start();

	void update();

	void pickupPiece();

	void showHoldingPiece();

	void dropPiece(const BoardPosition& from, const BoardPosition& to);

	void makeMove(const Move& move);

	void showPawnUpgrades(const int row, const int col, const bool white, const float padding);

	void printGameState();


	void run() {

		sf::Clock clock;

		sf::RenderWindow* window = displaySystem.window;

		inputSystem.window = window;

		start();

		// run the program as long as the window is open
		while (window->isOpen())
		{
			clock.restart();

			inputSystem.update();

			displaySystem.clear();

			update();

			displaySystem.display();

			while (clock.getElapsedTime().asMicroseconds() < 1000000 / FPS) {}
		}

	}

};

#endif // !SYSTEM_H