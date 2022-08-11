#include "ChessClient.hpp"
#include "Network/ClientMessage.hpp"
#include "Network/Server.hpp"
#include "Network/ServerMessage.hpp"

namespace netw
{
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

    void ChessClient::registerCallback(ServerMessage::Type type, std::function<void(ServerMessage*)> callback)
    {
        m_callbacks[type] = callback;
    }

    void ChessClient::authenticate()
    {
        auto message = ClientMessage(ClientMessage::Type::Authenticate);

        this->sendMessage(message);
    }

    void ChessClient::registerPlayer(const std::string& name)
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

    void ChessClient::joinExistingRoom(const std::string& name, const std::string password)
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

    void ChessClient::movePiece(const ClientMessage::MovePiece& movePiece)
    {
        auto message = ClientMessage(ClientMessage::Type::MovePiece,
            ClientMessage::MovePiece{
                .selected = movePiece.selected,
                .target = movePiece.target
            });
        this->sendMessage(message);
    }


    void ChessClient::resign()
    {
        auto message = ClientMessage(ClientMessage::Type::ResignGame);

        this->sendMessage(message);
    }

    void ChessClient::sendChatMessage(const std::string& chatMessage)
    {
        auto message = ClientMessage(ClientMessage::Type::PlayerSendMessage,
            ClientMessage::PlayerSendMessage{
                .text = chatMessage
            });
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

                Logger::debug("Received a message with the type <ServerMessage::Type({})> from the server", (uint8_t)message.type);

                // Call the specific function callback if it has been registered
                if (m_callbacks.find(message.type) != m_callbacks.end())
                {
                    m_callbacks.at(message.type)(&message);
                }
                else Logger::error("No known callback has been set for <ServerMessage::Type({})>", (uint8_t)message.type);
            }
            else Logger::error("Failed to receive packet from the server!");
        }
    }
}