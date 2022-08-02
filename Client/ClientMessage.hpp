#pragma once

#include <SFML/Network.hpp>
#include <variant>
#include "Core/Chess/PieceMovement.hpp"
#include "Room.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"

class ClientMessage
{
public:    
    enum class Type : sf::Uint8
    {
        Authenticate,
        CreateNewRoom,
        JoinExistingRoom,
        FetchAvailableRooms,
        MovePiece,  
        RequestForDraw,
        ResignGame
    };
    Type type;

    struct Authenticate
    {
        std::string name;
    };

    struct Room
    {
        std::string name;
        std::string password;
    };

    struct MovePiece
    {
        PieceMovement movement;
    };

    ClientMessage()
    {
        // default..
    }

    ClientMessage(Type p_type)
    {
        type = p_type;
    }

    template <typename T>
    ClientMessage(Type p_type, T m)
    {
        type = p_type;
        message = m;
    }

    void parseFromPacket(sf::Packet &packet);

    template <typename T>
    const T& getData() const
    {
        return std::get<T>(message);
    }

    sf::Packet& getPacket();

private:
    sf::Packet m_packet;

    using Message_t = std::variant<
        Authenticate,
        Room,
        MovePiece
    >;
    Message_t message;
};