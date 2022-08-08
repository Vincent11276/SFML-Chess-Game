#pragma once

#include <SFML/Network.hpp>
#include <variant>
#include "Core/Chess/PieceMovement.hpp"
#include "Room.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"
#include "Core/Misc/GameMode.hpp"
#include "Core/Misc/MatchDuration.hpp"

class ClientMessage
{
public:    
    enum class Type : sf::Uint8
    {
        Undefined,
        Authenticate,
        RegisterPlayer,
        CreateNewRoom,
        JoinExistingRoom,
        FetchAvailableRooms,
        MovePiece,
        RequestForDraw,
        ResignGame,
    };
    Type type = Type::Undefined;

    struct RegisterPlayer
    {
        std::string name;
    };

    struct CreateNewRoom
    {
        std::string     name;
        std::string     password;
        GameMode        mode;
        MatchDuration   duration;
    };

    struct JoinExistingRoom
    {
        std::string     name;
        std::string     password;
        std::string     link;
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
        : type(p_type) { }

    template <typename T>
    ClientMessage(Type p_type, T m)
        : type(p_type), message(m) { }

    void parseFromPacket(sf::Packet &packet);

    template <typename T>
    const T& getData() const
    {
        return std::get<T>(message);
    }

    void getPacket(sf::Packet* packet);

private:
    using Message_t = std::variant<
        RegisterPlayer,
        CreateNewRoom,
        JoinExistingRoom,
        MovePiece
    >;
    Message_t message;
};