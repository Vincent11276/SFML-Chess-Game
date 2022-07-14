#include "Chess.hpp"


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
		auto mouse = MouseInput::getRelativePosition();
		m_moveableChessPiece_Spr.setPosition(mouse.x, mouse.y);
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
	target.draw(m_chessPieces_TlMap, states);

	switch (m_state)
	{
	case State::SelectingPiece:
		break;

	case State::DraggingPiece:
		target.draw(m_moveableChessPiece_Spr, states);
		break;
	}
}

// actions

bool ChessGame::selectPiece(const sf::Vector2i& selectedPiece)
{
	PieceColor pieceColor = this->getPieceColor(selectedPiece);

	if (pieceColor == PieceColor::Neutral)
	{
		Logger::getInstance().log(LogLevel::INFO,
			"You cannot selected that empty tile!");

		return false;
	}

	if (pieceColor == m_playerTurn)
	{
		// important to update the values before quering for valid moves
		m_selectedPieceCoords = selectedPiece;
		m_selectedPieceType = this->getPieceType(selectedPiece);

		// process and cache valid moves for further processing
		this->processValidMoves();

		// change the texture of moveable piece to what the player selected
		this->updateMoveablePiece(this->getPieceType(selectedPiece));

		// remove selected piece from tile map and spawn moveable sprite
		m_chessPieces_TlMap.setCell(-1, m_selectedPieceCoords);

		// play grabbing piece sound

		// update current state
		m_state = State::DraggingPiece;

		Logger::getInstance().log(LogLevel::INFO,
			std::format("You have selected your piece at [{}, {}]!",
				selectedPiece.x, selectedPiece.y));

		return true;
	}
	else
	{
		Logger::getInstance().log(LogLevel::INFO,
			"You cannot selected your enemy's piece!");

		return false;
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
			m_chessPieces_TlMap.setCell(-1, m_selectedPieceCoords);

			// play piece touch down sound
			successFlag = true;

			Logger::getInstance().log(LogLevel::INFO,
				std::format("You have moved your piece from [{}, {}] to [{}, {}]",
					m_selectedPieceCoords.x, m_selectedPieceCoords.y, target.x, target.y));

			Logger::getInstance().log(LogLevel::INFO,
				this->getPlayerTurnStr() + "'s turn to make a move!");
		}
		else
		{
			// reset selected piece back
			m_chessPieces_TlMap.setCell(m_selectedPieceType, m_selectedPieceCoords);

			Logger::getInstance().log(LogLevel::INFO,
				std::format("You cannot move your piece from [{}, {}] to [{}, {}]",
					m_selectedPieceCoords.x, m_selectedPieceCoords.y, target.x, target.y));
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

bool ChessGame::processValidMoves()
{
	m_validMoves.clear();

	switch (this->getPieceType(m_selectedPieceCoords))
	{
	case B_PAWN:
	{
		// check if pawn can move forward 1x
		sf::Vector2i target { m_selectedPieceCoords.x, m_selectedPieceCoords.y + 1 };

		if (not this->isPieceExists(target) 
			&& this->isPieceInBounds(target))
		{
			m_validMoves.push_back(target);
		}

		// check if pawn has ever been moved to move forward 2x
		if (m_selectedPieceCoords.y == 1)
		{
			sf::Vector2i target = { m_selectedPieceCoords.x, m_selectedPieceCoords.y + 2 };

			if (not this->isPieceExists(target)) 
			{
				m_validMoves.push_back(target);
			}
		}

		// check if pawn can take left
		target = m_selectedPieceCoords + sf::Vector2i(-1, 1);

		if (this->isPieceCanTake(m_selectedPieceCoords, target))
		{
			m_validMoves.push_back(target);
		}

		// check if pawn can take right
		target = m_selectedPieceCoords + sf::Vector2i(1, 1);

		if (this->isPieceCanTake(m_selectedPieceCoords, target))
		{
			m_validMoves.push_back(target);
		}
	}
	break;

	case B_ROOK:
	{
		// check for base to top
		for (int y = m_selectedPieceCoords.y - 1; y > 0; y--)
		{
			if (this->isPieceExists({ m_selectedPieceCoords.x, y }))
			{
				m_validMoves.push_back({ m_selectedPieceCoords.x, y });
			}
			else break;
		}

		// check for base to down
		for (int y = m_selectedPieceCoords.y + 1; y < 8; y++)
		{
			if (this->isPieceExists({ m_selectedPieceCoords.x, y }))
			{
				m_validMoves.push_back({ m_selectedPieceCoords.x, y });
			}
			else break;
		}

		// check for base to left
		for (int x = m_selectedPieceCoords.x - 1; x > 0; x--)
		{
			if (this->isPieceExists({ x, m_selectedPieceCoords.y }))
			{
				m_validMoves.push_back({ x, m_selectedPieceCoords.y });
			}
			else break;
		}

		// check for base to right
		for (int x = m_selectedPieceCoords.x + 1; x < 8; x++)
		{
			if (this->isPieceExists({ x, m_selectedPieceCoords.y }))
			{
				m_validMoves.push_back({ x, m_selectedPieceCoords.y });
			}
			else break;
		}
	}
	break;

	case B_KNIGHT:
	{
		static std::vector<sf::Vector2i> knightValidOffsets
		{
			{ -1, 2 }, { 1, 2 },
			{ -1, -2 }, { 1, -2 },
			{ -2, 1 }, { -2, -1 },
			{ 2, 1 }, { 2, -1 }
		};

		for (auto& offset : knightValidOffsets)
		{
			sf::Vector2i target = m_selectedPieceCoords + offset;

			if (this->isPieceInBounds(target)
				|| this->isPieceCanTake(m_selectedPieceCoords, target))
			{
				m_validMoves.push_back(target);
			}
		}
	}
	break;

	case B_BISHOP:
		break;

	case B_QUEEN:
		break;

	case B_KING:
		break;

	case W_PAWN:
	{
		// check if pawn can move forward 1x
		sf::Vector2i target{ m_selectedPieceCoords.x, m_selectedPieceCoords.y - 1 };

		if (not this->isPieceExists(target)
			&& this->isPieceInBounds(target))
		{
			m_validMoves.push_back(target);
		}

		// check if pawn has ever been moved to move forward 2x
		if (m_selectedPieceCoords.y == 6)
		{
			sf::Vector2i target = { m_selectedPieceCoords.x, m_selectedPieceCoords.y - 2 };

			if (not this->isPieceExists(target))
			{
				m_validMoves.push_back(target);
			}
		}

		// check if pawn can take left
		target = m_selectedPieceCoords + sf::Vector2i(-1, -1);

		if (this->isPieceCanTake(m_selectedPieceCoords, target))
		{
			m_validMoves.push_back(target);
		}

		// check if pawn can take right
		target = m_selectedPieceCoords + sf::Vector2i(1, -1);

		if (this->isPieceCanTake(m_selectedPieceCoords, target))
		{
			m_validMoves.push_back(target);
		}
	}
	break;

	case W_ROOK:
		break;

	case W_KNIGHT:
		break;

	case W_BISHOP:
		break;

	case W_QUEEN:
		break;

	case W_KING:
		break;

	case None:
		return false;

	}
	return m_validMoves.size();
}

ChessGame::PieceType ChessGame::getPieceType(const sf::Vector2i coords)
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
	return m_selectedPieceCoords;
}

const std::vector<sf::Vector2i>& ChessGame::getValidMoves()
{
	return m_validMoves;
}

ChessGame::PieceColor ChessGame::getPieceColor(const sf::Vector2i& coords)
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

ChessGame::PieceColor ChessGame::getPieceColor(const PieceType pieceType)
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


ChessGame::PieceColor ChessGame::getPlayerTurn()
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

void ChessGame::updateMoveablePiece(PieceType pieceType)
{
	switch (pieceType)
	{
	case B_PAWN:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPiecePawnB));
		break;

	case B_ROOK:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceRookB));
		break;

	case B_KNIGHT:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceKnightB));
		break;

	case B_BISHOP:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceBishopB));
		break;

	case B_QUEEN:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceQueenB));
		break;

	case B_KING:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceKingB));
		break;

	case W_PAWN:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPiecePawnW));
		
		break;

	case W_ROOK:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceRookW));
		break;

	case W_KNIGHT:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceKnightW));
		break;

	case W_BISHOP:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceBishopW));
		break;

	case W_QUEEN:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceQueenW));
		break;

	case W_KING:
		m_moveableChessPiece_Spr.setTexture(
			ResourceManager::getTexture(ResourceKey::WoodPieceKingW));
		break;

	default:
		Logger::getInstance().log(LogLevel::DEBUG,
			"Unknown piece type enum value received for setting moveable chess piece sprite");
		break;
	}
	auto mouse = MouseInput::getRelativePosition();
	m_moveableChessPiece_Spr.setPosition(mouse.x, mouse.y);
	Helper::centerSprite(&m_moveableChessPiece_Spr);
}
