#ifndef BOARD_GO_H
#define BOARD_GO_H
#include"GameObject.h"

class ChessBoard;
class PieceSprites;
class GamePiece;


struct BoardGameObject : public GameObject {

	ChessBoard* board;

	PieceSprites* pieceSprites;

	bool mouseDown = false;

	sf::Vector2i prevPos;

	BoardGameObject(const string& filePath,
		float sizeX, float sizeY, float posX, float posY);

	void draw();

	void postDraw();

	void update();

	Vector2f getBoardSize();

	Vector2f getBoardEdge();

	pair<bool, sf::Vector2i> getGridPos(Vector2f pos);

	void showLegalMoves(int row, int col);

};

#endif // !BOARD_GO_H