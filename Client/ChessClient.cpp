#include "ChessClient.hpp"
#include "Client/ClientMessage.hpp"
#include "Client/Server.hpp"
#include "Client/ServerMessage.hpp"


bool ChessClient::connect()
{
    if (m_socket.connect(remoteAddress, remotePort) == sf::Socket::Done)
    {
        Logger::debug("You have been succesfuly connected to the server");

        t1 = std::thread(&ChessClient::handleIncomingPacketsAsync, this);

        m_isConnected = true;
    }
    else Logger::debug("Failed to connect you to the server. No internet connection?");

    return m_isConnected;
}

bool ChessClient::isConnected()
{
    return m_isConnected;
}

void ChessClient::onMessageReceived(ServerMessage::Type type, std::function<void(ServerMessage*)> callback)
{
    switch (type)
    {
    case ServerMessage::Type::AuthenticateSuccess: m_onAuthenticateSuccess = callback; break;
    case ServerMessage::Type::AuthenticateFailed: m_onAuthenticateFailed = callback; break;
    case ServerMessage::Type::RegistrationSuccess: m_onRegistrationSuccess = callback; break;
    case ServerMessage::Type::RegistrationFailed: m_onRegistrationFailed = callback; break;
    case ServerMessage::Type::CreateRoomSuccess: m_onCreateRoomSuccess = callback; break;
    case ServerMessage::Type::CreateRoomFailed: m_onCreateRoomFailed = callback; break;
    case ServerMessage::Type::JoinExistingRoomSuccess: m_onJoinRoomSuccess = callback; break;
    case ServerMessage::Type::JoinExistingRoomFailed: m_onJoinRoomFailed = callback; break;
    case ServerMessage::Type::FetchedAvailableRooms: m_onFetchedAvailableRooms = callback; break;
    case ServerMessage::Type::HostLeftRoom: m_onHostLeftRoom = callback; break;
    case ServerMessage::Type::PlayerLeftRoom: m_onPlayerLeftRoom = callback; break;
    case ServerMessage::Type::PlayerMovedPiece: m_onPlayerMovedPiece = callback; break;
    case ServerMessage::Type::PlayerRequestsForDraw: m_onPlayerRequestsForDraw = callback; break;
    case ServerMessage::Type::PlayerHasResigned: m_onPlayerHasResigned = callback; break;
    case ServerMessage::Type::PlayerHasDisconnected: m_onPlayerHasDisconnected = callback; break;
    case ServerMessage::Type::PlayerHasReconnected: m_onPlayerHasReconnected = callback; break;

    default:
        Logger::error("Unknown type received: <ServerMessage::Type({})>", (uint8_t)type);
    }
}

void ChessClient::authenticate()
{
    auto message = ClientMessage(ClientMessage::Type::Authenticate);

    this->sendMessage(message);
}

void ChessClient::registerPlayer(const std::string&name)
{
    auto message = ClientMessage(ClientMessage::Type::RegisterPlayer,
        ClientMessage::RegisterPlayer{
            .name = name
        });
    this->sendMessage(message);
}

void ChessClient::createNewRoom(const ClientMessage::CreateNewRoom& room)
{
    auto message = ClientMessage(ClientMessage::Type::CreateNewRoom, room);

    this->sendMessage(message);
}

void ChessClient::joinExistingRoom(const std::string &name, const std::string password)
{
    auto message = ClientMessage(ClientMessage::Type::JoinExistingRoom,
        ClientMessage::JoinExistingRoom{
            .name = name,
            .password = password,
            .link = ""
        });
    this->sendMessage(message);
}

void ChessClient::fetchAvailableRooms()
{
    auto message = ClientMessage(ClientMessage::Type::FetchAvailableRooms);

    this->sendMessage(message);
}

void ChessClient::movePiece(PieceMovement& pieceMovement)
{
    auto message = ClientMessage(ClientMessage::Type::MovePiece,
        ClientMessage::MovePiece{
            .movement = pieceMovement
        });
    this->sendMessage(message);
}


void ChessClient::resign()
{
    auto message = ClientMessage(ClientMessage::Type::ResignGame);

    this->sendMessage(message);
}

void ChessClient::sendMessage(ClientMessage& message)
{
    sf::Packet packet;
    packet << session.token;
    message.getPacket(&packet);

    if (m_socket.send(packet) != sf::Socket::Done)
    {
        std::cout << "Failed to send packet to the server!" << std::endl;
    }
}

void ChessClient::handleIncomingPacketsAsync()
{
    while (isAcceptingPackets)
    {
        sf::Packet packet;

        if (m_socket.receive(packet) == sf::Socket::Done)
        {
            ServerMessage message;
            message.parseFromPacket(packet);

            Logger::info("Received a message with the type <ServerMessage::Type({})> from the server", (uint8_t)message.type);

            switch (message.type)
            {
            case ServerMessage::Type::AuthenticateSuccess: m_onAuthenticateSuccess(&message); break;
            case ServerMessage::Type::AuthenticateFailed: m_onAuthenticateFailed(&message); break;
            case ServerMessage::Type::RegistrationSuccess: m_onRegistrationSuccess(&message); break;
            case ServerMessage::Type::RegistrationFailed: m_onRegistrationFailed(&message); break;
            case ServerMessage::Type::CreateRoomSuccess: m_onCreateRoomSuccess(&message); break;
            case ServerMessage::Type::CreateRoomFailed: m_onCreateRoomFailed(&message); break;
            case ServerMessage::Type::JoinExistingRoomSuccess: m_onJoinRoomSuccess(&message); break;
            case ServerMessage::Type::JoinExistingRoomFailed: m_onJoinRoomFailed(&message); break;
            case ServerMessage::Type::FetchedAvailableRooms: m_onFetchedAvailableRooms(&message); break;
            case ServerMessage::Type::HostLeftRoom: m_onHostLeftRoom(&message); break;
            case ServerMessage::Type::PlayerLeftRoom: m_onPlayerLeftRoom(&message); break;
            case ServerMessage::Type::PlayerMovedPiece: m_onPlayerMovedPiece  (&message); break;
            case ServerMessage::Type::PlayerRequestsForDraw: m_onPlayerRequestsForDraw(&message); break;
            case ServerMessage::Type::PlayerHasResigned: m_onPlayerHasResigned(&message); break;
            case ServerMessage::Type::PlayerHasDisconnected: m_onPlayerHasDisconnected(&message); break;
            case ServerMessage::Type::PlayerHasReconnected: m_onPlayerHasReconnected(&message); break;
            
            default:
                Logger::error("Unknown type received: <ServerMessage::Type({})>", (uint8_t)message.type);
            }
        }
        else std::cout << "Failed to receive packet from the server!" << std::endl;
    }
}