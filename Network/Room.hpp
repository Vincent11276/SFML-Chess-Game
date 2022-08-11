#pragma once

#include <SFML/Network.hpp>

#include "Player.hpp"
#include "Core/Misc/RoomStatus.hpp"

namespace netw
{
    struct Room
    {
        sf::Uint32      id;
        std::string     name;
        std::string     password;
        std::string     joinLink;
        sf::Uint8       size;
        RoomStatus      status;

        std::vector<Player> players;
    };
}