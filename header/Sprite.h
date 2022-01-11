#ifndef SPRITE_H
#define SPRITE_H
#include"headers.h"


// basically a wrapper around sf::Sprite
class Sprite {
private:
	sf::Sprite* sprite = nullptr;
	sf::Texture* texture = nullptr;

	Vector2f scale = sf::Vector2f(1, 1);
	Vector2f originalSize;

	bool centered = true;


public:
	Sprite(const string& filePath, float sizeX, float sizeY, bool isCentered=true);

	~Sprite();

	
	void draw(float x, float y);
	void draw(const Vector2f& pos);

	void setSize(float x, float y);
	void setScale(float x, float y);

	Vector2f getSize();


};


#endif // !SPRITE_H

