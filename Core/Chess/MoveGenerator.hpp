#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <functional>

#include "Core/Chess/Piece.hpp"
#include "Core/Chess/ChessPieces.hpp"
#include "Core/Utility/Helper.hpp"
#include "Core/Chess/PieceAction.hpp"
#include "Core/Chess/PieceMovement.hpp"

namespace chess
{
    class MoveGenerator
    {
    public:
        MoveGenerator()
        {
            // default..
        }

        void setPiecesToAnalyze(ChessPieces& chessPieces)
        {
            m_pieces = &chessPieces;
        }

        PieceMovement findValidMoveByCoords(const sf::Vector2i& coords)
        {
            for (auto& validMove : m_validMoves)
            {
                if (validMove.coords == coords)
                {
                    return validMove;
                }
            }
            return PieceMovement();
        }

        const std::vector<PieceMovement>& getAllValidMoves()
        {
            return m_validMoves;
        }

        void processEnPessantViability(const Piece& selectedPiece, const std::pair<Piece, PieceMovement>& previousMove, const sf::Vector2i& target)
        {
            // check if there is no piece on the target tile
            if (m_pieces->getType(target) != PieceType::None)
            {
                return;
            }

            // if (previousMove.first.type == PieceType::B_PAWN || previousMove.first.type == PieceType::W_PAWN)

            // check if the piece is a pawn and it has moved two squares forwards
            if (previousMove.second.action != PieceAction::TwoSquaresForward)
            {
                return;
            }

            if (selectedPiece.color == PieceColor::White)
            {
                if (target + sf::Vector2i{ 0, 1 } == previousMove.second.coords)
                {
                    m_validMoves.emplace_back(PieceMovement(PieceAction::EnPessantUp, target));
                }
            }
            else
            {
                if (target + sf::Vector2i{ 0, -1 } == previousMove.second.coords)
                {
                    m_validMoves.emplace_back(PieceMovement(PieceAction::EnPessantDown, target));
                }
            }
        }

        bool processValidMoves(const sf::Vector2i& selected, const std::pair<Piece, PieceMovement>& previousMove = std::pair<Piece, PieceMovement>())
        {
            if (m_pieces == nullptr)
            {
                std::cout << "You did not specify which pieces to analyze. Please call MoveGenerator::setPiecesToAnalyze() first!" << std::endl;

                return false;
            }
            else m_validMoves.clear();

            Piece selectedPiece = m_pieces->getPiece(selected);

            if (selectedPiece.type == B_PAWN || selectedPiece.type == W_PAWN)
            {
                bool colorFlag = !(m_pieces->getColor(selected) == m_pieces->getLayoutColor());

                // check if pawn can move ONE tiles forward
                sf::Vector2i target{ selected.x, selected.y + (colorFlag ? 1 : -1) };

                if (!m_pieces->isPieceExists(target) && this->isCoordsInBounds(target))
                {
                    m_validMoves.emplace_back(PieceMovement(PieceAction::Relocate, target));

                    // check if pawn can move TWO tiles forward
                    if (!selectedPiece.isEverMoved)
                    {
                        target = { selected.x, selected.y + (colorFlag ? 2 : -2) };

                        if (!m_pieces->isPieceExists(target))
                        {
                            m_validMoves.emplace_back(PieceMovement(PieceAction::TwoSquaresForward, target));
                        }
                    }
                }

                // check if pawn can take left
                target = selected + sf::Vector2i(-1, (colorFlag ? 1 : -1));

                if (this->isCoordsInBounds(target))
                {
                    if (this->isPieceCanTake(selected, target))
                    {
                        m_validMoves.emplace_back(PieceMovement(PieceAction::Capture, target));
                    }
                    this->processEnPessantViability(selectedPiece, previousMove, target);
                }

                // check if pawn can take right
                target = selected + sf::Vector2i(1, (colorFlag ? 1 : -1));

                if (this->isCoordsInBounds(target))
                {
                    if (this->isPieceCanTake(selected, target))
                    {
                        m_validMoves.emplace_back(PieceMovement(PieceAction::Capture, target));
                    }
                    this->processEnPessantViability(selectedPiece, previousMove, target);
                }
            }
            else if (selectedPiece.type == B_KNIGHT || selectedPiece.type == W_KNIGHT)
            {
                std::vector<sf::Vector2i> knightValidOffsets
                {
                    { -1, 2 }, { 1, 2 }, { -1, -2 }, { 1, -2 },
                    { -2, 1 }, { -2, -1 }, { 2, 1 }, { 2, -1 }
                };
                this->applyValidOffsets(selected, knightValidOffsets);
            }
            else if (selectedPiece.type == B_ROOK || selectedPiece.type == W_ROOK)
            {
                this->generateOrthogonalMoves(selected);
            }
            else if (selectedPiece.type == B_BISHOP || selectedPiece.type == W_BISHOP)
            {
                this->generateDiagonalMoves(selected);
            }
            else if (selectedPiece.type == B_QUEEN || selectedPiece.type == W_QUEEN)
            {
                this->generateOrthogonalMoves(selected);
                this->generateDiagonalMoves(selected);
            }
            else if (selectedPiece.type == B_KING || selectedPiece.type == W_KING)
            {
                std::vector<sf::Vector2i> kingValidOffsets
                {
                    { -1, -1 }, { 0, -1 }, { 1, -1 }, // top 
                    { -1, 0 }, { 1, 0 }, // mid
                    { -1, 1 }, { 0, 1 }, { 1, 1 } // bot
                };
                this->applyValidOffsets(selected, kingValidOffsets);

                // only allow castling if the king has not ever been moved
                if (!selectedPiece.isEverMoved)
                {
                    if (this->isCastlingLeftClear(selectedPiece))
                    {
                        sf::Vector2i targetCoords = { selectedPiece.coords.x - 2, selectedPiece.coords.y };

                        m_validMoves.emplace_back(PieceAction::CastleLeft, targetCoords);
                    }

                    if (this->isCastlingRightClear(selectedPiece))
                    {
                        sf::Vector2i targetCoords = { selectedPiece.coords.x + 2, selectedPiece.coords.y };

                        m_validMoves.emplace_back(PieceAction::CastleRight, targetCoords);
                    }
                }
            }
            return m_validMoves.size();
        }

