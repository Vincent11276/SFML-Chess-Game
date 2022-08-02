#include "ServerMessage.hpp"

#include <SFML/Network.hpp>
#include "Core/Chess/PieceAction.hpp"
#include "Core/Chess/PieceMovement.hpp"
#include "Room.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"


sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::AuthenticateSuccess& m)
{
    return packet << m.assignedId;
}

sf::Packet& operator >>(sf::Packet& packet, ServerMessage::AuthenticateSuccess& m)
{
    return packet >> m.assignedId;
}

sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::AuthenticateFailed& m)
{
    return packet << (sf::Uint8) m.reason;
}
sf::Packet& operator >>(sf::Packet& packet, ServerMessage::AuthenticateFailed& m)
{
    sf::Uint8 reasonVal;
    packet >> reasonVal;

    m.reason = ServerMessage::AuthenticateFailed::Reason(reasonVal);

    return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const ServerMessage::FetchedAvailableRooms& m)
{
    packet << m.rooms.size();

    for (auto& room: m.rooms)
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
        Room room;
        packet >> room;
        m.rooms.emplace_back(room);
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
        packet >> std::get<AuthenticateSuccess>(message);
        break;

    case Type::AuthenticateFailed:
        packet >> std::get<AuthenticateFailed>(message);
        break;

    case Type::FetchedAvailableRooms:
        packet >> std::get<FetchedAvailableRooms>(message);
        break;

    case Type::PlayerMovedPiece:
        packet >> std::get<PlayerMovedPiece>(message);
        break;

    default:
        break;
    }
}

sf::Packet& ServerMessage::getPacket()
{
    m_packet << (sf::Uint8)type;

    switch (type)
    {
    case Type::AuthenticateSuccess:
        m_packet << this->getData<AuthenticateSuccess>();
        break;

    case Type::AuthenticateFailed:
        m_packet << this->getData<AuthenticateFailed>();
        break;

    case Type::FetchedAvailableRooms:
        m_packet << this->getData<FetchedAvailableRooms>();
        break;

    case Type::PlayerMovedPiece:
        m_packet << this->getData<PlayerMovedPiece>();
        break;

    default:
        break;
    }
    return m_packet;
}