#include "ChessServer.hpp"
#include "Client/ClientMessage.hpp"
#include "Client/ServerMessage.hpp"
#include "SFML/Config.hpp"


ChessServer::ChessServer()
{
    
}

void ChessServer::addPlayerToRoom(Room* room, sf::Uint32 token)
{
    room->players.push_back(Player{
        .session = &m_sessions.at(token),
        .color = Player::Color::White
        });
    m_sessions.at(token).roomId = room->id;
}

void ChessServer::sendMessage(sf::TcpSocket &client, ServerMessage& message)
{
    sf::Packet packet;
    message.getPacket(&packet);

    if (client.send(packet) != sf::Socket::Done)
    {
        Logger::error("Failed to send packet to the server!");
    }
    else Logger::info("Sent <Message::Type({})> message to the server", (sf::Uint8)message.type);
}

void ChessServer::onNewClientConnection(sf::TcpSocket &client)
{

}

void ChessServer::onClientDisconnect(sf::TcpSocket &client)
{

}

void ChessServer::onPacketReceived(sf::TcpSocket &client, sf::Packet& packet)
{
    sf::Uint32 token;
    packet >> token;
    ClientMessage message;
    message.parseFromPacket(packet);
    ServerMessage response;

    switch (message.type)
    {
    case ClientMessage::Type::Authenticate:
        response = this->onAuthenticate(client, token, message);
        break;

    case ClientMessage::Type::RegisterPlayer:
        response = this->onRegisterPlayer(client, token, message);
        break;

    case ClientMessage::Type::CreateNewRoom:
        response = this->onCreateNewRoom(client, token, message);
        break;

    case ClientMessage::Type::JoinExistingRoom:
        response = this->onJoinExistingRoom(client, token, message);
        break;

    case ClientMessage::Type::FetchAvailableRooms:
        response = this->onFetchAvailableRooms(client, token, message);
        break;

    case ClientMessage::Type::MovePiece:
        response = this->onPieceMovement(client, token, message);
        break;

    case ClientMessage::Type::RequestForDraw:
        response = this->onRequestForDraw(client, token, message);
        break;

    case ClientMessage::Type::ResignGame:
        response = this->onResignGame(client, token, message);
        break;

    default:
        Logger::critical("Unknown message type received from {}. Type=<MessageType::Type({})>", token, (sf::Uint8)message.type);
        return;
    }
    Logger::info("Received Type=<MessageType::Type({})> from {}", (sf::Uint8)message.type, token);
    this->sendMessage(client, response);
}

ServerMessage ChessServer::onAuthenticate(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message)
{
    // check if player is already authenticated
    if (token != 0) return ServerMessage(ServerMessage::Type::AuthenticateFailed);
    
    sf::Uint32 assignedToken = Helper::makeUniqueId(m_sessions);
    m_sessions.emplace(assignedToken, ClientSession());
    m_sessions.at(assignedToken).isAuthenticated = true;
    m_sessions.at(assignedToken).token = assignedToken;

    return ServerMessage(ServerMessage::Type::AuthenticateSuccess,
        ServerMessage::AuthenticateSuccess{
            .assignedToken = assignedToken
        }
    );
}

ServerMessage ChessServer::onRegisterPlayer(sf::TcpSocket& client, sf::Uint32 token, const ClientMessage& message)
{
    // Check if player is not yet authenticated
    if (token == 0) return ServerMessage(ServerMessage::Type::RegistrationFailed);
        
    const auto& content = message.getData<ClientMessage::RegisterPlayer>();

    m_sessions.at(token).isRegistered = true;
    m_sessions.at(token).playerName = content.name;

    return ServerMessage(ServerMessage::Type::RegistrationSuccess);
}

