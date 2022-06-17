#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H
#include"Headers.h"
#include"BoardPosition.h"
#include"ChessPiece.h"


struct DisplaySystem;


struct BoardState {
	bool whiteToMove;
	BoardPosition whiteKing;
	BoardPosition blackKing;
	BoardPosition enpassPawn;

	bool whiteKingHasMoved;
	bool wqRookHasMoved;
	bool wkRookHasMoved;

	bool blackKingHasMoved;
	bool bqRookHasMoved;
	bool bkRookHasMoved;

	BoardState() :
		whiteToMove(true),
		whiteKing(-1, -1), blackKing(-1, -1),
		enpassPawn(-1, -1), whiteKingHasMoved(true),
		wqRookHasMoved(true), wkRookHasMoved(true),
		blackKingHasMoved(true), bqRookHasMoved(true),
		bkRookHasMoved(true) {
	}

	void copyBoardState(const BoardState& boardState) {
		whiteToMove = boardState.whiteToMove;

		whiteKing = boardState.whiteKing;
		blackKing = boardState.blackKing;
		enpassPawn = boardState.enpassPawn;

		whiteKingHasMoved = boardState.whiteKingHasMoved;
		wqRookHasMoved = boardState.wqRookHasMoved;
		wkRookHasMoved = boardState.wkRookHasMoved;

		blackKingHasMoved = boardState.blackKingHasMoved;
		bqRookHasMoved = boardState.bqRookHasMoved;
		bkRookHasMoved = boardState.bkRookHasMoved;
	}

	void operator=(const BoardState& boardState) {
		copyBoardState(boardState);
	}

};


class ChessBoard : public BoardState {
public:

	struct Move {
		BoardPosition from;
		BoardPosition to;
		bool enpassant = false;
		bool castlingKingSide = false;
		bool castlingQueenSide = false;
		ChessPieceType promotionTo = EMPTY;
		Move(const BoardPosition& from_, const BoardPosition& to_) :
			from(from_), to(to_), enpassant(false),
			castlingKingSide(false), castlingQueenSide(false), promotionTo(EMPTY) {}
	};

	// buffer to store chess pieces
	ChessPiece boardBuffer[NUM_SQUARES];

	const string sprName = "board";

	// position of the top left corner of the board
	sf::Vector2f boardPosition = sf::Vector2f(0, 0);

	// size of the board on screen in pixels
	const sf::Vector2f boardSizeOnScreen = sf::Vector2f(
		BOARD_SIZE_ON_SCREEN, BOARD_SIZE_ON_SCREEN);

	// size of an individual square
	const sf::Vector2f squareSize = sf::Vector2f(
		boardSizeOnScreen.x / (float)BOARD_SIZE,
		boardSizeOnScreen.y / (float)BOARD_SIZE);

	// size of the edge of the board sprite
	const sf::Vector2f boardEdge = sf::Vector2f(
		BOARD_EDGE * boardSizeOnScreen.x,
		BOARD_EDGE * boardSizeOnScreen.y);


	inline static bool isValid(const unsigned row, const unsigned col) {
		return (row < BOARD_SIZE) && (col < BOARD_SIZE);
	}

	inline static bool isValid(const BoardPosition& pos) {
		return isValid(pos.row, pos.col);
	}

	inline ChessPiece& get(const unsigned row, const unsigned col) {
		return boardBuffer[(row * BOARD_SIZE) + col];
	}

	inline ChessPiece& get(const BoardPosition& pos) {
		return get(pos.row, pos.col);
	}

	ChessBoard();
	ChessBoard(const ChessBoard& board);

	void addWhitePiece(const ChessPieceType type, const int row, const int col);

	void addBlackPiece(const ChessPieceType type, const int row, const int col);

	void addPiece(const ChessPiece piece, const int row, const int col);

	void addWhitePiece(const ChessPieceType type, const string& algebraicNot);

