#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include<SFML/Graphics.hpp>



struct GameObject {
	sf::Sprite* sprite = nullptr;
	sf::Texture* texture = nullptr;

	sf::Vector2f pos = sf::Vector2f(0, 0);
	sf::Vector2f size = sf::Vector2f(1, 1);

	GameObject(const std::string& filePath,
		int sizeX, int sizeY, int posX, int posY);

	~GameObject();

	virtual void start() = 0;

	virtual void update(float deltaTime) = 0;

	void updateSprite();

};



#endif // !GAMEOBJECT_H

