#pragma once

#include <SFML/Network.hpp>
#include <variant>
#include "Core/Chess/PieceMovement.hpp"
#include "Room.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"
#include "Core/Misc/GameMode.hpp"
#include "Core/Misc/MatchDuration.hpp"

namespace netw
{
    class ClientMessage
    {
    public:
        enum class Type : sf::Uint8
        {
            NotSet,
            Authenticate,
            RegisterPlayer,
            CreateNewRoom,
            JoinExistingRoom,
            FetchAvailableRooms,
            MovePiece,
            RequestForDraw,
            ResignGame,
            PlayerSendMessage,
            PlayerReadyToPlay
        };
        Type type = Type::NotSet;

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
            sf::Vector2i    selected;
            sf::Vector2i    target;
        };

        struct PlayerSendMessage
        {
            std::string text;
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

        void parseFromPacket(sf::Packet& packet);

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
            MovePiece,
            PlayerSendMessage
        >;
        Message_t message;
    };
}