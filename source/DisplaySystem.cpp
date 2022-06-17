#include"DisplaySystem.h"
#include<filesystem>





using std::cout;




string removeExt(const string& fname) {
	uint lastPeriod = fname.size();
	for (uint i = 0; i < fname.size(); i++) {
		if (fname[i] == '.') {
			lastPeriod = i;
		}
	}
	return fname.substr(0, lastPeriod);
}


DisplaySystem::DisplaySystem(const string& imagesPath) {
	window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Chess");

	canvas = new sf::RenderTexture();
	canvas->create(SCREEN_WIDTH, SCREEN_HEIGHT);

	std::filesystem::path path = imagesPath;

	for (const std::filesystem::directory_entry& dir_entry : std::filesystem::directory_iterator{ path }) {
		string name = removeExt(dir_entry.path().filename().string());

		// ignore the readme file
		if (name != "readme") {
			string imgPath(dir_entry.path().string());

			Texture* texture = new Texture();
			texture->loadFromFile(imgPath);

			textures.push_back(texture);

			sprites[name] = new Sprite(*texture);
		}
	}
}


DisplaySystem::~DisplaySystem() {

	if (window != nullptr) {
		delete window;
	}

	if (canvas != nullptr) {
		delete canvas;
	}

	for (auto it = textures.begin(); it != textures.end(); ++it) {
		Texture* ptr = *it;
		if (ptr != nullptr) {
			delete ptr;
		}
	}

	for (auto it = sprites.begin(); it != sprites.end(); ++it) {
		Sprite* ptr = it->second;
		if (ptr != nullptr) {
			delete ptr;
		}
	}


}


Vector2f getCenteredPos(Sprite* spr, const float x, const float y) {
	if (spr) {
		sf::FloatRect bounds = spr->getGlobalBounds();
		return Vector2f(x - (bounds.width / 2.0f), y - (bounds.height / 2.0f));
	}
	else {
		return Vector2f();
	}
}


void DisplaySystem::setSpriteSize(const string& name, const float width, const float height) {
	Sprite* spr = sprites[name];
	if (spr != nullptr) {
		sf::FloatRect bounds = spr->getLocalBounds();
		float sx = width / bounds.width;
		float sy = height / bounds.height;
		spr->setScale(sx, sy);
	}
}




void DisplaySystem::draw(const string& name, const Vector2f& pos, const float width, const float height) {
	Sprite* spr = sprites[name];
	if (spr != nullptr) {
		spr->setPosition(pos);

		sf::FloatRect bounds = spr->getLocalBounds();
		float sx = width / bounds.width;
		float sy = height / bounds.height;
		spr->setScale(sx, sy);

		canvas->draw(*spr);
	}
}


void DisplaySystem::draw(const string& name, const Vector2f& pos) {
	Sprite* spr = sprites[name];
	if (spr != nullptr) {
		spr->setPosition(pos);
		canvas->draw(*spr);
	}
}


void DisplaySystem::drawCentered(const string& name, const Vector2f& pos) {
	Sprite* spr = sprites[name];
	if (spr != nullptr) {
		sf::FloatRect bounds = spr->getGlobalBounds();
		Vector2f cPos(pos.x - (bounds.width / 2.0f), pos.y - (bounds.height / 2.0f));
		spr->setPosition(cPos);
		canvas->draw(*spr);
	}
}


void DisplaySystem::draw(const sf::Drawable& drawable) {
	canvas->draw(drawable);
}


sf::FloatRect DisplaySystem::getSpriteBounds(const string& name) {
	Sprite* spr = sprites[name];
	if (spr != nullptr) {
		return spr->getGlobalBounds();
	}
	else {
		return sf::FloatRect();
	}
}


void DisplaySystem::clear() {
	if (window != nullptr) {
		window->clear();
	}
}


void DisplaySystem::display() {
	if (window != nullptr) {
		Sprite sprite(canvas->getTexture());
		sprite.setScale(1.0, -1.0);
		sprite.setPosition(0.0, SCREEN_HEIGHT);
		window->draw(sprite);
		window->display();
		canvas->clear();
	}
}
