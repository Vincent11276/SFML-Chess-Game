#pragma once

#include <SFML/Graphics.hpp>
#include "../Engine/Piece.hpp"

enum class PieceAction
{
    Move,
    Take,
    Castle,
    EnPessant
};

class PieceMovement
{
    sf::Vector2i target;   

    PieceAction action;
};

class MoveGenerator
{
public:
    MoveGenerator()
    {
        // bool colorFlag = (this->getPieceColor(m_selectedCoords) == PieceColor::Black);

        // PieceType pieceType = this->getPieceType(m_selectedCoords);

        // if (pieceType == B_PAWN || pieceType == W_PAWN)
        // {
            
        // }
        // else if (pieceType == B_KNIGHT || pieceType == W_KNIGHT)
        // {
        //     std::vector<sf::Vector2i> knightValidOffsets
        //     {
        //         { -1, 2 }, { 1, 2 }, { -1, -2 }, { 1, -2 },
        //         { -2, 1 }, { -2, -1 }, { 2, 1 }, { 2, -1 }
        //     };
        //     this->applyValidOffsets(&knightValidOffsets, validMovesPtr);
        // }
        // else if (pieceType == B_ROOK || pieceType == W_ROOK)
        // {
        //     this->generateOrthogonalMoves(validMovesPtr);
        // }
        // else if (pieceType == B_BISHOP || pieceType == W_BISHOP)
        // {
        //     this->generateDiagonalMoves(validMovesPtr);
        // }
        // else if (pieceType == B_QUEEN || pieceType == W_QUEEN)
        // {
        //     this->generateOrthogonalMoves(validMovesPtr);
        //     this->generateDiagonalMoves(validMovesPtr);
        // }
        // else if (pieceType == B_KING || pieceType == W_KING)
        // {
        //     std::vector<sf::Vector2i> kingValidOffsets
        //     {
        //         { -1, -1 }, { 0, -1 }, { 1, -1 }, // top 
        //         { -1, 0 }, { 1, 0 }, // mid
        //         { -1, 1 }, { 0, 1 }, { 1, 1 } // bot
        //     };
        //     this->applyValidOffsets(&kingValidOffsets, validMovesPtr);
        // }
    }

    const std::vector<PieceMovement>& getPawn()
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
};