        bool isCoordsValidMove(const sf::Vector2i& coords)
        {
            for (const auto& validMove : m_validMoves)
            {
                if (coords == validMove.coords) return true;
            }
            return false;
        }

    private:
        std::vector<PieceMovement> m_validMoves;

        ChessPieces* m_pieces = nullptr;

        bool isCastlingLeftClear(const Piece& kingPiece)
        {
            // check if path to cross is clear and free from attacks
            for (int x = 2; x >= 1; x--)
            {
                sf::Vector2i targetCoords = { x, kingPiece.coords.y };

                // tile is empty
                if (m_pieces->getType(targetCoords) == PieceType::None)
                {
                    if (!this->isTileFreeFromAttacks(targetCoords))
                    {
                        return false;
                    }
                }
                else return false;
            }

            // check if most left piece (most likely a rook) has not ever been moved
            return !m_pieces->getPiece({ 0, kingPiece.coords.y }).isEverMoved;
        }

        bool isCastlingRightClear(const Piece& kingPiece)
        {
            // check if path to cross is clear and free from attacks
            for (int x = 4; x <= 6; x++)
            {
                sf::Vector2i targetCoords = { x, kingPiece.coords.y };

                if (m_pieces->getType(targetCoords) == PieceType::None)
                {
                    if (!this->isTileFreeFromAttacks(targetCoords))
                    {
                        return false;
                    }
                }
                else return false;
            }

            // check if most left piece (most likely a rook) has not ever been moved
            return !m_pieces->getPiece({ 7, kingPiece.coords.y }).isEverMoved;
        }

