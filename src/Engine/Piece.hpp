#pragma once

#include "../Engine/PieceType.hpp"


struct Piece
{
    Piece()
    {
        // default..
    }

    Piece(PieceType p_type)
    {
        type = p_type;
    }
    
    PieceType type = PieceType::None;

    bool isEverMoved = 0;
};