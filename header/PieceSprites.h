#ifndef PIECE_SPRITES_H
#define PIECE_SPRITES_H
#include"headers.h"

class Sprite;


class PieceSprites {
private:
    vector<Sprite*> sprites;

public:
    PieceSprites(float sizeX, float sizeY);
    ~PieceSprites();

    Sprite* get(const ChessPiece& pieceType);

};


#endif // !PIECE_SPRITES_H