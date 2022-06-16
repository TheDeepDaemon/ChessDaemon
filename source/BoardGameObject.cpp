#include"BoardGameObject.h"
#include"PieceSprites.h"
#include"ChessBoard.h"
#include"System.h"
#include"util.h"


using sf::Vector2i;


PieceSprites* createPieceSprites(const Vector2f& boardSize) {
	Vector2f temp = boardSize - (boardSize * (float)Constants::BOARD_EDGE);
	Vector2f cellSize = temp / (float)BOARD_SIDE_LENGTH;
	return new PieceSprites(cellSize.x, cellSize.y);
}


BoardGameObject::BoardGameObject(const string& filePath,
	float sizeX, float sizeY, float posX, float posY) :
	GameObject(filePath, sizeX, sizeY, posX, posY), board(new ChessBoard()),
	pieceSprites(createPieceSprites(getSprSize())) {
	movesContainer.reserve(Constants::BOARD_SIZE);
}


inline void print(const string& name, const Vector2f& vec) {
	cout << name << " = <" << vec.x << ", " << vec.y << ">\n";
}


inline void print(const string& name, const float x, const float y) {
	print(name, Vector2f(x, y));
}


inline void print(const string& name, const Vector2i& vec) {
	cout << name << " = <" << vec.x << ", " << vec.y << ">\n";
}



Vector2f BoardGameObject::getBoardSize() {
	Vector2f boardSize = getSprSize();
	return boardSize - (boardSize * (float)Constants::BOARD_EDGE);
}


Vector2f BoardGameObject::getBoardEdge() {
	return pos - (getBoardSize() / 2.0f) - Vector2f(2, 2);
}




inline Vector2f boardToScreenPos(
	const int row, const int col, const Vector2f boardEdge, const Vector2f cellSize) {
	float gridX = (float)col;
	float gridY = (float)BOARD_SIDE_LENGTH - row - 1;
	float pieceX = (cellSize.x / 2) + boardEdge.x + (gridX * cellSize.x);
	float pieceY = (cellSize.y / 2) + boardEdge.y + (gridY * cellSize.y);
	return Vector2f(pieceX, pieceY);
}




void BoardGameObject::isInCheck() {
	if (board->isKingInCheck<true>()) {
		cout << "White in check\n";
	}
	if (board->isKingInCheck<false>()) {
		cout << "Black in check\n";
	}
}




void BoardGameObject::showLegalMoves(const int row, const int col) {
	Vector2f boardSize = getBoardSize();

	// the top left edge of the board
	Vector2f boardEdge = getBoardEdge();

	// how big is a square on the board?
	Vector2f cellSize = boardSize * 1.01f / (float)BOARD_SIDE_LENGTH;

	Vector2f sprSize = cellSize / 3.0f;
	Sprite* spr = new Sprite("green dot.png", sprSize.x, sprSize.y);

	for (int i = 0; i < BOARD_SIDE_LENGTH; i++) {
		for (int j = 0; j < BOARD_SIDE_LENGTH; j++) {
			if (board->isLegalMoveNoCheck(row, col, i, j)) {
				Vector2f pos = boardToScreenPos(i, j, boardEdge, cellSize);
				spr->draw(pos);
			}
		}
	}

}



void BoardGameObject::postDraw() {
	System* sys = System::getInstance();

	Vector2f boardSize = getBoardSize();
	
	// the top left edge of the board
	Vector2f boardEdge = getBoardEdge();

	// how big is a square on the board?
	Vector2f cellSize = boardSize * 1.01f / (float)BOARD_SIDE_LENGTH;

	for (int i = 0; i < BOARD_SIDE_LENGTH; i++) {
		for (int j = 0; j < BOARD_SIDE_LENGTH; j++) {
			if (!mouseDown || (mouseDown && (prevPos != Vector2i(j, i)))) {
				ChessPiece piece = board->get(i, j);
				if (piece != ChessPiece::EMPTY) {
					Sprite* pSpr = pieceSprites->get(piece);
					pSpr->draw(boardToScreenPos(i, j, boardEdge, cellSize));
				}
			}
		}
	}

	if (mouseDown) {
		ChessPiece piece = board->get(prevPos.y, prevPos.x);
		Sprite* pSpr = pieceSprites->get(piece);
		if (pSpr != nullptr) {
			Vector2f mousePos = Vector2f(sys->getMousePos());
			pSpr->draw(mousePos.x, mousePos.y);

			showLegalMoves(prevPos.y, prevPos.x);
		}

	}

}


// screen position to board position
pair<bool, Vector2i> BoardGameObject::getGridPos(const Vector2f& pos) {

	Vector2f boardEdge = getBoardEdge();
	Vector2f boardSize = getBoardSize();

	Vector2f posOnBoard = pos - boardEdge;

	if (posOnBoard.x < boardSize.x
		&& posOnBoard.y < boardSize.y
		&& posOnBoard.x >= 0 && posOnBoard.y >= 0) {

		Vector2f cellSize = boardSize / (float)BOARD_SIDE_LENGTH;
		Vector2i gridPos = Vector2i(
			int(posOnBoard.x / cellSize.x),
			int(posOnBoard.y / cellSize.y));

		// double check to make sure 
		// the square on the board exists
		if (gridPos.x < BOARD_SIDE_LENGTH
			&& gridPos.y < BOARD_SIDE_LENGTH) {
			return pair<bool, Vector2i>(true, Vector2i(gridPos.x, BOARD_SIDE_LENGTH - gridPos.y - 1));
		}
	}
	return pair<bool, Vector2i>(false, Vector2i(-1, -1));
}


void BoardGameObject::update() {
	System* sys = System::getInstance();

	if (mouseDown) {

		if (sys->isMouseReleased()) {

			Vector2f mousePos = Vector2f(sys->getMousePos());

			pair<bool, Vector2i> mouseGridPos = getGridPos(mousePos);

			// if the mouse drops the piece on the board
			if (mouseGridPos.first) {

				int rowTo = mouseGridPos.second.y;
				int colTo = mouseGridPos.second.x;

				int row = prevPos.y;
				int col = prevPos.x;

				//cout << "________________\n";

				// check if legal move
				// if it is, drop a piece there
				if (board->isLegalMoveNoCheck(row, col, rowTo, colTo)) {

					int colPrev = prevPos.x;
					int rowPrev = prevPos.y;

					board->makeLegalMove(row, col, rowTo, colTo);
					isInCheck();
					
				}
			}

			mouseDown = false;
		}

	}
	else {
		
		// if mouse goes down
		if (sys->isMousePressed()) {

			Vector2f mousePos = Vector2f(sys->getMousePos());

			pair<bool, Vector2i> mouseGridPos = getGridPos(mousePos);

			if (mouseGridPos.first) {
				mouseDown = true;
				prevPos = mouseGridPos.second;
			}
		}
	}

}

