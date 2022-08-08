#pragma once

#include <SFML/Network.hpp>
#include <string>

struct ClientSession
{
    sf::Uint32 token;

    sf::TcpSocket* client;

    bool isRegistered = false;
    bool isAuthenticated = false;

    enum class State : sf::Uint8
    {
        Idle,
        InGame,
        InRoom
    };
    State state = State::Idle;

    std::string     playerName;
    sf::Uint32      playerId;
    sf::Uint32      roomId;
};