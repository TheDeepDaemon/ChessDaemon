#ifndef GAMEPIECE_H
#define GAMEPIECE_H
#include"GameObject.h"


struct GamePiece : public GameObject {

	int boardRow = -1;
	int boardCol = -1;

	GamePiece(const std::string& filePath,
		int sizeX, int sizeY, int posX, int posY);

	void start();

	void update(float deltaTime);


};

#endif 