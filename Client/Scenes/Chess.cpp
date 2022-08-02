#include "Chess.hpp"
#include "Core/Inputs/Mouse.hpp"
#include "SFML/System/Vector2.hpp"

#include <functional>


ChessGame::ChessGame()
{
	m_playerTurn = PieceColor::White;
	m_state = State::SelectingPiece;
}

void ChessGame::init()
{
	auto& chessBoardTex = ResourceManager::getTexture(ResourceKey::WoodChessBoard);
	m_chessBoard_Spr.setTexture(chessBoardTex);
	m_chessBoard_Spr.setOrigin(20, 22);
	
	m_chessPieces.initialize(PieceColor::White);

	m_moveGenerator.setPiecesToAnalyze(m_chessPieces);

	Logger::getInstance().log(LogLevel::INFO, "Chess Game Match Started!");
}

void ChessGame::handleEvents(const sf::Event& e)
{	
	switch (m_state)
	{
	case State::SelectingPiece:
		break;

	case State::DraggingPiece:
		m_moveablePiece.followMouse();
		break;
	}
}

void ChessGame::update(float delta)
{
	switch (m_state)
	{
	case State::SelectingPiece:
		break;

	case State::DraggingPiece:
		break;
	}
}

void ChessGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(m_chessBoard_Spr, states);
	target.draw(m_pieceHighlighter, states);
	target.draw(m_chessPieces, states);

	switch (m_state)
	{
	case State::SelectingPiece:
		break;

	case State::DraggingPiece:
		target.draw(m_moveablePiece, states);
		break;
	}
}

// actions

bool ChessGame::isPieceCanSelect(const sf::Vector2i& coords)
{
	// check if selected piece is in bounds of the 8x8 board
	if (!this->isPieceInBounds(coords))
	{
		return false;
	}

	// check if selected "piece" is empty
	if (m_chessPieces.getColor(coords) == PieceColor::Neutral)
	{
		return false;
	}

	// check if selected piece is theirs
	if (m_chessPieces.getColor(coords) != m_playerTurn)
	{
		return false;
	}
	return true;
}

bool ChessGame::isPieceInBounds(const sf::Vector2i& coords)
{
	return coords.x >= 0 && coords.y >= 0 && coords.x < 8 && coords.y < 8;
}


bool ChessGame::selectPiece(const sf::Vector2i& selected)
{
	if (this->isPieceCanSelect(selected))
	{
		// delete previously marked valid moves
		m_pieceHighlighter.unmarkValidMoves(m_moveGenerator.getAllValidMoves());
		m_pieceHighlighter.remove(m_selectedPiece.coords);

		// important to update the values before quering for valid moves
		m_selectedPiece = m_chessPieces.getPiece(selected);

		// generate new valid moves for the newly selected piece
		m_moveGenerator.processValidMoves(m_selectedPiece.coords, m_previousMove);

		// mark new valid moves after deleting old one and new moves are generated
		m_pieceHighlighter.markValidMoves(m_moveGenerator.getAllValidMoves());
		m_pieceHighlighter.markAsHighlighted(m_selectedPiece.coords);
		
		// highlight the previous move back as it has been deleted
		m_pieceHighlighter.markAsHighlighted(m_previousMove.first.coords);
		m_pieceHighlighter.markAsHighlighted(m_previousMove.second.coords);

		// remove selected piece from tile map
		m_chessPieces.removePiece(m_selectedPiece.coords);

		// spawn moveable sprite that follows mouse position in real time
		m_moveablePiece.changeType(m_selectedPiece.type);
		m_moveablePiece.followMouse();

		// play grabbing piece sound
		// amogus

		// update current state
		m_state = State::DraggingPiece;

		Logger::getInstance().log(LogLevel::DEBUG,
			std::format("You have selected your piece at [{}, {}]!",
				m_selectedPiece.coords.x, m_selectedPiece.coords.y));

		return true;
	}

	Logger::getInstance().log(LogLevel::DEBUG, 
		"You cannot selected your enemy's piece!");
	
	return false;
}

bool ChessGame::moveSelectedPiece(const sf::Vector2i& target)
{
	// always set the state to selecting regardless if the piece has moved or not
	m_state = State::SelectingPiece;

	if (m_selectedPiece.color == m_playerTurn)
	{
		m_chessPieces.setPiece(m_selectedPiece, m_selectedPiece.coords);

		if (m_moveGenerator.isCoordsValidMove(target))
		{
			// unhighlight the previous move
			m_pieceHighlighter.unmarkValidMoves(m_moveGenerator.getAllValidMoves());
			m_pieceHighlighter.remove(m_previousMove.first.coords);
			m_pieceHighlighter.remove(m_previousMove.second.coords);

			// find the right piece movement on the generator based on the selected target param
			PieceMovement targetMove = m_moveGenerator.findValidMoveByCoords(target);
			
			// store previous selected piece and the move taken
			m_previousMove = std::make_pair(m_selectedPiece, targetMove);
			
			// execute the piecemovement
			m_chessPieces.executeMovement(m_selectedPiece, targetMove);
			
			// highlight the new recent move after deleting the old one
			m_pieceHighlighter.markAsHighlighted(m_previousMove.first.coords);
			m_pieceHighlighter.markAsHighlighted(m_previousMove.second.coords);
			
			// process pieces after action
			this->processAfterMove();

			// tell the controller that the piece succesfully moved
			return true;
		}
	}
	return false;
}

void ChessGame::processAfterMove() 
{
	// Check if king is checked

	// Check if it is checkmate

	// other player's turn to make an action
	this->switchPlayerTurn();
}

void ChessGame::switchPlayerTurn()
{
	if (m_playerTurn == PieceColor::Black)
	{
		m_playerTurn = PieceColor::White;
	}
	else
	{
		m_playerTurn = PieceColor::Black;
	}
}

sf::Vector2i ChessGame::getMouseHoveringPiece()
{
	sf::Vector2i pieceCoords = MouseInput::getRelativePosition();

	pieceCoords.x -= getPosition().x;
	pieceCoords.y -= getPosition().y;

	pieceCoords.x /= (80.f * getScale().x);
	pieceCoords.y /= (80.f * getScale().y);
	
	return pieceCoords;
}

sf::Vector2i ChessGame::getSelectedPiece()
{
	return m_selectedPiece.coords;
}

PieceColor ChessGame::getPlayerTurn()
{
	return m_playerTurn;
}