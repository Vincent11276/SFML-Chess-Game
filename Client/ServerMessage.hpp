#pragma once

#include <SFML/Network.hpp>
#include "Core/Chess/PieceMovement.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"
#include <variant>



class ServerMessage
{
public:
    struct AuthenticateSuccess
    {
        sf::Uint32 assignedToken;
    };

    struct RegistrationFailed
    {
        enum class Reason
        {
            ServerFull,
            NameAlreadyExists,
            AlreadyRegistered,
        };
        Reason reason;
    };

    struct CreateRoomFailed
    {
        enum class Reason : sf::Uint8
        {
            NameAlreadyExists,
            NotYetRegistered,
            IsInRoomAlready
        };
        Reason reason;
    };

    struct FetchedAvailableRooms
    {
        struct RoomListing
        {
            enum class Status : sf::Uint8
            {
                Waiting,
                Full,
                InGame
            };

            std::string name;
            uint8_t	size;
            Status status = Status::Waiting;
        };
        std::vector<RoomListing> roomListing;
    };

    struct PlayerMovedPiece
    {
        PieceMovement movement;
    };

    enum class Type : sf::Uint8
    {
        NotSet,
        AuthenticateSuccess,
        AuthenticateFailed,
        RegistrationSuccess,
        RegistrationFailed,
        CreateRoomSuccess,
        CreateRoomFailed,
        JoinExistingRoomSuccess,
        JoinExistingRoomFailed,
        FetchedAvailableRooms,
        HostLeftRoom,
        PlayerLeftRoom,
        PlayerMovedPiece,  
        PlayerRequestsForDraw,
        PlayerHasResigned,
        PlayerHasDisconnected,
        PlayerHasReconnected
    };
    Type type = Type::NotSet;

    ServerMessage() = default;

    ServerMessage(Type type_)
    {
        type = type_;
    }

    template <typename T>
    ServerMessage(Type type_, T m)
    {
        type = type_;
        content = m;
    }

    template <typename T>
    T& getContent()
    {
        return std::get<T>(content);
    }

    void parseFromPacket(sf::Packet &packet);

    void getPacket(sf::Packet* packet);
    
private:
    using Content_t = std::variant<
        AuthenticateSuccess,
        RegistrationFailed,
        CreateRoomFailed,
        FetchedAvailableRooms,
        PlayerMovedPiece
    >;
    
    Content_t content;
};