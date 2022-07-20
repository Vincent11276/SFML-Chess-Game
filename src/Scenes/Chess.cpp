#include "Chess.hpp"
#include <functional>


ChessGame::ChessGame()
	: m_chessPieces_TlMap(sf::Vector2i(8, 8), sf::Vector2f(80, 80))
{
	m_playerTurn = PieceColor::White;
	m_state = State::SelectingPiece;
}

void ChessGame::init()
{
	auto& chessBoardTex = ResourceManager::getTexture(ResourceKey::WoodChessBoard);
	m_chessBoard_Spr.setTexture(chessBoardTex);
	m_chessBoard_Spr.setOrigin(20, 22);

	static TileSet tileSet;

	if (!tileSet.create(sf::Vector2f(80.0f, 80.0f), 12))
	{
		Logger::getInstance().log(LogLevel::ERROR, "Failed to create tileset!");
	}

	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceBishopB));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceBishopW));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKingB));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKingW));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKnightB));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKnightW));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPiecePawnB));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPiecePawnW));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceQueenB));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceQueenW));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceRookB));
	tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceRookW));

	m_chessPieces_TlMap.setTileSet(tileSet);

	std::vector<std::vector<int>> chessLayout
	{
		{10, 4, 0, 2, 8, 0, 4, 10},
		{ 6, 6, 6, 6, 6, 6, 6, 6 },
		{-1,-1,-1,-1,-1,-1,-1,-1 },
		{-1,-1,-1,-1,-1,-1,-1,-1 },
		{-1,-1,-1,-1,-1,-1,-1,-1 },
		{-1,-1,-1,-1,-1,-1,-1,-1 },
		{ 7, 7, 7, 7, 7, 7, 7, 7 },
		{11, 5, 1, 3, 9, 1, 5, 11}
	};
	m_chessPieces_TlMap.mapCellsFrom(chessLayout);
	
	Logger::getInstance().log(LogLevel::INFO, "Chess Game Match Started!");

	Logger::getInstance().log(LogLevel::INFO, this->getPlayerTurnStr() + "'s turn to make a move!");

}

