#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include<SFML/Graphics.hpp>



struct GameObject {
	sf::Sprite* sprite = nullptr;

	virtual void update(float deltaTime) = 0;

	GameObject(const std::string& filePath,
		int sizeX, int sizeY, int posX = 0, int posY = 0);

	~GameObject();

};



#endif // !GAMEOBJECT_H

