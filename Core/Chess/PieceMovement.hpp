#pragma once

#include <SFML/System/Vector2.hpp>

#include "PieceAction.hpp"


namespace chess
{
    struct PieceMovement
    {
        PieceMovement() = default;

        PieceMovement(PieceAction p_action, sf::Vector2i p_target)
        {
            action = p_action;
            coords = p_target;
        }

        PieceAction action = PieceAction::None;
        sf::Vector2i coords;

        bool isKingChecked = false;
    };
}