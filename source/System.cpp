#include"System.h"
#include<iostream>


using namespace std;



System::System(int width, int height) {
    mainWindow = new sf::Window(sf::VideoMode(width, height), "ChessDaemon");
}

System::~System() {
    delete mainWindow;
}




void System::pollEvents() {
    sf::Event event;
    while (mainWindow->pollEvent(event))
    {
        switch (event.type) {
        case sf::Event::Closed:
            mainWindow->close();
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                mouseInfo.isBeingPressedDown = true;
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                mouseInfo.released = true;
            }
            break;
        }

    }
}


void System::updateMousePressed() {
    mouseInfo.isCurrentlyPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}


void System::updateMousePosition() {
    sf::Vector2i pos = sf::Mouse::getPosition(*mainWindow);
    mouseInfo.xPos = pos.x;
    mouseInfo.yPos = pos.y;
}


void System::updateInputs() {
    pollEvents();
    updateMousePressed();
}


void System::update() {

}


void System::display() {

}


void System::run() {
    float frameTime = 1 / FPS;

    sf::Clock clock;

    while (mainWindow->isOpen()) {
        clock.restart();
        updateInputs();
        update();
        display();


        while (clock.getElapsedTime().asSeconds() < frameTime) {
        }
    }
}

