#include "ClientMessage.hpp"
#include "SFML/Config.hpp"
#include <iostream>


sf::Packet& operator <<(sf::Packet& packet, const ClientMessage::RegisterPlayer& m)
{
    return packet << m.name;
}

sf::Packet& operator >>(sf::Packet& packet, ClientMessage::RegisterPlayer& m)
{
    return packet >> m.name;
}

sf::Packet& operator <<(sf::Packet& packet, const ClientMessage::CreateNewRoom& m)
{
    return packet << m.name << m.password << (sf::Uint8)m.mode << (sf::Uint8)m.duration;
}

sf::Packet& operator >>(sf::Packet& packet, ClientMessage::CreateNewRoom& m)
{
    packet >> m.name >> m.password;
 
    sf::Uint8 modeVal;
    packet >> modeVal;
    m.mode = GameMode(modeVal);

    sf::Uint8 durationVal;
    packet >> durationVal;
    m.duration = MatchDuration(modeVal);

    return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const ClientMessage::JoinExistingRoom& m)
{
    return packet << m.name << m.password << m.link;
}

sf::Packet& operator >>(sf::Packet& packet, ClientMessage::JoinExistingRoom& m)
{
    return packet >> m.name >> m.password >> m.link;
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

    std::cout << "Size of packet =" << packet.getDataSize() << std::endl;

    // insert content if it has one
    switch (type)
    {
    case Type::RegisterPlayer:
    {
        RegisterPlayer registerPlayer;
        packet >> registerPlayer;
        message = registerPlayer;
    }
    break;

    case Type::CreateNewRoom:
    {
        CreateNewRoom createNewRoom;
        packet >> createNewRoom;
        message = createNewRoom;
    }
    break;

    case Type::JoinExistingRoom:
    {
        JoinExistingRoom joinExistingRoom;
        packet >> joinExistingRoom;
        message = joinExistingRoom;
    }
    break;

    case Type::MovePiece:
    {
        MovePiece movePiece;
        packet >> movePiece;
        message = movePiece;
    }
    break;

    default:
        break;
    }
}

void ClientMessage::getPacket(sf::Packet *packet)
{
    *packet << (sf::Uint8)type;

    switch (type)
    {
    case Type::RegisterPlayer:
        *packet << std::get<RegisterPlayer>(message);
        break;

    case Type::CreateNewRoom:
        *packet << std::get<CreateNewRoom>(message);
        break;

    case Type::JoinExistingRoom:
        *packet << std::get<JoinExistingRoom>(message);
        break;

    case Type::MovePiece:
        *packet << std::get<MovePiece>(message);
        break;

    default:
        break;
    }
}