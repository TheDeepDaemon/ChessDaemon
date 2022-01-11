#include"headers.h"
#include"GameObject.h"
#include"System.h"
#include"util.h"


GameObject::GameObject(const string& filePath,
    float sizeX, float sizeY, float posX, float posY) {
    sprite = new Sprite(filePath, sizeX, sizeY);
    pos = sf::Vector2f(posX, posY);
}


void GameObject::draw() {
    sprite->draw(pos);
    postDraw();
}


Vector2f GameObject::getSprSize() {
    return sprite->getSize();
}