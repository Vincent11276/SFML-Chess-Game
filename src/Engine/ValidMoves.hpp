#pragma once

#include "../Engine/TileMap.hpp"


class ValidMoveGetter
{
public:
    ValidMoveGetter() = delete;

    static bool getPawn(const sf::Vector2i& selected, const TileMap& pieces, std::vector<sf::Vector2i>* validMovesPtr)
    {
        // validMovesPtr

        // // check if pawn can move forward 1x
		// sf::Vector2i target { m_selectedPieceCoords.x, m_selectedPieceCoords.y + 1 };

		// if (not this->isPieceExists(target) 
		// 	&& this->isPieceInBounds(target))
		// {
		// 	validMovesPtr->push_back(target);
		// }

		// // check if pawn has ever been moved to move forward 2x
		// if (m_selectedPieceCoords.y == 1)
		// {
		// 	sf::Vector2i target = { m_selectedPieceCoords.x, m_selectedPieceCoords.y + 2 };

		// 	if (not this->isPieceExists(target)) 
		// 	{
		// 		m_validMoves.push_back(target);
		// 	}
		// }

		// // check if pawn can take left
		// target = m_selectedPieceCoords + sf::Vector2i(-1, 1);

		// if (this->isPieceCanTake(m_selectedPieceCoords, target))
		// {
		// 	m_validMoves.push_back(target);
		// }

		// // check if pawn can take right
		// target = m_selectedPieceCoords + sf::Vector2i(1, 1);

		// if (this->isPieceCanTake(m_selectedPieceCoords, target))
		// {
		// 	m_validMoves.push_back(target);
		// }
        return true;
    }

    static bool getBishop();

    static bool getRook();

    static bool getKnight();

    static bool getQueen();

    static bool getKing();
};