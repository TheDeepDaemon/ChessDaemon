#ifndef SYSTEM_H
#define SYSTEM_H
#include<SFML/Graphics.hpp>


struct System {
    struct {
        bool isBeingPressedDown = false;
        bool isCurrentlyPressed = false;
        bool released = false;

        int xPos = 0;
        int yPos = 0;

    } mouseInfo;

    sf::Window* mainWindow = nullptr;

    const int FPS = 60;

    std::vector<sf::Sprite> objects;



    System(int width, int height);
    ~System();


    void pollEvents();

    void updateMousePressed();

    void updateMousePosition();

    void updateInputs();

    void update();

    void display();

    void run();

};


#endif // !SYSTEM_H