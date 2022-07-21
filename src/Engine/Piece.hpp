#pragma once

#include <SFML/System/Vector2.hpp>

#include "../Engine/PieceType.hpp"

#include "../Engine/PieceColor.hpp"

struct Piece
{
    sf::Vector2i coords;
    PieceType type;
    PieceColor color;

    bool isEverMoved = 0;
};