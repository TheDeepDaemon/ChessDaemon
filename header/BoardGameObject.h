#ifndef BOARD_GO_H
#define BOARD_GO_H
#include"GameObject.h"
#include<memory>
#include"BoardPosition.h"

class ChessBoard;
class PieceSprites;
class GamePiece;


struct BoardGameObject : public GameObject {

	std::unique_ptr<ChessBoard> board;

	std::unique_ptr<PieceSprites> pieceSprites;

	std::vector<BoardPosition> movesContainer;

	bool mouseDown = false;

	sf::Vector2i prevPos;

	BoardGameObject(const string& filePath,
		float sizeX, float sizeY, float posX, float posY);

	void postDraw();

	void update();

	Vector2f getBoardSize();

	Vector2f getBoardEdge();

	pair<bool, sf::Vector2i> getGridPos(const Vector2f& pos);

	void showLegalMoves(const int row, const int col);

	void isInCheck();

};

#endif // !BOARD_GO_H