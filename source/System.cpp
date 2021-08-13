#include"System.h"
#include<iostream>
#include<cstdint>
#include"GameObject.h"
#include"GamePiece.h"


using namespace std;

System* System::instance = nullptr;


System::System(int width, int height) {
    mainWindow = new sf::RenderWindow(sf::VideoMode(width, height), "ChessDaemon");
}


void System::initInstance(int width, int height) {
    if (instance == nullptr) {
        instance = new System(width, height);
    }
}


System* System::getInstance() {
    return instance;
}


System::~System() {

    for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin();
        it != gameObjects.end(); ++it) {
        GameObject* gameObj = it->second;
        delete gameObj;
    }

    delete mainWindow;
    instance = nullptr;
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
                mouseBeingPressedDown = true;
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                mouseReleased = true;
            }
            break;
        }

    }
}


void System::updateMousePressed() {
    mouseCurrentlyPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}


void System::updateMousePosition() {
    sf::Vector2i pos = sf::Mouse::getPosition(*mainWindow);
    mouseX = pos.x;
    mouseY = pos.y;
}


void System::updateInputs() {
    mouseBeingPressedDown = false;
    mouseReleased = false;
    pollEvents();
    updateMousePressed();
    updateMousePosition();
}





void System::run() {
    int64_t frameTime = 1000000 / (int64_t)FPS;

    sf::Clock clock;

    sf::Clock runtime;
    runtime.restart();
    float lastUpdate = 0.0f;

    start();

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



template<typename T>
void System::newGameObject(const std::string& name, const std::string& filePath, 
    int sizeX, int sizeY, int posX, int posY) {
    T* go = new T(filePath, sizeX, sizeY, posX, posY);
    gameObjects.insert(pair<string, GameObject*>(name, go));
}


GameObject* System::getGameObject(const std::string& name) {
    return gameObjects[name];
}


bool System::isMousePressed() {
    return mouseBeingPressedDown;
}

bool System::isMouseHeld() {
    return mouseCurrentlyPressed;
}

bool System::isMouseReleased() {
    return mouseReleased;
}




void System::init() {
    newGameObject<GamePiece>("piece", "gamepiece.png", 64, 64, 400, 400);
}


void System::start() {
    init();
    for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin();
        it != gameObjects.end(); ++it) {
        GameObject* gameObj = it->second;
        gameObj->start();
    }
}


void System::update(float deltaTime) {
    for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin();
        it != gameObjects.end(); ++it) {
        GameObject* gameObj = it->second;
        gameObj->update(deltaTime);
    }
}


void System::display() {
    mainWindow->clear(sf::Color::Black);
    for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin();
        it != gameObjects.end(); ++it) {
        GameObject* gameObj = it->second;
        gameObj->updateSprite();
        mainWindow->draw(*(gameObj->sprite));
    }
    mainWindow->display();
}


sf::Vector2i System::getMousePos() { 
    return sf::Vector2i(mouseX, mouseY); 
}

