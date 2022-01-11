#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include"headers.h"
#include"Sprite.h"


struct GameObject {
	Sprite* sprite;

	sf::Vector2f pos = sf::Vector2f(0, 0);

	GameObject(const string& filePath,
		float sizeX, float sizeY, float posX, float posY);


	void draw();

	virtual void postDraw() {}
	virtual void update() {}

	Vector2f getSprSize();

};

#endif // !GAMEOBJECT_H