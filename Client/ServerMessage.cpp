#include "ServerMessage.hpp"

#include <SFML/Network.hpp>
#include "Core/Chess/PieceAction.hpp"
#include "Core/Chess/PieceMovement.hpp"
#include "Room.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"


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

    for (auto& room: m.roomListing)
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
    return packet << (sf::Uint8)m.movement.action << m.movement.coords.x << m.movement.coords.y;
}

sf::Packet& operator >>(sf::Packet& packet, ServerMessage::PlayerMovedPiece& m)
{
    sf::Uint8 actionVal;
    packet >> actionVal;
    packet >> m.movement.coords.x ;
    packet >> m.movement.coords.y;
    m.movement.action = PieceAction(actionVal);

    return packet; 
}

void ServerMessage::parseFromPacket(sf::Packet &packet)
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

    default:
        break;
    }
}