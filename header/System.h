#ifndef SYSTEM_H
#define SYSTEM_H
#include"headers.h"

class Sprite;
struct GameObject;
struct Board;
class PieceTextures;


struct System {
private:
    static System* instance;

    // true if the left mouse button
    // goes from up to down
    bool mouseDown = false;

    // true if the left mouse button is currently down
    bool mouseCurrentlyPressed = false;

    // true if the left mouse button goes from down to up
    bool mouseUp = false;

    // mouse position in window
    int mouseX = 0;
    int mouseY = 0;

    // main window for rendering everything in
    sf::RenderWindow* mainWindow = nullptr;

    // frames per second
    const int FPS = 60;

    // all sprites in the game, store references 
    // so that they can be deleted
    vector<Sprite*> spriteCollection;

    // all game objects that are shown in the window
    unordered_map<string, GameObject*> gameObjects;

    // the gameboard contains all board information
    Board* gameBoard = nullptr;

    // create the system
    System();


public:

    static void initInstance();

    static System* getInstance();

    static void addSprite(Sprite* sprite);
    
    ~System();

    sf::Vector2u getScreenSize();

    void pollEvents();

    void updateMousePressed();

    void updateMousePosition();

    void updateInputs();

    void init();

    void display();

    static void drawSprite(sf::Sprite* sprite);

    void run();

    template<typename T>
    GameObject* newGameObject(const string& name, const string& filePath,
        float sizeX=1.0f, float sizeY=1.0f, float posX=0.0f, float posY=0.0f);

    template<typename T>
    GameObject* newGameObject(const string& name, const string& filePath,
        float sizeX=1.0f, float sizeY=1.0f, const Vector2f& pos = Vector2f(0, 0));

    GameObject* getGameObject(const string& name);

    bool isMousePressed();
    bool isMouseHeld();
    bool isMouseReleased();
    sf::Vector2i getMousePos();


};


#endif // !SYSTEM_H