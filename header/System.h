#ifndef SYSTEM_H
#define SYSTEM_H
#include<SFML/Graphics.hpp>
#include<string>
#include<unordered_map>

struct GameObject;


struct System {
    struct {
        bool isBeingPressedDown = false;
        bool isCurrentlyPressed = false;
        bool released = false;

        int xPos = 0;
        int yPos = 0;

    } mouseInfo;

    sf::RenderWindow* mainWindow = nullptr;

    const int FPS = 60;

    std::unordered_map<std::string, GameObject*> sprites;

    System(int width, int height);
    ~System();


    void pollEvents();

    void updateMousePressed();

    void updateMousePosition();

    void updateInputs();

    void update(float deltaTime);

    void display();

    void run();

    void newGameObject(const std::string& name, const std::string& filePath,
        int sizeX, int sizeY, int posX=0, int posY=0);


};


#endif // !SYSTEM_H