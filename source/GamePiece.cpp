#include"GamePiece.h"
#include"System.h"

using sf::Vector2i;


GamePiece::GamePiece(const std::string& filePath,
	int sizeX, int sizeY, int posX, int posY) : 
	GameObject(filePath, sizeX, sizeY, posX, posY) {
}


void GamePiece::start() {

}


void GamePiece::update(float deltaTime) {
	System* sys = System::getInstance();

	bool mousePressed = sys->isMouseHeld();

	if (mousePressed) {
		Vector2i mousePos = sys->getMousePos();
		pos = sf::Vector2f((float)mousePos.x, (float)mousePos.y);
	}
	

}




