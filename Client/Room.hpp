#pragma once

#include <SFML/Network.hpp>

#include "Player.hpp"


struct Room
{
    std::string id;
    std::string name;

    std::pair<Player, Player> players;
};

inline sf::Packet& operator <<(sf::Packet& packet, const Room& m)
{
    return packet << m.id << m.name;
}
inline sf::Packet& operator >>(sf::Packet& packet, Room& m)
{
    return packet >> m.id >> m.name;
}