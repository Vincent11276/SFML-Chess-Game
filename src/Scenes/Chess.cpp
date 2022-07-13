#include "Chess.hpp"


ChessGame::ChessGame()
	: m_chessPieces_TlMap(sf::Vector2i(8, 8), sf::Vector2f(80, 80))
{
	m_playerTurn = PieceColor::White;
	m_state = State::SelectingPiece;

	Helper::centerSprite(&m_moveableChessPiece_Spr);
}

void ChessGame::init()
{
	static sf::Texture chessBoard_Tex;

	if (!chessBoard_Tex.loadFromFile("../../../Assets/Chess_Artwork/Chess Board/Wood/Chess_Board.png"))
	{
		Logger::getInstance().log(LogLevel::ERROR, "Failed to load chess board texture!");
	}
	m_chessBoard_Spr.setTexture(chessBoard_Tex);
	m_chessBoard_Spr.setOrigin(20, 22);

	static TileSet tileSet;
	if (!tileSet.create(sf::Vector2f(80.0f, 80.0f), 12))
	{
		Logger::getInstance().log(LogLevel::ERROR, "Failed to create tileset!");
	}
	tileSet.loadFromDirectory("../../../Assets/Chess_Artwork/Chess Pieces/Wood");
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

	target.draw(this->m_chessBoard_Spr, states);

	target.draw(m_chessPieces_TlMap, states);
}

// actions

bool ChessGame::selectPiece(const sf::Vector2i& selectedPiece)
{
	if (this->getPieceColor(selectedPiece) == m_playerTurn)
	{
		// important to update the value before quering for valid moves
		m_selectedPiece = selectedPiece;

		// process and cache valid moves for further processing
		this->processValidMoves();

		switch (this->getPieceType(selectedPiece))
		{
		case B_PAWN:
			break;

		case B_ROOK:
			break;

		case B_KNIGHT:
			break;

		case B_BISHOP:
			break;

		case B_QUEEN:
			break;

		case B_KING:
			break;

		case W_PAWN:
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

		default:
			Logger::getInstance().log(LogLevel::ERROR,
				"Unknown piece type enum value received for setting moveable chess piece sprite");
			break;
		}
		//m_moveableChessPiece_Spr.setTexture();

		// play grabbing piece sound

		return true;
	}
	return false;
}

bool ChessGame::moveSelectedPiece(const sf::Vector2i& target)
{
	bool successFlag = false;

	if (this->getPieceColor(m_selectedPiece) == m_playerTurn)
	{
		if (Helper::isElementExists(target, m_validMoves))
		{
			int value = m_chessPieces_TlMap.getCell(m_selectedPiece);

			m_chessPieces_TlMap.setCell(value, target);
			m_chessPieces_TlMap.setCell(-1, m_selectedPiece);

			// play piece touch down sound
			successFlag = true;

			Logger::getInstance().log(LogLevel::INFO,
				std::format("You have moved your piece from [{}, {}] to [{}, {}]",
					m_selectedPiece.x, m_selectedPiece.y, target.x, target.y));
		}
		else
		{
			Logger::getInstance().log(LogLevel::INFO,
				std::format("You cannot move your piece from [{}, {}] to [{}, {}]",
					m_selectedPiece.x, m_selectedPiece.y, target.x, target.y));
		}
	}

	if (successFlag) this->switchPlayerTurn();

	Logger::getInstance().log(LogLevel::INFO,
		this->getPlayerTurnStr() + "'s turn to make a move!");
	
	return successFlag;
}

