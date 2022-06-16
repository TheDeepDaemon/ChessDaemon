#ifndef DISPLAY_SYSTEM_H
#define DISPLAY_SYSTEM_H
#include"Headers.h"



struct DisplaySystem {

	sf::RenderWindow* window = nullptr;
	sf::RenderTexture* canvas = nullptr;
	std::list<Texture*> textures;
	std::unordered_map<string, Sprite*> sprites;


	DisplaySystem(const string& imagesPath);

	~DisplaySystem();

	void draw(const string& name, const Vector2f& pos, const float width, const float height);

	void draw(const string& name, const Vector2f& pos);

	void drawCentered(const string& name, const Vector2f& pos);

	void setSpriteSize(const string& name, const float width, const float height);

	void draw(const sf::Drawable& drawable);

	void clear();

	void display();

	sf::FloatRect getSpriteBounds(const string& name);

};

#endif // !DISPLAY_SYSTEM_H