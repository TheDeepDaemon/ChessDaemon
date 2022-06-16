#ifndef HEADERS_H
#define HEADERS_H
#include<SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<cstdint>
#include<unordered_map>

/*
* example screen sizes:
* 800, 600
* 960, 540
* 1024, 576
* 1600, 1200
* 1920, 1080
* 3840, 2160
*/

// size of the screen in pixels
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

// frames per second
#define FPS 30

// number of squares across one side of the board
#define BOARD_SIZE 8

// number of squares on the board total
#define NUM_SQUARES 64

// how much to scale a piece sprite
// relative to the square size
#define PIECE_SCALE 0.75f
#define GREEN_DOT_SCALE 0.5f

// whether to show a literal grid
// of squares on the board
#define GRID_IS_VISIBLE false

// size of the board's grid on the screen
#define BOARD_SIZE_ON_SCREEN (std::min(SCREEN_WIDTH, SCREEN_HEIGHT) * 3 / 4)

// how much of the board sprite is edge
#define BOARD_EDGE 0.06


using std::cout;
using std::cin;
using std::string;
using sf::Texture;
using sf::Sprite;
using sf::Vector2f;



typedef int64_t int64;
typedef uint64_t uint64;
typedef uint64 uint;



#endif // !HEADERS_H