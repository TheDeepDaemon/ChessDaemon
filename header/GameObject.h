#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include<SFML/Graphics.hpp>



struct GameObject {
	sf::Sprite* sprite = nullptr;
	sf::Texture* texture = nullptr;

	GameObject(const std::string& filePath,
		int sizeX, int sizeY, int posX, int posY);

	~GameObject();

	virtual void start() = 0;

	virtual void update(float deltaTime) = 0;

};



#endif // !GAMEOBJECT_H

