#pragma once


#include <SFML/Network.hpp>

struct Player
{
    Player()
    {
        // default..
    }

    Player(sf::Uint32 p_id, const std::string& p_name)
    {
        id = p_id;
        name = p_name;
    }

    sf::Uint32 id;
    std::string name;
};