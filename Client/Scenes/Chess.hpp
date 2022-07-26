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

#include "Client/States/GameStateManager.hpp"
#include "Client/Scenes/PieceHighlighter.hpp"
#include "Client/Scenes/MoveablePiece.hpp"


typedef std::vector<std::pair<std::string, std::string>> MoveHistory;

class ChessGame: public sf::Drawable, public sf::Transformable
{
public:
	ChessGame();

	void init();
	void handleEvents(const sf::Event& e);
	void update([[maybe_unused]] float delta);
	void cleanUp();

	bool selectPiece(const sf::Vector2i& selectedPiece);
	bool moveSelectedPiece(const sf::Vector2i& target);

	void processAfterMove();
	bool isCheckMate();

	const std::vector<sf::Vector2i>& getValidMoves();

	sf::Vector2i getMouseHoveringPiece();

	sf::Vector2i getGlobalMousePosition();

private:
	void applyValidOffsets(std::vector<sf::Vector2i>* offsetsPtr, std::vector<sf::Vector2i>* validMovesPtr);
	void generateDiagonalMoves(std::vector<sf::Vector2i>* validMovesPtr);
	void generateOrthogonalMoves(std::vector<sf::Vector2i>* validMovesPtr);

	bool processValidMoves(std::vector<sf::Vector2i>* validMovesPtr);
	bool isPieceExists(const sf::Vector2i& coords);
	bool isPieceInBounds(const sf::Vector2i& coords);

	sf::Vector2i getSelectedPiece();
	PieceColor getPieceColor(const sf::Vector2i& coords);
	PieceColor getPieceColor(const PieceType pieceType);
	PieceType getPieceType(const sf::Vector2i coords);
	std::string getPieceTypeStr(const sf::Vector2i coords);
	PieceColor getPlayerTurn();
	std::string getPlayerTurnStr();

	bool isPieceCanSelect(const sf::Vector2i& coords);
	bool isPieceCanTake(const sf::Vector2i& selected, const sf::Vector2i& target);

	void switchPlayerTurn();

	PieceColor m_playerTurn;
	sf::Vector2i m_selectedCoords;
	Piece m_selectedPiece;
	MoveGenerator m_moveGenerator;
	MoveHistory m_moveHistory;
	// (first = selected, second = target) bad design ye
	std::pair<Piece, PieceMovement> m_previousMove; 
	
	sf::Sprite m_chessBoard_Spr;
	PieceHighlighter m_pieceHighlighter;
	ChessPieces m_chessPieces;
	MoveablePiece m_moveablePiece;


	enum State
	{
		SelectingPiece,
		DraggingPiece
	};
	State m_state;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};