bool ChessGame::processValidMoves()
{
	m_validMoves.clear();

	switch (this->getPieceType(m_selectedPiece))
	{
	case B_PAWN:
	{
		// check if pawn can move forward 1x
		sf::Vector2i target { m_selectedPiece.x, m_selectedPiece.y + 1 };

		if (not this->isPieceExists(target) 
			&& this->isPieceInBounds(target))
		{
			m_validMoves.push_back(target);
		}

		// check if pawn has ever been moved to move forward 2x
		if (m_selectedPiece.y == 1)
		{
			sf::Vector2i target = { m_selectedPiece.x, m_selectedPiece.y + 2 };

			if (not this->isPieceExists(target)) 
			{
				m_validMoves.push_back(target);
			}
		}

		// check if pawn can take left
		target = m_selectedPiece + sf::Vector2i(-1, 1);

		if (this->isPieceCanTake(m_selectedPiece, target))
		{
			m_validMoves.push_back(target);
		}

		// check if pawn can take right
		target = m_selectedPiece + sf::Vector2i(1, 1);

		if (this->isPieceCanTake(m_selectedPiece, target))
		{
			m_validMoves.push_back(target);
		}
	}
	break;

	case B_ROOK:
	{
		// check for base to top
		for (int y = m_selectedPiece.y - 1; y > 0; y--)
		{
			if (this->isPieceExists({ m_selectedPiece.x, y }))
			{
				m_validMoves.push_back({ m_selectedPiece.x, y });
			}
			else break;
		}

		// check for base to down
		for (int y = m_selectedPiece.y + 1; y < 8; y++)
		{
			if (this->isPieceExists({ m_selectedPiece.x, y }))
			{
				m_validMoves.push_back({ m_selectedPiece.x, y });
			}
			else break;
		}

		// check for base to left
		for (int x = m_selectedPiece.x - 1; x > 0; x--)
		{
			if (this->isPieceExists({ x, m_selectedPiece.y }))
			{
				m_validMoves.push_back({ x, m_selectedPiece.y });
			}
			else break;
		}

		// check for base to right
		for (int x = m_selectedPiece.x + 1; x < 8; x++)
		{
			if (this->isPieceExists({ x, m_selectedPiece.y }))
			{
				m_validMoves.push_back({ x, m_selectedPiece.y });
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
			sf::Vector2i target = m_selectedPiece + offset;

			if (this->isPieceInBounds(target)
				|| this->isPieceCanTake(m_selectedPiece, target))
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
		sf::Vector2i target{ m_selectedPiece.x, m_selectedPiece.y - 1 };

		if (not this->isPieceExists(target)
			&& this->isPieceInBounds(target))
		{
			m_validMoves.push_back(target);
		}

		// check if pawn has ever been moved to move forward 2x
		if (m_selectedPiece.y == 6)
		{
			sf::Vector2i target = { m_selectedPiece.x, m_selectedPiece.y - 2 };

			if (not this->isPieceExists(target))
			{
				m_validMoves.push_back(target);
			}
		}

		// check if pawn can take left
		target = m_selectedPiece + sf::Vector2i(-1, -1);

		if (this->isPieceCanTake(m_selectedPiece, target))
		{
			m_validMoves.push_back(target);
		}

		// check if pawn can take right
		target = m_selectedPiece + sf::Vector2i(1, -1);

		if (this->isPieceCanTake(m_selectedPiece, target))
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

	//switch (m_chessPieces_TlMap.getCell(coords))
	//{
	//case 6:		
	//	return PieceType::B_PAWN;

	//case 10:	
	//	return PieceType::B_ROOK;

	//case 4:		
	//	return PieceType::B_KNIGHT;

	//case 0:		
	//	return PieceType::B_BISHOP;

	//case 2:		
	//	return PieceType::B_QUEEN;

	//case 8:		
	//	return PieceType::B_KING;

	//case 7:		
	//	return PieceType::W_PAWN;

	//case 11:	
	//	return PieceType::W_ROOK;

	//case 5:		
	//	return PieceType::W_KNIGHT;

	//case 1:		
	//	return PieceType::W_BISHOP;

	//case 3:		
	//	return PieceType::W_QUEEN;

	//case 9:		
	//	return PieceType::W_KING;

	//default:	
	//	return PieceType::None;
	//}
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
	return m_selectedPiece;
}

const std::vector<sf::Vector2i>& ChessGame::getValidMoves()
{
	return m_validMoves;
}

ChessGame::PieceColor ChessGame::getPieceColor(const sf::Vector2i& coords)
{
	// if value based on enum is divisible by 2, it is black otherwise white

	if (this->getPieceType(coords) % 2 == 0)
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


