#include"GameObject.h"
#include<iostream>

using namespace std;



GameObject::GameObject(const std::string& filePath,
    int sizeX, int sizeY, int posX, int posY) {
    sf::Texture texture;
    if (!texture.loadFromFile(filePath))
    {
        cout << "ERROR - File " << filePath << " not loaded.";
    }
    sprite = new sf::Sprite(texture, sf::IntRect(posX, posY, sizeX, sizeY));
}

GameObject::~GameObject() {
    delete sprite;
}