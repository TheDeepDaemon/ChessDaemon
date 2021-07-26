#ifndef SYSTEM_H
#define SYSTEM_H
#include<SFML/Graphics.hpp>
#include<string>
#include<unordered_map>


struct GameObject;


struct System {
private:
    // true if the left mouse button
    // goes from up to down
    bool mouseBeingPressedDown = false;

    // true if the left mouse button is down, period
    bool mouseCurrentlyPressed = false;

    // true if the left mouse button goes from down to up
    bool mouseReleased = false;

    // mouse position in window
    int mouseX = 0;
    int mouseY = 0;

    // main window for rendering everything in
    sf::RenderWindow* mainWindow = nullptr;

    // frames per second
    const int FPS = 60;

    // all game objects that are shown in the window
    std::unordered_map<std::string, GameObject*> gameObjects;


public:

    System(int width, int height);
    ~System();


    void pollEvents();

    void updateMousePressed();

    void updateMousePosition();

    void updateInputs();

    void init();

    void start();

    void update(float deltaTime);

    void display();

    void run();

    template<typename T>
    void newGameObject(const std::string& name, const std::string& filePath,
        int sizeX, int sizeY, int posX=0, int posY=0);

    GameObject* getGameObject(const std::string& name);

    bool isMousePressed();
    bool isMouseHeld();
    bool isMouseReleased();


};


#endif // !SYSTEM_H