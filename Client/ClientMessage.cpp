#include "ClientMessage.hpp"
#include "SFML/Config.hpp"
#include <iostream>


sf::Packet& operator <<(sf::Packet& packet, const ClientMessage::Authenticate& m)
{
    return packet << m.name;
}

sf::Packet& operator >>(sf::Packet& packet, ClientMessage::Authenticate& m)
{
    return packet >> m.name;
}

sf::Packet& operator <<(sf::Packet& packet, const ClientMessage::Room& m)
{
    return packet << m.name << m.password;
}

sf::Packet& operator >>(sf::Packet& packet, ClientMessage::Room& m)
{
    return packet >> m.name >> m.password;
}

sf::Packet& operator <<(sf::Packet& packet, const ClientMessage::MovePiece& m)
{
    return packet << (sf::Uint8)m.movement.action << m.movement.coords.x << m.movement.coords.y;
}

sf::Packet& operator >>(sf::Packet& packet, ClientMessage::MovePiece& m)
{
    sf::Uint8 actionVal;
    packet >> actionVal;
    packet >> m.movement.coords.x;
    packet >> m.movement.coords.y;
    m.movement.action = PieceAction(actionVal);
    
    return packet;
}

void ClientMessage::parseFromPacket(sf::Packet &packet)
{
    sf::Uint8 typeVal;
    packet >> typeVal;

    type = Type(typeVal);

    switch (type)
    {
    case Type::Authenticate:
        packet >> std::get<Authenticate>(message);
        break;

    case Type::CreateNewRoom:
        packet >> std::get<Room>(message);
        break;

    case Type::JoinExistingRoom:
        packet >> std::get<Room>(message);
        break;

    case Type::MovePiece:
        packet >> std::get<MovePiece>(message);
        break;

    default:
        break;
    }
}

sf::Packet& ClientMessage::getPacket()
{
    m_packet << (sf::Uint8)type;

    switch (type)
    {
    case Type::Authenticate:
        m_packet << std::get<Authenticate>(message);
        break;

    case Type::CreateNewRoom:
        m_packet << std::get<Room>(message);
        break;

    case Type::JoinExistingRoom:
        m_packet << std::get<Room>(message);
        break;

    case Type::MovePiece:
        m_packet << std::get<MovePiece>(message);
        break;

    default:
        break;
    }
    return m_packet;
}