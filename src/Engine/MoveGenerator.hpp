#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "../Engine/Piece.hpp"
#include "../Engine/ChessPieces.hpp"
#include "../Utility/Helper.hpp"

enum class PieceAction
{
    None,
    Move,
    Take,
    Castle,
    EnPessant
};

struct PieceMovement
{
    PieceMovement()
    {
        // default..
    }

    PieceMovement(PieceAction p_action, sf::Vector2i p_target)
    {
        action = p_action;
        coords = p_target;
    }

    PieceAction action;
    sf::Vector2i coords;   
};


class MoveGenerator
{
public:
    MoveGenerator()
    {
        // default..
    }

    void setPiecesToAnalyze(const std::shared_ptr<ChessPieces>& pieces)
    {
        this->m_pieces = pieces;
    }

    const std::vector<PieceMovement>& getValidMoves()
    {
        return m_validMoves;
    }

    bool processValidMoves(const sf::Vector2i& selected)
    {
        std::cout << "pls print" << std::endl;
        PieceType pieceType = m_pieces->getType(selected);
        std::cout << "amogus" << std::endl; // not printing

        // if (pieceType == B_PAWN || pieceType == W_PAWN)
        // {
        //     bool colorFlag = (m_pieces->getColor(selected) == PieceColor::Black);

        //     // check if pawn can move ONE tiles forward
        //     sf::Vector2i target { selected.x, selected.y + (colorFlag ? 1 : -1) };

        //     if (not m_pieces->isPieceExists(target) && this->isPieceInBounds(target))
        //     {
        //         m_validMoves.emplace_back(PieceMovement(PieceAction::Move, target));
        //     }

        //     // check if pawn can move TWO tiles forward
        //     if (selected.y == (colorFlag ? 1 : 6 ))
        //     { 
        //         target = { selected.x, selected.y + (colorFlag ? 2 : -2 ) };

        //         if (!m_pieces->isPieceExists(target))
        //         {
        //             m_validMoves.emplace_back(PieceMovement(PieceAction::Move, target));
        //         }
        //     }

        //     // check if pawn can take left
        //     target = selected + sf::Vector2i(-1, (colorFlag ? 1 : -1));

        //     if (this->isPieceCanTake(selected, target))
        //     {
        //         m_validMoves.emplace_back(PieceMovement(PieceAction::Take, target));
        //     }
        //     // check if pawn can take right
        //     target = selected + sf::Vector2i(1, (colorFlag ? 1 : -1));

        //     if (this->isPieceCanTake(selected, target))
        //     {
        //         m_validMoves.emplace_back(PieceMovement(PieceAction::Take, target));
        //     }

        // }
        // else if (pieceType == B_KNIGHT || pieceType == W_KNIGHT)
        // {
        //     std::vector<sf::Vector2i> knightValidOffsets
        //     {
        //         { -1, 2 }, { 1, 2 }, { -1, -2 }, { 1, -2 },
        //         { -2, 1 }, { -2, -1 }, { 2, 1 }, { 2, -1 }
        //     };
        //     this->applyValidOffsets(selected, knightValidOffsets);
        // }
        // else if (pieceType == B_ROOK || pieceType == W_ROOK)
        // {
        //     this->generateOrthogonalMoves(selected);
        // }
        // else if (pieceType == B_BISHOP || pieceType == W_BISHOP)
        // {
        //     this->generateDiagonalMoves(selected);
        // }
        // else if (pieceType == B_QUEEN || pieceType == W_QUEEN)
        // {
        //     this->generateOrthogonalMoves(selected);
        //     this->generateDiagonalMoves(selected);
        // }
        // else if (pieceType == B_KING || pieceType == W_KING)
        // {
        //     std::vector<sf::Vector2i> kingValidOffsets
        //     {
        //         { -1, -1 }, { 0, -1 }, { 1, -1 }, // top 
        //         { -1, 0 }, { 1, 0 }, // mid
        //         { -1, 1 }, { 0, 1 }, { 1, 1 } // bot
        //     };
        //     this->applyValidOffsets(selected, kingValidOffsets);
        // }
        return m_validMoves.size();
    }

    bool isCoordsValidMove(const sf::Vector2i& coords)
    {
        for (const auto& validMove: m_validMoves)
        {
            if (coords == validMove.coords) return true;
        }
        return false;
    }

private:
    std::vector<PieceMovement> m_validMoves;
    std::shared_ptr<ChessPieces> m_pieces;

    bool isPieceInBounds(const sf::Vector2i& coords)
    {
	    return coords.x >= 0 && coords.y >= 0 && coords.x < 8 && coords.y < 8;
    }
    
    bool isPieceCanTake(const sf::Vector2i& selected, const sf::Vector2i& target)
    {
        // taking an empty space is prohibited
        if (!m_pieces->isPieceExists(selected))
        {
            return false;
        }

        // taking its own piece is prohibited
        return m_pieces->getColor(selected) != m_pieces->getColor(target);
    }

    void applyValidOffsets(const sf::Vector2i selected, std::vector<sf::Vector2i>& offsetsPtr)
    {
        for (auto& offset: offsetsPtr)
        {
            sf::Vector2i target = selected + offset;

            if (this->isPieceInBounds(target))
            {
                if (!m_pieces->isPieceExists(target))
                {
                    m_validMoves.emplace_back(PieceMovement(PieceAction::Move, target));
                }
                else if (this->isPieceCanTake(selected, target))
                {
                    m_validMoves.emplace_back(PieceMovement(PieceAction::Take, target));
                }
            }
        }
    }

    void generateDiagonalMoves(const sf::Vector2i& selected)
    {
        auto validateMove = [&](const sf::Vector2i& target)
        {
            if (!m_pieces->isPieceExists(target))
            {
                m_validMoves.emplace_back(PieceMovement(PieceAction::Move, target));
            }
            else
            {
                if (this->isPieceCanTake(selected, target))
                {
                    m_validMoves.emplace_back(PieceMovement(PieceAction::Take, target));
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
            for (sf::Vector2i target = selected + reg.first; reg.second(target); target += reg.first)
            {
                if (validateMove(target)) break;
            }
        }
    }

    void generateOrthogonalMoves(const sf::Vector2i& selected)
    {
        auto validateMove = [&](const sf::Vector2i& target)
        {
            if (!m_pieces->isPieceExists(target))
            {
                m_validMoves.emplace_back(PieceMovement(PieceAction::Move, target));
            }
            else
            {
                if (this->isPieceCanTake(selected, target))
                {
                    m_validMoves.emplace_back(PieceMovement(PieceAction::Take, target));
                }
                return true;
            }
            return false;
        };

        // 	// check for base to top
        for (int y = selected.y - 1; y > 0; y--)
        {
            if (validateMove({ selected.x, y })) break;
        }

        // check for base to down                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
        for (int y = selected.y + 1; y < 8; y++)
        {
            if (validateMove({ selected.x, y })) break;
        }

        // check for base to left
        for (int x = selected.x - 1; x > 0; x--)
        {
            if (validateMove({ x, selected.y })) break;
        }

        // check for base to right
        for (int x = selected.x + 1; x < 8; x++)
        {
            if (validateMove({ x, selected.y })) break;
        }
    }
};