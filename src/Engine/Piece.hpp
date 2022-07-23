#pragma once

#include <SFML/System/Vector2.hpp>

#include "../Engine/PieceType.hpp"

#include "../Engine/PieceColor.hpp"

struct Piece
{
    sf::Vector2i coords;
    PieceType type = PieceType::None;
    PieceColor color = PieceColor::Neutral;

    // turns on when piece is moved
    bool isEverMoved = 0;

    // only triggers on first move, turns back off next move
    bool isFirstMove = 0;
};