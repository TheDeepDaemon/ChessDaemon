#include"GameObject.h"
#include<iostream>

using namespace std;



GameObject::GameObject(const std::string& filePath,
    int sizeX, int sizeY, int posX, int posY) {
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(filePath))
    {
        cout << "ERROR - File " << filePath << " not loaded.";
    }
    sprite = new sf::Sprite(*texture);
    sprite->setScale(sizeX / sprite->getLocalBounds().width, sizeY / sprite->getLocalBounds().height);
}

GameObject::~GameObject() {
    delete texture;
    delete sprite;
}

