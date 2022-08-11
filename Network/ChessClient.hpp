#pragma once 

#include <functional>
#include <string>
#include <iostream>

#include "Network/ServerMessage.hpp"
#include "Core/Chess/PieceMovement.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/Network/Socket.hpp"
#include "SFML/Network/TcpSocket.hpp"
#include "Network/ChessServer.hpp"
#include "Network/ClientMessage.hpp"
#include "Client/Scenes/Game.hpp"
#include "ClientSession.hpp"

namespace netw
{
    class ChessServer;

    class ChessClient
    {
    public:
        ClientSession session;
        sf::IpAddress remoteAddress = sf::IpAddress::getLocalAddress();
        uint16_t remotePort = 53000;
        bool isAcceptingPackets = true;

        bool connect();
        bool isConnected();
        void registerCallback(ServerMessage::Type type, std::function<void(ServerMessage*)> callback);
        void sendMessage(ClientMessage& message);

        void authenticate();
        void registerPlayer(const std::string& name);
        void createNewRoom(const ClientMessage::CreateNewRoom& room);
        void joinExistingRoom(const std::string& roomId, const std::string password);
        void fetchAvailableRooms();
        void movePiece(const ClientMessage::MovePiece& pieceMovement);
        void sendChatMessage(const std::string& message);
        void resign();

        ChessClient() = default;
        ChessClient(const ChessClient&) = delete;
        ChessClient& operator=(const ChessClient&) = delete;

        static ChessClient& getInstance()
        {
            static ChessClient instance;
            return instance;
        }

    private:
        bool m_isConnected = false;

        std::thread t1;
        sf::TcpSocket m_socket;

        std::map<ServerMessage::Type,
            std::function<void(ServerMessage*)>> m_callbacks;

        void handleIncomingPacketsAsync();
    };
}