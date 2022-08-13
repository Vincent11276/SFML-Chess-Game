#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

#include "Client/Scenes/ChessPieces.hpp"
#include "Core/Utility/Helper.hpp"
#include "Piece.hpp"
#include "PieceAction.hpp"
#include "PieceMovement.hpp"

namespace chess
{
    class MoveGenerator
    {
    public:
        struct Config
        {
            chess::Piece selected;
            std::pair<bool, bool> isChecked;

        };

        MoveGenerator();

        void setPiecesToAnalyze(ChessPieces& chessPieces);
        bool processValidMoves(const sf::Vector2i& selected, const std::pair<Piece, PieceMovement>& previousMove = std::pair<Piece, PieceMovement>());
        bool isCoordsValidMove(const sf::Vector2i& coords);
        bool isThereAnythingtoTake();

        PieceMovement findValidMoveByCoords(const sf::Vector2i& coords);
        const std::vector<PieceMovement>& getAllValidMoves();
        bool isTileFreeFromAttacks(const sf::Vector2i& coords);

    private:
        std::vector<PieceMovement> m_validMoves;
        ChessPieces* m_pieces = nullptr;

        bool isCastlingLeftClear(const Piece& kingPiece);
        bool isCastlingRightClear(const Piece& kingPiece);
        bool isCoordsInBounds(const sf::Vector2i& coords);
        bool isPieceCanTake(const sf::Vector2i& selected, const sf::Vector2i& target);

        void applyValidOffsets(const sf::Vector2i selected, std::vector<sf::Vector2i>& offsetsPtr);
        void processEnPessantViability(const Piece& selectedPiece, const std::pair<Piece, PieceMovement>& previousMove, const sf::Vector2i& target);
        void generateDiagonalMoves(const sf::Vector2i& selected);
        void generateOrthogonalMoves(const sf::Vector2i& selected);
    };
}