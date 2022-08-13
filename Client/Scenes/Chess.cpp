#include "Chess.hpp"
#include "Core/Inputs/Mouse.hpp"
#include "SFML/System/Vector2.hpp"
#include <functional>
#include "Game.hpp"


ChessGame::ChessGame()
{
	// default..
}

void ChessGame::init(chess::PieceColor pieceColor)
{
	m_side = pieceColor;
	m_playerTurn = chess::PieceColor::White;

	if (pieceColor == chess::PieceColor::White)
	{
		m_state = State::SelectingPiece;
	}
	else m_state = State::Waiting;

	m_chessBoard.setOrigin(20, 22);
	m_chessPieces.initialize(pieceColor);
	m_moveGenerator.setPiecesToAnalyze(m_chessPieces);

	m_pieceMarker.init();

	Logger::debug("Chess Game has been initialized!");
}

void ChessGame::handleEvents(const sf::Event& e)
{	
	sf::Vector2i coords = this->getMouseHoveringCoords();

	if (m_state == State::SelectingPiece)
	{
		if (this->isPieceCanSelect(coords))
		{
			Game::getInstance()->setCursor(sf::Cursor::Hand);
		}
		else
		{
			Game::getInstance()->setCursor(sf::Cursor::Arrow);
		}
	}
	else if (m_state == State::DraggingPiece)
	{
		m_pieceMarker.setHoveringTile(coords);
		m_moveablePiece.followMouse();
	}
}

void ChessGame::update(float delta)
{
	// amogus
}

void ChessGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(m_chessBoard, states);
	target.draw(m_pieceMarker, states);
	target.draw(m_chessPieces, states);

	if (m_state == State::DraggingPiece)
	{
		target.draw(m_moveablePiece, states);
	}
}

bool ChessGame::isPieceCanSelect(const sf::Vector2i& coords)
{
	// check if selected piece is in bounds of the 8x8 board
	if (!this->isPieceInBounds(coords))
	{
		return false;
	}

	// check if selected "piece" is empty
	if (m_chessPieces.getColor(coords) == chess::PieceColor::Neutral)
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

bool ChessGame::trySelectPiece(const sf::Vector2i& selected)
{
	if (this->isPieceCanSelect(selected))
	{
		// important to update the values before quering for valid moves
		m_selectedPiece = m_chessPieces.getPiece(selected);

		// generate new valid moves for the newly selected piece
		m_moveGenerator.processValidMoves(m_selectedPiece.coords, m_previousMove);

		// mark the selected piece and the newly generated valid moves
		m_pieceMarker.setValidMoves(m_moveGenerator.getAllValidMoves());
		m_pieceMarker.setSelectedPiece(m_selectedPiece.coords); 

		// remove the selected piece from the tilemap and spawn moveable object
		m_chessPieces.removePiece(m_selectedPiece.coords);
		m_moveablePiece.changeType(m_selectedPiece.type);
		m_moveablePiece.followMouse();

		// proceed to the next state
		m_state = State::DraggingPiece;

		Logger::debug("You have selected your piece at [{}, {}]!", 
			m_selectedPiece.coords.x, m_selectedPiece.coords.y);

		return true;
	}
	Logger::debug("You cannot selected your enemy's piece!"); 
	return false;
}

void ChessGame::selectPiece(const sf::Vector2i& selected)
{
	//// important to update the values before quering for valid moves
	//m_selectedPiece = m_chessPieces.getPiece(selected);

	//// generate new valid moves for the newly selected piece
	//m_moveGenerator.processValidMoves(m_selectedPiece.coords, m_previousMove);
}

bool ChessGame::tryMoveSelectedPiece(const sf::Vector2i& target)
{
	// always set the state to selecting regardless if the piece has moved or not
	m_state = State::SelectingPiece;

	if (m_selectedPiece.color == m_playerTurn)
	{
		m_chessPieces.setPiece(m_selectedPiece, m_selectedPiece.coords);
		m_pieceMarker.resetHoveringTile();

		if (m_moveGenerator.isCoordsValidMove(target))
		{
			// find the right piece movement on the generator based on the selected target cords
			chess::PieceMovement targetMove = m_moveGenerator.findValidMoveByCoords(target);
			
			// execute the piecemovement
			m_chessPieces.executeMovement(m_selectedPiece, targetMove);
			
			// process pieces after moving
			this->processAfterMove(targetMove);

			// tell the "controller" that the piece succesfully moved
			return true;
		}
	}
	return false;
}

void ChessGame::moveSelectedPiece(const sf::Vector2i& target)
{

}

void ChessGame::processAfterMove(chess::PieceMovement movement)
{
	// Check if the white king is in check
	if (!m_moveGenerator.isTileFreeFromAttacks(m_chessPieces.getWhiteKingCoords()))
	{
		Logger::info("White king has been checked");

		m_isKingChecked.first = true;
	}

	// Check if the black king is in check
	if (!m_moveGenerator.isTileFreeFromAttacks(m_chessPieces.getBlackKingCoords()))
	{
		Logger::info("Black king has been checked");

		m_isKingChecked.second = true;
	}

	// update piece marker
	m_pieceMarker.setPreviousMove(m_selectedPiece.coords, movement.coords);
	m_pieceMarker.resetValidMoves();

	// store previous selected piece and the move taken
	m_previousMove = std::make_pair(m_selectedPiece, movement);

	// other player's turn to make an action
	this->switchPlayerTurn();
}

void ChessGame::switchPlayerTurn()
{
	if (m_playerTurn == chess::PieceColor::Black)
	{
		m_playerTurn = chess::PieceColor::White;
	}
	else
	{
		m_playerTurn = chess::PieceColor::Black;
	}
}

sf::Vector2i ChessGame::getMouseHoveringCoords()
{
	sf::Vector2i pieceCoords = MouseInput::getRelativePosition();

	pieceCoords.x -= getPosition().x;
	pieceCoords.y -= getPosition().y;

	pieceCoords.x /= (80.f * getScale().x);
	pieceCoords.y /= (80.f * getScale().y);
	
	return pieceCoords;
}

sf::Vector2i ChessGame::getSelectedCoords()
{
	return m_selectedPiece.coords;
}

void ChessGame::flipCoords(sf::Vector2i* coords)
{
	coords->x = 8 - coords->x;
	coords->y = 8 - coords->y;
}

chess::PieceColor ChessGame::getPlayerTurn()
{
	return m_playerTurn;
}