ServerMessage ChessServer::onCreateNewRoom(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message)
{
    const auto& content = message.getData<ClientMessage::CreateNewRoom>();

    // Check if player is registered
    if (token == 0)
    {
        Logger::critical("{} tried to create a room but is not yet registered", token);
      
        return ServerMessage(ServerMessage::Type::CreateRoomFailed,
            ServerMessage::CreateRoomFailed{
                .reason = ServerMessage::CreateRoomFailed::Reason::NotYetRegistered
            });
    }

    // Check if player is already in a room
    if (m_sessions.at(token).state == ClientSession::State::InRoom)
    {
        return ServerMessage(ServerMessage::Type::CreateRoomFailed,
            ServerMessage::CreateRoomFailed{
                .reason = ServerMessage::CreateRoomFailed::Reason::IsInRoomAlready
            });
    }

    // Check if room name is already taken 
    for (const auto& pair : m_rooms)
    {
        if (pair.second.name == content.name)
        {
            return ServerMessage(ServerMessage::Type::CreateRoomFailed,
                ServerMessage::CreateRoomFailed{
                    .reason = ServerMessage::CreateRoomFailed::Reason::NameAlreadyExists
                });
        }
    }

    // Finally create the new room and add the host inside
    Room newRoom {
        .id = Helper::makeUniqueId(m_rooms),
        .name = content.name,
        .password = content.password,
        .joinLink = Random::getAlphaNumericString(JOIN_CODE_SIZE),
        .size = DEFAULT_ROOM_SIZE,
        .status = RoomStatus::Waiting,
    };
    this->addPlayerToRoom(&newRoom, token);
    m_rooms.emplace(newRoom.id, newRoom);
    
    // Tell the client that room creation has been successful
    return ServerMessage(ServerMessage::Type::CreateRoomSuccess);
}

ServerMessage ChessServer::onJoinExistingRoom(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message)
{
    const auto& content = message.getData<ClientMessage::JoinExistingRoom>();

    // Check if player is NOT already in a room
    if (m_sessions.at(token).state != ClientSession::State::Idle)
    {
        return ServerMessage(ServerMessage::Type::JoinExistingRoomFailed);
    }

    // Check if room exists by iterating through every rooms
    for (const auto& pair : m_rooms)
    {
        if (pair.second.name == content.name)
        {
            // Check if the given password matches
            if (pair.second.password == content.password)
            {
                return ServerMessage(ServerMessage::Type::JoinExistingRoomSuccess);
            }
        }
    }

    // Otherwise don't accept the request
    return ServerMessage(ServerMessage::Type::JoinExistingRoomFailed);
}

ServerMessage ChessServer::onFetchAvailableRooms(sf::TcpSocket& client, sf::Uint32 token, const ClientMessage& message)
{
    auto roomListing = std::vector<ServerMessage::FetchedAvailableRooms::RoomListing>();

    // Loop through available rooms and insert required datas
    for (const auto& value : m_rooms)
    {
        using RoomStatus_t = ServerMessage::FetchedAvailableRooms::RoomListing::Status;

        auto newRoom = ServerMessage::FetchedAvailableRooms::RoomListing{
            .name = value.second.name,
            .size = value.second.size,
            .status = (RoomStatus_t)value.second.status
        };
        roomListing.push_back(newRoom);
    }

    return ServerMessage(ServerMessage::Type::FetchedAvailableRooms,
        ServerMessage::FetchedAvailableRooms{
            .roomListing = roomListing
        });
}

ServerMessage ChessServer::onPieceMovement(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message)
{
    const auto& content = message.getData<ClientMessage::MovePiece>();

    // Echo the movement to every client inside the room
    for (auto& player : m_rooms.at(m_sessions.at(token).roomId).players)
    {
        // Prevents sending the packet to itself
        if (player.session->token == token) continue;
       
        auto response = ServerMessage(ServerMessage::Type::PlayerMovedPiece,
            ServerMessage::PlayerMovedPiece{
                .movement = content.movement
            });
        this->sendMessage(*player.session->client, response);
    }
    return ServerMessage();
}

ServerMessage ChessServer::onRequestForDraw(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message)
{
    // Echo the movement to every client inside the room
    Room* room = &m_rooms.at(m_sessions.at(token).roomId);

    for (auto& player : room->players)
    {
        // Prevents sending the packet to itself
        if (player.session->token == token) continue;

        auto response = ServerMessage(ServerMessage::Type::PlayerRequestsForDraw);

        this->sendMessage(*player.session->client, response);
    }
    return ServerMessage();
}

ServerMessage ChessServer::onResignGame(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message)
{
    // Echo the movement to every client inside the room
    for (auto& player : m_rooms.at(m_sessions.at(token).roomId).players)
    {
        // Prevents sending the packet to itself
        if (player.session->token == token) continue;

        auto response = ServerMessage(ServerMessage::Type::PlayerHasResigned);

        this->sendMessage(*player.session->client, response);
    }
    return ServerMessage();
}