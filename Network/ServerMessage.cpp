#include "ServerMessage.hpp"

#include <SFML/Network.hpp>
#include "Core/Chess/PieceAction.hpp"
#include "Core/Chess/PieceMovement.hpp"
#include "Room.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"

namespace netw
{
    sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::AuthenticateSuccess& m)
    {
        return packet << m.assignedToken;
    }

    sf::Packet& operator >>(sf::Packet& packet, ServerMessage::AuthenticateSuccess& m)
    {
        return packet >> m.assignedToken;
    }

    sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::RegistrationFailed& m)
    {
        return packet << (sf::Uint8)m.reason;
    }
    sf::Packet& operator >>(sf::Packet& packet, ServerMessage::RegistrationFailed& m)
    {
        sf::Uint8 reasonVal;
        packet >> reasonVal;

        m.reason = ServerMessage::RegistrationFailed::Reason(reasonVal);

        return packet;
    }

    inline sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::FetchedAvailableRooms::RoomListing& m)
    {
        return packet << m.name << m.size << (sf::Uint8)m.status;
    }
    inline sf::Packet& operator >>(sf::Packet& packet, ServerMessage::FetchedAvailableRooms::RoomListing& m)
    {
        packet >> m.name >> m.size;

        sf::Uint8 statusVal;
        packet >> statusVal;

        m.status = ServerMessage::FetchedAvailableRooms::RoomListing::Status(statusVal);

        return packet;
    }

    sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::FetchedAvailableRooms& m)
    {
        packet << m.roomListing.size();

        for (auto& room : m.roomListing)
        {
            packet << room;
        }
        return packet;
    }

    sf::Packet& operator >>(sf::Packet& packet, ServerMessage::FetchedAvailableRooms& m)
    {
        std::size_t arrSize;
        packet >> arrSize;

        for (std::size_t i = 0; i < arrSize; i++)
        {
            ServerMessage::FetchedAvailableRooms::RoomListing room;
            packet >> room;
            m.roomListing.emplace_back(room);
        }
        return packet;
    }

    sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::PlayerMovedPiece& m)
    {
        return packet << m.selected.x << m.selected.y << m.target.x << m.target.y;
    }

    sf::Packet& operator >>(sf::Packet& packet, ServerMessage::PlayerMovedPiece& m)
    {
        packet >> m.selected.x;
        packet >> m.selected.y;

        packet >> m.target.x;
        packet >> m.target.y;

        return packet;
    }

    sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::PlayerSentMessage& m)
    {
        return packet << m.author << m.text;
    }

    sf::Packet& operator >>(sf::Packet& packet, ServerMessage::PlayerSentMessage& m)
    {
        return packet >> m.author >> m.text;
    }

    sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::ChessGameStarted& m)
    {
        return packet << (sf::Uint8)m.side;
    }

    sf::Packet& operator >>(sf::Packet& packet, ServerMessage::ChessGameStarted& m)
    {
        sf::Uint8 sideVal;
        packet >> sideVal;
        m.side = (Player::Color)sideVal;

        return packet;
    }

    sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::PlayerJoinedRoom& m)
    {
        return packet << m.name;
    }

    sf::Packet& operator >>(sf::Packet& packet, ServerMessage::PlayerJoinedRoom& m)
    {
        return packet >> m.name;
    }

    void ServerMessage::parseFromPacket(sf::Packet& packet)
    {
        sf::Uint8 typeVal;
        packet >> typeVal;
        type = Type(typeVal);

        switch (type)
        {
        case Type::AuthenticateSuccess:
        {
            AuthenticateSuccess authenticateSuccess;
            packet >> authenticateSuccess;
            content = authenticateSuccess;
        }
        break;

        case Type::RegistrationFailed:
        {
            RegistrationFailed registrationFailed;
            packet >> registrationFailed;
            content = registrationFailed;
        }
        break;

        case Type::FetchedAvailableRooms:
        {
            FetchedAvailableRooms fetchedAvailableRooms;
            packet >> fetchedAvailableRooms;
            content = fetchedAvailableRooms;
        }
        break;

        case Type::PlayerMovedPiece:
        {
            PlayerMovedPiece playerMovedPiece;
            packet >> playerMovedPiece;
            content = playerMovedPiece;
        }
        break;

        case Type::PlayerSentMessage:
        {
            PlayerSentMessage playerSentMessage;
            packet >> playerSentMessage;
            content = playerSentMessage;
        }
        break;

        case Type::ChessGameStarted:
        {
            ChessGameStarted chessGameStarted;
            packet >> chessGameStarted;
            content = chessGameStarted;
        }
        break;

        case Type::PlayerJoinedRoom:
        {
            PlayerJoinedRoom playerJoinedRoom;
            packet >> playerJoinedRoom;
            content = playerJoinedRoom;
        }
        break;

        default:
            break;
        }
    }

    void ServerMessage::getPacket(sf::Packet* packet)
    {
        *packet << (sf::Uint8)type;

        switch (type)
        {
        case Type::AuthenticateSuccess:
            *packet << this->getContent<AuthenticateSuccess>();
            break;

        case Type::RegistrationFailed:
            *packet << this->getContent<RegistrationFailed>();
            break;

        case Type::FetchedAvailableRooms:
            *packet << this->getContent<FetchedAvailableRooms>();
            break;

        case Type::PlayerMovedPiece:
            *packet << this->getContent<PlayerMovedPiece>();
            break;

        case Type::PlayerSentMessage:
            *packet << this->getContent<PlayerSentMessage>();
            break;

        case Type::ChessGameStarted:
            *packet << this->getContent<ChessGameStarted>();
            break;

        case Type::PlayerJoinedRoom:
            *packet << this->getContent<PlayerJoinedRoom>();
            break;

        default:
            break;
        }
    }
}