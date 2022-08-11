#pragma once


#include <SFML/Network.hpp>
#include "ClientSession.hpp"

namespace netw
{
    struct Player
    {
        ClientSession* session;

        enum class Color : sf::Uint8
        {
            NotSet,
            Black,
            White
        };
        Color color;
    };
}