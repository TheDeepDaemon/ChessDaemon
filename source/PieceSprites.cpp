#include"PieceSprites.h"
#include"constants.h"
#include"util.h"
#include"Sprite.h"


// load the sprites from the .png files, store references to them
PieceSprites::PieceSprites(float sizeX, float sizeY) : sprites((size_t)ChessPiece::NUM_PIECE_TYPES, nullptr) {
	
	float pieceSizeX = sizeX * 3.0f / 4.0f;
	float pieceSizeY = sizeY * 3.0f / 4.0f;
	bool centrd = true;

	sprites[(size_t)ChessPiece::WHITE_KING] = new Sprite("white king.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::WHITE_QUEEN] = new Sprite("white queen.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::WHITE_ROOK] = new Sprite("white rook.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::WHITE_BISHOP] = new Sprite("white bishop.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::WHITE_KNIGHT] = new Sprite("white knight.png", pieceSizeX, pieceSizeY, centrd);

	Sprite* pawn = new Sprite("white pawn.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::WHITE_PAWN] = pawn;
	sprites[(size_t)ChessPiece::WHITE_EN_PASSANTABLE_PAWN] = pawn;


	sprites[(size_t)ChessPiece::BLACK_KING] = new Sprite("black king.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::BLACK_QUEEN] = new Sprite("black queen.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::BLACK_ROOK] = new Sprite("black rook.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::BLACK_BISHOP] = new Sprite("black bishop.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::BLACK_KNIGHT] = new Sprite("black knight.png", pieceSizeX, pieceSizeY, centrd);

	pawn = new Sprite("black pawn.png", pieceSizeX, pieceSizeY, centrd);
	sprites[(size_t)ChessPiece::BLACK_PAWN] = pawn;
	sprites[(size_t)ChessPiece::BLACK_EN_PASSANTABLE_PAWN] = pawn;
	

}


// delete all sprites when done
PieceSprites::~PieceSprites() {

	for (size_t i = 0; i < sprites.size(); i++) {
		if (sprites[i] != nullptr) {
			delete sprites[i];
			sprites[i] = nullptr;
		}
	}

}


// access the texture corresponding to a specific piece
Sprite* PieceSprites::get(const ChessPiece& pieceType) {
	return sprites[(size_t)pieceType];
}

