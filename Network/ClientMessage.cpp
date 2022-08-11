#include "ClientMessage.hpp"
#include "SFML/Config.hpp"
#include <iostream>

namespace netw
{
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
        return packet << m.selected.x << m.selected.y << m.target.x << m.target.y;
    }

    sf::Packet& operator >>(sf::Packet& packet, ClientMessage::MovePiece& m)
    {
        packet >> m.selected.x;
        packet >> m.selected.y;

        packet >> m.target.x;
        packet >> m.target.y;

        return packet;
    }

    sf::Packet& operator <<(sf::Packet& packet, const ClientMessage::PlayerSendMessage& m)
    {
        return packet << m.text;
    }

    sf::Packet& operator >>(sf::Packet& packet, ClientMessage::PlayerSendMessage& m)
    {
        return packet >> m.text;
    }

    void ClientMessage::parseFromPacket(sf::Packet& packet)
    {
        sf::Uint8 typeVal;
        packet >> typeVal;

        type = Type(typeVal);

        // insert content if it has one
        switch (type)
        {
        case ClientMessage::Type::RegisterPlayer:
        {
            RegisterPlayer registerPlayer;
            packet >> registerPlayer;
            message = registerPlayer;
        }
        break;

        case ClientMessage::Type::CreateNewRoom:
        {
            CreateNewRoom createNewRoom;
            packet >> createNewRoom;
            message = createNewRoom;
        }
        break;

        case ClientMessage::Type::JoinExistingRoom:
        {
            JoinExistingRoom joinExistingRoom;
            packet >> joinExistingRoom;
            message = joinExistingRoom;
        }
        break;

        case ClientMessage::Type::MovePiece:
        {
            MovePiece movePiece;
            packet >> movePiece;
            message = movePiece;
        }
        break;

        case ClientMessage::Type::PlayerSendMessage:
        {
            PlayerSendMessage sendMessage;
            packet >> sendMessage;
            message = sendMessage;
        }

        default:
            break;
        }
    }

    void ClientMessage::getPacket(sf::Packet* packet)
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

        case Type::PlayerSendMessage:
            *packet << std::get<PlayerSendMessage>(message);
            break;

        default:
            break;
        }
    }
}