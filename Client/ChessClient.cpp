#include "ChessClient.hpp"
#include "Client/ClientMessage.hpp"
#include "Client/Server.hpp"
#include "Client/ServerMessage.hpp"


void ChessClient::connect()
{
    if (m_socket.connect(remoteAddress, remotePort) == sf::Socket::Done)
    {            
        t1 = std::thread(&ChessClient::handleIncomingPacketsAsync, this);

        m_isConnected = true;
    }
}

void ChessClient::onMessageReceived(std::function<void(ServerMessage*)> callback)
{
    m_onMessageReceived = callback;
}

bool ChessClient::isConnected()
{
    return m_isConnected;
}

void ChessClient::authenticate(const std::string& name)
{
    auto message = ClientMessage(ClientMessage::Type::Authenticate,
        ClientMessage::Authenticate {
            .name = name
    });
    this->sendMessage(message);
}

void ChessClient::createNewRoom(const std::string &name, const std::string password)
{
    auto message = ClientMessage(ClientMessage::Type::CreateNewRoom,
        ClientMessage::Room {
            .name = name,
            .password = password
    });
    this->sendMessage(message);
}

void ChessClient::joinExistingRoom(const std::string &name, const std::string password)
{
    auto message = ClientMessage(ClientMessage::Type::JoinExistingRoom,
        ClientMessage::Room {
            .name = name,
            .password = password
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
    if (m_socket.send(message.getPacket()) != sf::Socket::Done)
    {
        std::cout << "Failed to send packet to the server!" << std::endl;
    }
}

void ChessClient::handleIncomingPacketsAsync()
{
    while (true)
    {
        sf::Packet packet;

        if (m_socket.receive(packet) == sf::Socket::Done)
        {
            std::cout << "received something" << std::endl;
            ServerMessage message;
            message.parseFromPacket(packet);
            m_onMessageReceived(&message);
        }
        else std::cout << "Failed to receive packet from the server!" << std::endl;
    }
}