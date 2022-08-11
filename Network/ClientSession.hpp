#pragma once

#include <SFML/Network.hpp>
#include <string>

namespace netw
{
    struct ClientSession
    {
        sf::Uint32 token = 0;

        sf::TcpSocket* client = nullptr;

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
        sf::Uint32      playerId = 0;
        sf::Uint32      roomId = 0;
    };
}