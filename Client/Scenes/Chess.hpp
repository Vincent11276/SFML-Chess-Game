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
#include "Core/Engine/GameStateManager.hpp"
#include "Core/Chess/PieceType.hpp"
#include "Core/Chess/PieceColor.hpp"
#include "Core/Chess/MoveGenerator.hpp"
#include "Core/Inputs/Mouse.hpp"
#include "Core/Utility/Logger.hpp"
#include "Core/Utility/Helper.hpp"
#include "PieceMarker.hpp"
#include "MoveablePiece.hpp"
#include "PieceMarker.hpp"
#include "ChessBoard.hpp"
#include "ChessPieces.hpp"


typedef std::vector<std::pair<std::string, std::string>> MoveHistory;

class ChessGame : public sf::Drawable, public sf::Transformable
{
public:
	ChessGame();

	void init(chess::PieceColor pieceColor);
	void handleEvents(const sf::Event& e);
	void update([[maybe_unused]] float delta);
	void cleanUp() { };

	bool trySelectPiece(const sf::Vector2i& selectedPiece);
	void selectPiece(const sf::Vector2i& selectedPiece);
	bool tryMoveSelectedPiece(const sf::Vector2i& target);
	void moveSelectedPiece(const sf::Vector2i& target);
	bool isCheckMate() { };
	void switchPlayerTurn();

	sf::Vector2i getMouseHoveringCoords();
	sf::Vector2i getSelectedCoords();
	void flipCoords(sf::Vector2i* coords);

private:
	chess::PieceColor m_side;

	enum State
	{
		Waiting,
		SelectingPiece,
		DraggingPiece
	};
	State m_state;

	bool isPieceCanSelect(const sf::Vector2i& coords);
	bool isPieceInBounds(const sf::Vector2i& coords);

	void processAfterMove(chess::PieceMovement movement);

	chess::PieceColor getPlayerTurn();

	std::pair<chess::Piece, chess::PieceMovement> m_previousMove;

	std::pair<bool, bool>	m_isKingChecked;
	chess::PieceColor		m_playerTurn;
	chess::Piece			m_selectedPiece;
	chess::MoveGenerator	m_moveGenerator;

	ChessBoard		m_chessBoard;
	PieceMarker		m_pieceMarker;
	ChessPieces		m_chessPieces;
	MoveablePiece	m_moveablePiece;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};