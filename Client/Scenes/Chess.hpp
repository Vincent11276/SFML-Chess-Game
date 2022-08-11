#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <memory>

#include "Core/Engine/TileMap.hpp"
#include "Core/Engine/ResourceManager.hpp"

#include "Core/Chess/PieceType.hpp"
#include "Core/Chess/PieceColor.hpp"
#include "Core/Chess/MoveGenerator.hpp"
#include "Core/Chess/ChessPieces.hpp"

#include "Core/Inputs/Mouse.hpp"
#include "Core/Utility/Logger.hpp"
#include "Core/Utility/Helper.hpp"

#include "Core/Engine/GameStateManager.hpp"
#include "Client/Scenes/PieceHighlighter.hpp"
#include "Client/Scenes/MoveablePiece.hpp"


typedef std::vector<std::pair<std::string, std::string>> MoveHistory;

class ChessGame: public sf::Drawable, public sf::Transformable
{
public:
	ChessGame();

	void init(PieceColor pieceColor);
	void handleEvents(const sf::Event& e);
	void update([[maybe_unused]] float delta);
	void cleanUp() { };

	bool trySelectPiece(const sf::Vector2i& selectedPiece);
	void selectPiece(const sf::Vector2i& selectedPiece);
	bool tryMoveSelectedPiece(const sf::Vector2i& target);
	void moveSelectedPiece(const sf::Vector2i& target);
	bool isCheckMate() { };

	sf::Vector2i getMouseHoveringCoords();
	sf::Vector2i getSelectedCoords();
	void flipCoords(sf::Vector2i* coords);

private:
	PieceColor m_side;

	enum State
	{
		Waiting,
		SelectingPiece,
		DraggingPiece
	};
	State m_state;

	bool isPieceCanSelect(const sf::Vector2i& coords);
	bool isPieceInBounds(const sf::Vector2i& coords);

	void switchPlayerTurn();
	void processAfterMove();

	PieceColor getPlayerTurn();

	Piece m_selectedPiece;
	PieceColor m_playerTurn;
	MoveGenerator m_moveGenerator;
	MoveHistory m_moveHistory;

	std::pair<Piece, PieceMovement> m_previousMove; 
	sf::Sprite m_chessBoard_Spr;
	PieceHighlighter m_pieceHighlighter;
	ChessPieces m_chessPieces;
	MoveablePiece m_moveablePiece;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};