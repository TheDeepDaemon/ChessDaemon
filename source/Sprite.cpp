#include"Sprite.h"
#include"util.h"
#include"System.h"



Sprite::Sprite(const string& filePath, float sizeX, float sizeY, bool isCentered) {

    centered = isCentered;

    // create texture and load from file
    texture = loadTexture(filePath);

    sf::Vector2u texSize = texture->getSize();
    originalSize = Vector2f((float)texSize.x, (float)texSize.y);

    sprite = new sf::Sprite(*texture);

    setSize(sizeX, sizeY);

    System::addSprite(this);

}


// when deleting the sprite, also delete the sfml objects
Sprite::~Sprite() {
    delete texture;
    texture = nullptr;
    delete sprite;
    sprite = nullptr;
}


// set size in pixels
void Sprite::setSize(float x, float y) {
    scale = Vector2f(x / originalSize.x, y / originalSize.y);
}


// set how much the image is scaled
void Sprite::setScale(float x, float y) {
    scale = Vector2f(x, y);
}


// get size in pixels
Vector2f Sprite::getSize() {
    return Vector2f(scale.x * originalSize.x, scale.y * originalSize.y);
}


void Sprite::draw(const Vector2f& pos) {
    sprite->setScale(scale);
    sf::Vector2f size = sf::Vector2f(scale.x * originalSize.x, scale.y * originalSize.y);
    
    if (centered) {
        sprite->setPosition(pos - (size / 2.0f));
    }
    else {
        sprite->setPosition(pos);
    }

    Vector2f position = sprite->getPosition();

    System::drawSprite(sprite);

}


void Sprite::draw(float x, float y) {
    draw(Vector2f(x, y));
}