        bool isTileFreeFromAttacks(const sf::Vector2i& coords)
        {
            bool isSafeFlag = false;

            auto resetTile = [&]()
            {
                Piece emptyPiece = Piece{
                .coords = coords,
                .type = PieceType::None,
                .color = m_pieces->getColor(coords)
                };
                m_pieces->setPiece(emptyPiece, coords);
            };

            auto isCanAttackBy = [&](const Piece& attackPattern)
            {
                m_pieces->setPiece(attackPattern, coords);

                MoveGenerator moveGenerator;
                moveGenerator.setPiecesToAnalyze(*m_pieces);
                moveGenerator.processValidMoves(coords);

                for (const auto& validMove : moveGenerator.getAllValidMoves())
                {
                    if (validMove.action == PieceAction::Capture)
                    {
                        return false;
                    }
                }
                return true;
            };

            // for orthogonal and diagonal attackers
            Piece queenAttackPattern = Piece{
                .coords = coords,
                .type = PieceType::W_QUEEN, // queen color doesn't matter
                .color = (coords.y == 7 ? PieceColor::White : PieceColor::Black)
            };

            if (!isCanAttackBy(queenAttackPattern))
            {
                resetTile();

                return false;
            }

            // for orthogonal and diagonal attackers
            Piece knightAttackPattern = Piece{
                .coords = coords,
                .type = (m_pieces->getColor(coords) == PieceColor::Black ? PieceType::B_KNIGHT : PieceType::W_KNIGHT), // queen color doesn't matter
                .color = (coords.y == 7 ? PieceColor::White : PieceColor::Black)
            };

            if (!isCanAttackBy(knightAttackPattern))
            {
                resetTile();

                return false;
            }
            resetTile();

            return true;
        }

        bool isCoordsInBounds(const sf::Vector2i& coords)
        {
            return coords.x >= 0 && coords.y >= 0 && coords.x < 8 && coords.y < 8;
        }

        bool isPieceCanTake(const sf::Vector2i& selected, const sf::Vector2i& target)
        {
            // taking an empty space is prohibited
            if (!m_pieces->isPieceExists(target))
            {
                return false;
            }

            // taking its own piece is prohibited
            return m_pieces->getColor(selected) != m_pieces->getColor(target);
        }

        void applyValidOffsets(const sf::Vector2i selected, std::vector<sf::Vector2i>& offsetsPtr)
        {
            for (auto& offset : offsetsPtr)
            {
                sf::Vector2i target = selected + offset;

                if (this->isCoordsInBounds(target))
                {
                    if (!m_pieces->isPieceExists(target))
                    {
                        m_validMoves.emplace_back(PieceMovement(PieceAction::Relocate, target));
                    }
                    else if (this->isPieceCanTake(selected, target))
                    {
                        m_validMoves.emplace_back(PieceMovement(PieceAction::Capture, target));
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
                    m_validMoves.emplace_back(PieceMovement(PieceAction::Relocate, target));
                }
                else
                {
                    if (this->isPieceCanTake(selected, target))
                    {
                        m_validMoves.emplace_back(PieceMovement(PieceAction::Capture, target));
                    }
                    return true;
                }
                return false;
            };

            std::vector<std::pair<sf::Vector2i, std::function<bool(sf::Vector2i target)>>> registers
            {
                { sf::Vector2i(-1, -1), [](const sf::Vector2i& target) { return target.x >= 0 && target.y >= 0; } },
                { sf::Vector2i(1, -1), [](const sf::Vector2i& target) { return target.x < 8 && target.y >= 0; } },
                { sf::Vector2i(-1, 1), [](const sf::Vector2i& target) { return target.x >= 0 && target.y < 8; } },
                { sf::Vector2i(1, 1), [](const sf::Vector2i& target) { return target.x < 8 && target.y < 8; } },
            };

            for (auto& reg : registers)
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
                    m_validMoves.emplace_back(PieceMovement(PieceAction::Relocate, target));
                }
                else
                {
                    if (this->isPieceCanTake(selected, target))
                    {
                        m_validMoves.emplace_back(PieceMovement(PieceAction::Capture, target));
                    }
                    return true;
                }
                return false;
            };

            // check for base to top
            for (int y = selected.y - 1; y >= 0; y--)
            {
                if (validateMove({ selected.x, y })) break;
            }

            // check for base to down                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
            for (int y = selected.y + 1; y < 8; y++)
            {
                if (validateMove({ selected.x, y })) break;
            }

            // check for base to left
            for (int x = selected.x - 1; x >= 0; x--)
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
}