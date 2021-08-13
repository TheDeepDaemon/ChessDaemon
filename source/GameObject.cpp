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

    pos = sf::Vector2f(posX, posY);
    size = sf::Vector2f(sizeX, sizeY);
    updateSprite();
    
}

GameObject::~GameObject() {
    delete texture;
    delete sprite;
}

void GameObject::updateSprite() {
    sprite->setScale(size.x / sprite->getLocalBounds().width, size.x / sprite->getLocalBounds().height);
    sprite->setPosition(pos - (size / 2.0f));
}