	void addBlackPiece(const ChessPieceType type, const string& algebraicNot);

	void setupBoard();

	void display(DisplaySystem* disp, const BoardPosition mask);

	void displayPossibleMoves(
		DisplaySystem* disp, const BoardPosition from, const std::list<Move>& possibleMoves);

	void setCenteredPosition(
		const float x = SCREEN_WIDTH / 2, const float y = SCREEN_HEIGHT / 2);

	BoardPosition screenToBoard(const sf::Vector2f& screenPos);

	sf::Vector2f boardToScreen(const BoardPosition& boardPos);

	bool isLegal(const BoardPosition& from, const BoardPosition& to);

	bool isKingInCheck(const bool white);

	void getPossibleMoves(const BoardPosition& pos, std::list<Move>& moves);

	void getAllPossibleMoves(const bool white,
		std::list<Move>& moves);

	bool wouldBeInCheck(const bool white, const Move& move);

	bool isCheckMated(const bool white);

	bool isDraw(const bool white);

	void movePiece(const BoardPosition& from, const BoardPosition& to);

	void makeMove(const Move& move);

	void debugLegalMoves(const BoardPosition& from);

	ChessBoard getNextPosition(const Move& move);

	void operator=(const BoardState& boardState) {
		copyBoardState(boardState);
	}

	void operator=(const ChessBoard& board);

private:

	void copyBoard(const ChessBoard& board) {
		memcpy(boardBuffer, board.boardBuffer, sizeof(ChessPiece) * NUM_SQUARES);
		copyBoardState(board);
	}

	// specific piece rules
	inline bool isLegalPawn(
		const BoardPosition& from, const BoardPosition& to,
		const BoardPosition& move, const bool white,
		const ChessPiece& pieceTo);

	inline bool isLegalRook(
		const BoardPosition& from, const BoardPosition& to,
		const BoardPosition& move);

	inline bool isLegalKnight(
		const BoardPosition& from, const BoardPosition& to,
		const BoardPosition& move);

	inline bool isLegalBishop(
		const BoardPosition& from, const BoardPosition& to,
		const BoardPosition& move);

	inline bool isLegalQueen(
		const BoardPosition& from, const BoardPosition& to,
		const BoardPosition& move);

	inline bool isLegalKing(
		const BoardPosition& from, const BoardPosition& to,
		const BoardPosition& move);

	bool isKingInCheck(const BoardPosition& pos);

	inline bool isClearForKing(const bool isWhite, const BoardPosition& kingPos, const int row, const int col);

	inline bool clearToCastle(const bool isWhite, const BoardPosition& pos, const int direction);

	bool isPiece(const int row, const int col, const ChessPiece piece);
	bool isPiece(const BoardPosition& pos, const ChessPiece piece);

	template<int rowDir, int colDir>
	ChessPiece lookInDirection(const BoardPosition& from);

	template<int rowDir, int colDir>
	bool isKnight(const BoardPosition& from, const ChessPiece& oppositeKnight);

	inline bool isColorPiece(const int row, const int col, const bool isWhite);

	inline bool isWhitePiece(const int row, const int col);

	inline bool isBlackPiece(const int row, const int col);

	inline bool isEmpty(const int row, const int col);

	inline void addIfIsColor(
		const BoardPosition& from,
		const int row, const int col,
		const bool isWhite, std::list<Move>& moves);

	inline void addIfIsColorOrEmpty(
		const BoardPosition& from, const int row, const int col, const bool isWhite, std::list<Move>& moves);

	template<int rowDir, int colDir>
	void addMovesInDirection(const BoardPosition& from, const bool isWhite, std::list<Move>& moves);

	void getPawnMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves);

	void getRookMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves);

	void getKnightMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves);

	void getBishopMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves);

	void getQueenMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves);

	void getKingMoves(const BoardPosition& pos, const bool isWhite, std::list<Move>& moves);

};

#endif // !CHESS_BOARD_H