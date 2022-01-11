#include"headers.h"
#include"System.h"
#include"GameObject.h"
#include"BoardGameObject.h"
#include"util.h"




System* System::instance = nullptr;


// initialize the main window when creating the system
System::System() {
    mainWindow = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "ChessDaemon");
}


// call the constructor internally, since it is a singleton
void System::initInstance() {
    if (instance == nullptr) {
        instance = new System();
    }
}


System* System::getInstance() {
    return instance;
}

void System::addSprite(Sprite* sprite) {
    if (instance != nullptr) {
        instance->spriteCollection.push_back(sprite);
    }
}


System::~System() {
    unordered_map<string, GameObject*>::iterator it;

    // delete all game objects before deleting the system
    for (it = gameObjects.begin(); it != gameObjects.end(); ++it) {
        GameObject* gameObj = it->second;
        delete gameObj;
        gameObj = nullptr;
    }

    for (size_t i = 0; i < spriteCollection.size(); i++) {
        delete spriteCollection[i];
    }

    spriteCollection.clear();

    
    // delete the window
    delete mainWindow;
    mainWindow = nullptr;

    // set the single instance to null
    instance = nullptr;
}


sf::Vector2u System::getScreenSize() {
    return mainWindow->getSize();
}


// go through events and respond accordingly
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
                mouseDown = true;
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                mouseUp = true;
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


// update variables that keep track of inputs
void System::updateInputs() {
    mouseDown = false;
    mouseUp = false;
    pollEvents();
    updateMousePressed();
    updateMousePosition();
}


// call this function to run the system
void System::run() {

    // divide number of microseconds by number of frames
    int64_t frameTime = 1000000 / (int64_t)FPS;

    sf::Clock clock;

    init();

    // this is the main game loop
    while (mainWindow->isOpen()) {
        clock.restart();
        updateInputs();
        display();

        while (clock.getElapsedTime().asMicroseconds() < frameTime) {
        }
    }
}



template<typename T>
GameObject* System::newGameObject(const string& name, const string& filePath, 
    float sizeX, float sizeY, float posX, float posY) {
    T* go = new T(filePath, sizeX, sizeY, posX, posY);
    gameObjects.insert(pair<string, GameObject*>(name, go));
    return go;
}

template<typename T>
GameObject* System::newGameObject(const string& name, const string& filePath,
    float sizeX, float sizeY, const Vector2f& pos) {
    T* go = new T(filePath, sizeX, sizeY, pos.x, pos.y);
    gameObjects.insert(pair<string, GameObject*>(name, go));
    return go;
}


GameObject* System::getGameObject(const string& name) {
    return gameObjects[name];
}


bool System::isMousePressed() {
    return mouseDown;
}

bool System::isMouseHeld() {
    return mouseCurrentlyPressed;
}

bool System::isMouseReleased() {
    return mouseUp;
}


void System::init() {
    
    Vector2f boardPos = fractionToLoc(0.5, 0.5);
    GameObject* board = newGameObject<BoardGameObject>("board", "board.jpeg", 1024, 1024, boardPos);


}




void System::drawSprite(sf::Sprite* sprite) {
    if (instance != nullptr && sprite != nullptr) {
        instance->mainWindow->draw(*(sprite));
    }
}



// draw all game objects to the screen
void System::display() {

    // update gameobjects before displaying
    unordered_map<string, GameObject*>::iterator it;
    for (it = gameObjects.begin(); it != gameObjects.end(); ++it) {
        it->second->update();
    }

    // clear screen before displaying
    mainWindow->clear(sf::Color::Black);

    // go through each game object and draw it
    for (it = gameObjects.begin(); it != gameObjects.end(); ++it) {
        it->second->draw();
    }

    // display to the screen
    mainWindow->display();
}


sf::Vector2i System::getMousePos() { 
    return sf::Vector2i(mouseX, mouseY); 
}

