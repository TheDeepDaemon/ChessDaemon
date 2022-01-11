#ifndef UTIL_H
#define UTIL_H
#include"headers.h"


// return the sign of integer x
int sign(int x);


// take a board position and convert it into a position on the screen
Vector2f boardToScreenPos(Vector2f boardEdge, Vector2f boardSize, int row, int col, bool inverted);


// create a new texture and return a pointer to it
sf::Texture* loadTexture(const string& filePath);


// convert proportion of the screen height to 
Vector2f fractionToLoc(float x, float y);


#endif