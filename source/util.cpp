#include"util.h"


// return the sign of integer x
int sign(int x) {
	if (x == 0) {
		return 0;
	}
	else {
		return (x > 0) ? 1 : -1;
	}
}


// take a board position and convert it into a position on the screen
Vector2f boardToScreenPos(Vector2f boardEdge, Vector2f boardSize, int row, int col, bool inverted) {
	float squareSizeX = boardSize.x / BOARD_SIDE_LENGTH;
	float squareSizeY = boardSize.y / BOARD_SIDE_LENGTH;
	float x = boardEdge.x + (col * squareSizeX);
	float y;

	if (inverted) {
		y = boardEdge.y + boardSize.y - (row * squareSizeY);
	}
	else {
		y = boardEdge.y + (row * squareSizeY);
	}

	return Vector2f(x, y);
}


sf::Texture* loadTexture(const string& filePath) {
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(filePath))
	{
		cout << "ERROR - File " << filePath << " not loaded.";
		return nullptr;
	}
	return texture;
}


Vector2f fractionToLoc(float x, float y) {
	return Vector2f(x * (float)SCREEN_WIDTH, y * (float)SCREEN_HEIGHT);
}
