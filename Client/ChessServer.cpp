#include "ChessServer.hpp"
#include "Client/ClientMessage.hpp"
#include "Client/ServerMessage.hpp"
#include "SFML/Config.hpp"

ChessServer::ChessServer()
{
    
}

void ChessServer::sendMessage(sf::TcpSocket &client, ServerMessage& message)
{
    if (client.send(message.getPacket()) != sf::Socket::Done)
    {
        std::cout << "Failed to send packet to the server!" << std::endl;
    }
}

Player* ChessServer::getPlayerById(sf::Uint32 playerId)
{
    return &m_chessPlayers.at(playerId);
}

Room* ChessServer::getRoomById(sf::Uint32 roomId)
{
    return &m_chessRooms.at(roomId);
}

Room* ChessServer::getPlayerRoom(sf::Uint32 playerId)
{
    sf::Uint32 roomId = m_playerRoom.at(playerId);

    return this->getRoomById(roomId);
}

void ChessServer::onNewClientConnection(sf::TcpSocket &client)
{

}

void ChessServer::onClientDisconnect(sf::TcpSocket &client)
{

}

void ChessServer::onPacketReceived(sf::TcpSocket &client, sf::Packet& packet)
{
    // sf::Uint32 playerId = 0;
    // packet >> playerId;

    ClientMessage message;
    message.parseFromPacket(packet);

    ServerMessage response;

    switch (message.type)
    {
    case ClientMessage::Type::Authenticate:
        response = this->onAuthenticate(client, message);
        break;

    case ClientMessage::Type::CreateNewRoom:
        response = this->onCreateNewRoom(client, message);
        break;

    case ClientMessage::Type::JoinExistingRoom:
        response = this->onJoinExistingRoom(client, message);
        break;

    case ClientMessage::Type::FetchAvailableRooms:
        response = this->onFetchAvailableRooms(client, message);
        break;

    case ClientMessage::Type::MovePiece:
        response = this->onPieceMovement(client, message);
        break;

    case ClientMessage::Type::RequestForDraw:
        response = this->onRequestForDraw(client, message);
        break;

    case ClientMessage::Type::ResignGame:
        response = this->onResignGame(client, message);
        break;

    default:
        std::cout << "Unknown message type received" << std::endl;
        break;
    }
    this->sendMessage(client, response);
}

ServerMessage ChessServer::onAuthenticate(sf::TcpSocket &client, const ClientMessage& message)
{
    // check if player is already authenticated

    const auto& content = message.getData<ClientMessage::Authenticate>();
    std::cout << "Someone tried to authenticate with the name: " << content.name << std::endl;

    // if (playerId != -1)
    // {
    //     return ServerMessage(ServerMessage::Type::AuthenticateFailed,
    //         ServerMessage::AuthenticateFailed {
    //             .reason = ServerMessage::AuthenticateFailed::AlreadyAuthenticated,
    //     });
    // }

    sf::Uint32 assignedPlayerId = Random::getWholeNumber<sf::Uint32>();

    // ugly way to generate unique id ikik
    while (m_chessPlayers.find(assignedPlayerId) != m_chessPlayers.end())
    {
        assignedPlayerId = Random::getWholeNumber<sf::Uint32>();
    }
    m_chessPlayers.emplace(assignedPlayerId, Player(assignedPlayerId, content.name));

    return ServerMessage(ServerMessage::Type::AuthenticateSuccess, 
        ServerMessage::AuthenticateSuccess { 
            .assignedId = assignedPlayerId 
        }
    );
}

ServerMessage ChessServer::onCreateNewRoom(sf::TcpSocket &client, const ClientMessage& message)
{
    const auto& content = message.getData<ClientMessage::Room>();

    ServerMessage reply(ServerMessage::Type::CreateRoomSuccess);
    this->unicast(client, reply.getPacket());
}

ServerMessage ChessServer::onJoinExistingRoom(sf::TcpSocket &client, const ClientMessage& message)
{
    ServerMessage reply(ServerMessage::Type::JoinRoomSuccess);
    this->unicast(client, reply.getPacket());
}

ServerMessage ChessServer::onFetchAvailableRooms(sf::TcpSocket &client, const ClientMessage& message)
{

}

ServerMessage ChessServer::onPieceMovement(sf::TcpSocket &client, const ClientMessage& message)
{

}

ServerMessage ChessServer::onRequestForDraw(sf::TcpSocket &client, const ClientMessage& message)
{

}

ServerMessage ChessServer::onResignGame(sf::TcpSocket &client, const ClientMessage& message)
{

}