void ChessGame::handleEvents(const sf::Event& e)
{	
	switch (m_state)
	{
	case State::SelectingPiece:
	{
		
	}
	break;

	case State::DraggingPiece:
	{
		m_moveablePiece.followMouse();
	}	
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

void ChessGame::cleanUp()
{

}

void ChessGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = this->getTransform();

	target.draw(m_chessBoard_Spr, states);
	target.draw(m_pieceHighlighter, states);
	target.draw(m_chessPieces_TlMap, states);

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

bool ChessGame::selectPiece(const sf::Vector2i& selectedPiece)
{
	// check if selected piece is in bounds of 8x8 board
	if (!this->isPieceInBounds(selectedPiece))
	{
		return false;
	}

	PieceColor pieceColor = this->getPieceColor(selectedPiece);

	// check if selected "piece" is empty
	if (pieceColor == PieceColor::Neutral)
	{
		Logger::getInstance().log(LogLevel::INFO,
			"You cannot selected that empty tile!");

		return false;
	}

	if (pieceColor == m_playerTurn)
	{
		// important to update the values before quering for valid moves
		m_selectedCoords = selectedPiece;
		m_selectedPieceType = this->getPieceType(selectedPiece);

		// delete previously marked valid moves
		for (auto& coords: m_validMoves)
		{
			m_pieceHighlighter.unmark(coords);
		}

		// clear previously set valid moves and generate new one
		m_validMoves.clear();

		this->processValidMoves(&m_validMoves);

		// mark new valid moves after deleting old one
		for (auto& coords: m_validMoves)
		{
			m_pieceHighlighter.markAsCanMove(coords);
		}

		// remove selected piece from tile map and spawn moveable sprite
		m_chessPieces_TlMap.setCell(-1, m_selectedCoords);
		m_moveablePiece.changeType(m_selectedPieceType);
		m_moveablePiece.followMouse();

		// play grabbing piece sound


		// update current state
		m_state = State::DraggingPiece;

		Logger::getInstance().log(LogLevel::DEBUG,
			std::format("You have selected your piece at [{}, {}]!",
				selectedPiece.x, selectedPiece.y));

		return true;
	}
	else
	{
		Logger::getInstance().log(LogLevel::DEBUG,
			"You cannot selected your enemy's piece!");
	}
	return false;
}

bool ChessGame::moveSelectedPiece(const sf::Vector2i& target)
{
	bool successFlag = false;

	if (this->getPieceColor(m_selectedPieceType) == m_playerTurn)
	{
		if (Helper::isElementExists(target, m_validMoves))
		{
			m_chessPieces_TlMap.setCell(m_selectedPieceType, target);
			m_chessPieces_TlMap.setCell(-1, m_selectedCoords);

			// delete previously marked valid moves
			for (auto& coords: m_validMoves)
			{
				m_pieceHighlighter.unmark(coords);
			}

			// play piece touch down sound
			successFlag = true;

			Logger::getInstance().log(LogLevel::DEBUG,
				std::format("You have moved your piece from [{}, {}] to [{}, {}]",
					m_selectedCoords.x, m_selectedCoords.y, target.x, target.y));

			Logger::getInstance().log(LogLevel::DEBUG,
				this->getPlayerTurnStr() + "'s turn to make a move!");
		}
		else
		{
			// reset selected piece back
			m_chessPieces_TlMap.setCell(m_selectedPieceType, m_selectedCoords);

			Logger::getInstance().log(LogLevel::DEBUG,
				std::format("You cannot move your piece from [{}, {}] to [{}, {}]",
					m_selectedCoords.x, m_selectedCoords.y, target.x, target.y));
		}
	}
	// update state
	m_state = State::SelectingPiece;
	
	if (successFlag) 
	{
		this->switchPlayerTurn();
	}	
	return successFlag;
}

void ChessGame::generateDiagonalMoves(std::vector<sf::Vector2i>* validMovesPtr)
{
	auto validateMove = [&](const sf::Vector2i& target)
	{
		if (!this->isPieceExists(target))
		{
			validMovesPtr->push_back(target);
		}
		else
		{
			if (this->isPieceCanTake(m_selectedCoords, target))
			{
				validMovesPtr->push_back(target); 
			}
			return true;
		}
		return false;
	};

	std::vector<std::pair<sf::Vector2i, std::function<bool(sf::Vector2i target)>>> registers
	{
		{ sf::Vector2i(-1, -1), [](const sf::Vector2i& target) { return target.x >= 0 && target.y >= 0; } },
		{ sf::Vector2i(1, -1), [](const sf::Vector2i& target) { return target.x < 7 && target.y >= 0; } },
		{ sf::Vector2i(-1, 1), [](const sf::Vector2i& target) { return target.x >= 0 && target.y < 7; } },
		{ sf::Vector2i(1, 1), [](const sf::Vector2i& target) { return target.x < 7 && target.y < 7; } },
	};

	for (auto& reg: registers)
	{
		for (sf::Vector2i target = m_selectedCoords + reg.first; reg.second(target); target += reg.first)
		{
			if (validateMove(target)) break;
		}
	}
}


void ChessGame::generateOrthogonalMoves(std::vector<sf::Vector2i>* validMovesPtr)
{
	auto validateMove = [&](const sf::Vector2i& target)
	{
		if (!this->isPieceExists(target))
		{
			validMovesPtr->push_back(target);
		}
		else
		{
			if (this->isPieceCanTake(m_selectedCoords, target))
			{
				validMovesPtr->push_back(target); 
			}
			return true;
		}
		return false;
	};

	// 	// check for base to top
	for (int y = m_selectedCoords.y - 1; y > 0; y--)
	{
		if (validateMove({m_selectedCoords.x, y })) break;
	}

	// check for base to down                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	for (int y = m_selectedCoords.y + 1; y < 8; y++)
	{
		if (validateMove({ m_selectedCoords.x, y })) break;
	}

	// check for base to left
	for (int x = m_selectedCoords.x - 1; x > 0; x--)
	{
		if (validateMove({ x, m_selectedCoords.y })) break;
	}

	// check for base to right
	for (int x = m_selectedCoords.x + 1; x < 8; x++)
	{
		if (validateMove({ x, m_selectedCoords.y })) break;
	}
}

void ChessGame::applyValidOffsets(std::vector<sf::Vector2i>* offsetsPtr, std::vector<sf::Vector2i>* validMovesPtr)
{
	for (auto& offset: *offsetsPtr)
	{
		sf::Vector2i target = m_selectedCoords + offset;

		if (this->isPieceInBounds(target))
		{
			if (this->isPieceCanTake(m_selectedCoords, target)
				|| !this->isPieceExists(target))
			{
				validMovesPtr->emplace_back(target);
			}
		}
	}
}

bool ChessGame::processValidMoves(std::vector<sf::Vector2i>* validMovesPtr)
{
	bool colorFlag = (this->getPieceColor(m_selectedCoords) == PieceColor::Black);

	PieceType pieceType = this->getPieceType(m_selectedCoords);

	if (pieceType == B_PAWN || pieceType == W_PAWN)
	{
		// check if pawn can move forward 1x
		sf::Vector2i target { m_selectedCoords.x, m_selectedCoords.y + (colorFlag ? 1 : -1) };

		if (not this->isPieceExists(target) && this->isPieceInBounds(target))
		{
			validMovesPtr->emplace_back(target);
		}

		// check if pawn has ever been moved
		if (m_selectedCoords.y == (colorFlag ? 1 : 6 ))
		{ 
			target = { m_selectedCoords.x, m_selectedCoords.y + (colorFlag ? 2 : -2 ) };

			if (!this->isPieceExists(target))
			{
				validMovesPtr->emplace_back(target);
				m_pieceHighlighter.markAsCanMove(target);
			}
		}

		// check if pawn can take left
		target = m_selectedCoords + sf::Vector2i(-1, (colorFlag ? 1 : -1));

		if (this->isPieceCanTake(m_selectedCoords, target))
		{
			validMovesPtr->emplace_back(target);
			m_pieceHighlighter.markAsCanTake(target);
		}

		// check if pawn can take right
		target = m_selectedCoords + sf::Vector2i(1, (colorFlag ? 1 : -1));

		if (this->isPieceCanTake(m_selectedCoords, target))
		{
			validMovesPtr->emplace_back(target);
			m_pieceHighlighter.markAsCanTake(target);
		}
	}
	else if (pieceType == B_KNIGHT || pieceType == W_KNIGHT)
	{
		std::vector<sf::Vector2i> knightValidOffsets
		{
			{ -1, 2 }, { 1, 2 }, { -1, -2 }, { 1, -2 },
			{ -2, 1 }, { -2, -1 }, { 2, 1 }, { 2, -1 }
		};
		this->applyValidOffsets(&knightValidOffsets, validMovesPtr);
	}
	else if (pieceType == B_ROOK || pieceType == W_ROOK)
	{
		this->generateOrthogonalMoves(validMovesPtr);
	}
	else if (pieceType == B_BISHOP || pieceType == W_BISHOP)
	{
		this->generateDiagonalMoves(validMovesPtr);
	}
	else if (pieceType == B_QUEEN || pieceType == W_QUEEN)
	{
		this->generateOrthogonalMoves(validMovesPtr);
		this->generateDiagonalMoves(validMovesPtr);
	}
	else if (pieceType == B_KING || pieceType == W_KING)
	{
		std::vector<sf::Vector2i> kingValidOffsets
		{
			{ -1, -1 }, { 0, -1 }, { 1, -1 }, // top 
			{ -1, 0 }, { 1, 0 }, // mid
			{ -1, 1 }, { 0, 1 }, { 1, 1 } // bot
		};
		this->applyValidOffsets(&kingValidOffsets, validMovesPtr);

		// // check for castling
		// PieceColor pieceColor = this->getPieceColor(pieceType);

		// if (pieceColor == PieceColor::Black && m_isBlackKingMoved)
		// {
		// 	// check for base to left
		// 	for (int x = m_selectedCoords.x - 1; x > 0; x--)
		// 	{
		// 		sf::Vector2i target = { x, m_selectedCoords.y };

		// 		if (this->isPieceExists(target))
		// 		{
		// 			if (this->getPieceType(target) == PieceType::W_ROOK)
		// 			{
						
		// 			}
		// 		}
		// 		else break;
		// 	}

		// 	// check for base to right
		// 	for (int x = m_selectedCoords.x + 1; x < 8; x++)
		// 	{
		// 		if (validateMove({ x, m_selectedCoords.y })) break;
		// 	}
		// }
		// else if (pieceColor == PieceColor::White && m_isWhiteKingMoved)
		// {

		// }
	}
	return validMovesPtr->size();
}

bool ChessGame::processAfterMove() 
{
	
}

PieceType ChessGame::getPieceType(const sf::Vector2i coords)
{
	return PieceType(m_chessPieces_TlMap.getCell(coords));
}

bool ChessGame::isPieceExists(const sf::Vector2i& coords)
{
	// simply checks if there is a piece in that specific coords
	return (m_chessPieces_TlMap.getCell(coords) != -1);
}

bool ChessGame::isCheckMate()
{
	return m_isCheckMate;
}

bool ChessGame::isPieceInBounds(const sf::Vector2i & location)
{
	return location.x >= 0 && location.y >= 0 && location.x < 8 && location.y < 8;
}

bool ChessGame::isPieceCanTake(const sf::Vector2i& selected, const sf::Vector2i& target)
{
	// taking an empty space is prohibited
	if (m_chessPieces_TlMap.getCell(target) == -1) return false;
	
	// taking its own piece is prohibited
	return this->getPieceColor(selected) != this->getPieceColor(target);
}

sf::Vector2i ChessGame::getMouseHoveringPiece()
{
	return m_chessPieces_TlMap.worldToMap(MouseInput::getRelativePosition());
}

sf::Vector2i ChessGame::getSelectedPiece()
{
	return m_selectedCoords;
}

const std::vector<sf::Vector2i>& ChessGame::getValidMoves()
{
	return m_validMoves;
}

PieceColor ChessGame::getPieceColor(const sf::Vector2i& coords)
{
	PieceType pieceType = this->getPieceType(coords);

	// check if it's empty
	if (pieceType == PieceType::None)
	{
		return PieceColor::Neutral;
	}

	// if value based on enum is divisible by 2, it is black otherwise white
	if (pieceType % 2 == 0)
	{
		return PieceColor::Black;
	}
	else
	{
		return PieceColor::White;
	}
}

PieceColor ChessGame::getPieceColor(const PieceType pieceType)
{
	if (pieceType % 2 == 0)
	{
		return PieceColor::Black;
	}
	else
	{
		return PieceColor::White;
	}
}

PieceColor ChessGame::getPlayerTurn()
{
	return m_playerTurn;
}

std::string ChessGame::getPlayerTurnStr()
{
	switch (this->getPlayerTurn())
	{
	case PieceColor::Black:
		return "Black";

	case PieceColor::White:
		return "White";

	case PieceColor::Neutral:
		return "Neutral";
	};
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
