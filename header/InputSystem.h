#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#include"Headers.h"



struct InputSystem {

	sf::RenderWindow* window = nullptr;

	sf::Vector2i mousePos = sf::Vector2i(0, 0);
	bool mouseDown = false;
	bool mouseUp = false;

	void update() {
		mouseDown = false;
		mouseUp = false;

		sf::Event ev;
		while (window->pollEvent(ev))
		{
			switch (ev.type) {
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::MouseButtonPressed:
				if (ev.mouseButton.button == sf::Mouse::Button::Left) {
					mouseDown = true;
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (ev.mouseButton.button == sf::Mouse::Button::Left) {
					mouseUp = true;
				}
				break;
			}
		}

		sf::Vector2i mousePosRaw = sf::Mouse::getPosition(*window);

		sf::Vector2u wSize = window->getSize();

		// adjust it so it is always in the original coordinates
		// if the screen is resized
		int mx = (mousePosRaw.x * SCREEN_WIDTH) / wSize.x;
		int my = (mousePosRaw.y * SCREEN_HEIGHT) / wSize.y;

		mousePos = sf::Vector2i(mx, my);

	}

	sf::Vector2f getMousePos() {
		return sf::Vector2f(mousePos);
	}

};



#endif // !INPUT_SYSTEM_H