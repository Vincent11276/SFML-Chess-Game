#pragma once

#include <SFML/Network.hpp>
#include "Core/Chess/PieceMovement.hpp"
#include "Room.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"
#include <variant>

class ServerMessage
{
public:
    struct AuthenticateSuccess
    {
        sf::Uint32 assignedId;
    };

    struct AuthenticateFailed
    {
        enum Reason : sf::Uint8
        {
            ServerFull,
            NameAlreadyExists,
            AlreadyAuthenticated
        };     
        Reason reason;
    };

    struct FetchedAvailableRooms
    {
        std::vector<Room> rooms;
    };

    struct PlayerMovedPiece
    {
        PieceMovement movement;
    };

    enum class Type : sf::Uint8
    {
        AuthenticateSuccess,
        AuthenticateFailed,
        CreateRoomSuccess,
        CreateRoomFailed,
        JoinRoomSuccess,
        JoinRoomFailed,
        FetchedAvailableRooms,
        HostLeftRoom,
        PlayerLeftRoom,
        PlayerMovedPiece,  
        PlayerRequestsForDraw,
        PlayerHasResigned,
        PlayerHasDisconnected,
        PlayerHasReconnected
    };
    Type type;

    ServerMessage() {}

    ServerMessage(Type p_type)
    {
        type = p_type;
    }

    template <typename T>
    ServerMessage(Type p_type, T m)
    {
        type = p_type;
        message = m;
    }

    void parseFromPacket(sf::Packet &packet);

    template <typename T>
    T& getData()
    {
        return std::get<T>(message);
    }

    sf::Packet& getPacket();
    
private:
    sf::Packet m_packet;

    using Message_t = std::variant<
        AuthenticateSuccess,
        AuthenticateFailed,
        FetchedAvailableRooms,
        PlayerMovedPiece
    >;
    
    Message_t message;
};