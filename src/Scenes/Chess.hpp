#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>

#include "../Engine/TileMap.hpp"
#include "../Inputs/Mouse.hpp"
#include "../Utility/Logger.hpp"
#include "../Utility/Helper.hpp"
#include "../States/GameStateManager.hpp"
#include "../Engine/ResourceManager.hpp"

typedef std::vector<std::pair<std::string, std::string>> MoveHistory;

class ChessGame: public sf::Drawable, public sf::Transformable
{
public:
	enum PieceColor
	{
		Black,
		White
	};

	enum PieceType
	{
		B_PAWN = 6,
		B_ROOK = 10,
		B_KNIGHT = 4,
		B_BISHOP = 0,
		B_QUEEN = 2,
		B_KING = 8,
		W_PAWN = 7,
		W_ROOK = 11,
		W_KNIGHT = 5,
		W_BISHOP = 1,
		W_QUEEN = 3,
		W_KING = 9,

		None = -1
	};

	ChessGame();

	void init();

	void handleEvents(const sf::Event& e);

	void update([[maybe_unused]] float delta);

	void cleanUp();

	bool selectPiece(const sf::Vector2i& selectedPiece);

	bool moveSelectedPiece(const sf::Vector2i& target);

	bool isPieceExists(const sf::Vector2i& coords);

	bool isPieceInBounds(const sf::Vector2i& coords);

	sf::Vector2i getMouseHoveringPiece();

	bool isCheckMate();

	sf::Vector2i getSelectedPiece();

	bool processValidMoves();

	const std::vector<sf::Vector2i>& getValidMoves();


private:
	PieceColor getPieceColor(const sf::Vector2i& coords);

	PieceType getPieceType(const sf::Vector2i coords);

	std::string getPieceTypeStr(const sf::Vector2i coords);

	PieceColor getPlayerTurn();

	std::string getPlayerTurnStr();

	bool isPieceCanTake(const sf::Vector2i& selected, const sf::Vector2i& target);
	void switchPlayerTurn();


	sf::Sprite m_chessBoard_Spr;
	sf::Sprite m_moveableChessPiece_Spr;
	TileMap m_chessPieces_TlMap;
	MoveHistory m_moveHistory;
	PieceColor m_playerTurn;
	sf::Vector2i m_selectedPiece;
	std::vector<sf::Vector2i> m_validMoves;

	bool m_isCheckMate = 0;

	enum State
	{
		SelectingPiece,
		DraggingPiece
	};
	State m_state;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};