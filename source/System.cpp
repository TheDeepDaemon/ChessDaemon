#include"System.h"
#include<iostream>
#include<cstdint>
#include"GameObject.h"


using namespace std;



System::System(int width, int height) {
    mainWindow = new sf::RenderWindow(sf::VideoMode(width, height), "ChessDaemon");
}

System::~System() {

    for (std::unordered_map<std::string, GameObject*>::iterator it = sprites.begin();
        it != sprites.end(); ++it) {
        GameObject* gameObj = it->second;
        delete gameObj;
    }

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
    mouseInfo.isBeingPressedDown = false;
    mouseInfo.released = false;
    pollEvents();
    updateMousePressed();
    updateMousePosition();
}


void System::update(float deltaTime) {
    for (std::unordered_map<std::string, GameObject*>::iterator it = sprites.begin();
        it != sprites.end(); ++it) {
        GameObject* gameObj = it->second;
        gameObj->update(deltaTime);
    }
}


void System::display() {
    for (std::unordered_map<std::string, GameObject*>::iterator it = sprites.begin();
        it != sprites.end(); ++it) {
        GameObject* gameObj = it->second;
        mainWindow->draw(*(gameObj->sprite));
    }
}


void System::run() {
    int64_t frameTime = 1000000 / (int64_t)FPS;

    sf::Clock clock;

    sf::Clock runtime;
    runtime.restart();
    float lastUpdate = 0.0f;

    while (mainWindow->isOpen()) {
        clock.restart();
        updateInputs();
        update(runtime.getElapsedTime().asSeconds() - lastUpdate);
        lastUpdate = runtime.getElapsedTime().asSeconds();
        display();


        while (clock.getElapsedTime().asMicroseconds() < frameTime) {
        }
    }
}




void System::newGameObject(const std::string& name, const std::string& filePath, 
    int sizeX, int sizeY, int posX, int posY) {
    GameObject* go = new GameObject(filePath, sizeX, sizeY, posX, posY);
    sprites.insert(pair<string, GameObject*>(name, go));
}
