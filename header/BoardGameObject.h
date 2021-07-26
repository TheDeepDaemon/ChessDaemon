#ifndef BOARD_IMAGE_H
#define BOARD_IMAGE_H
#include"GameObject.h"


struct BoardGameObject : public GameObject {

	BoardGameObject(const std::string& filePath,
		int sizeX, int sizeY, int posX, int posY);

	void start();

	void update(float deltaTime);

};

#endif