#pragma once 

#include <functional>
#include <string>
#include <iostream>

#include "Client/ServerMessage.hpp"
#include "Core/Chess/PieceMovement.hpp"
#include "SFML/Config.hpp"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/Network/Socket.hpp"
#include "SFML/Network/TcpSocket.hpp"
#include "Client/ChessServer.hpp"
#include "Client/ClientMessage.hpp"
#include "Client/Scenes/Game.hpp"
#include "ClientSession.hpp"

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
    void onMessageReceived(ServerMessage::Type type, std::function<void(ServerMessage*)> callback);
    void sendMessage(ClientMessage& message);

    void authenticate();
    void registerPlayer(const std::string& name);
    void createNewRoom(const ClientMessage::CreateNewRoom& room);
    void joinExistingRoom(const std::string &roomId, const std::string password);
    void fetchAvailableRooms();
    void movePiece(PieceMovement& pieceMovement);
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

    void handleIncomingPacketsAsync();

    std::function<void(ServerMessage*)> m_onAuthenticateSuccess;
    std::function<void(ServerMessage*)> m_onAuthenticateFailed;
    std::function<void(ServerMessage*)> m_onRegistrationSuccess;
    std::function<void(ServerMessage*)> m_onRegistrationFailed;
    std::function<void(ServerMessage*)> m_onCreateRoomSuccess;
    std::function<void(ServerMessage*)> m_onCreateRoomFailed;
    std::function<void(ServerMessage*)> m_onJoinRoomSuccess;
    std::function<void(ServerMessage*)> m_onJoinRoomFailed;
    std::function<void(ServerMessage*)> m_onFetchedAvailableRooms;
    std::function<void(ServerMessage*)> m_onHostLeftRoom;
    std::function<void(ServerMessage*)> m_onPlayerLeftRoom;
    std::function<void(ServerMessage*)> m_onPlayerMovedPiece;
    std::function<void(ServerMessage*)> m_onPlayerRequestsForDraw;
    std::function<void(ServerMessage*)> m_onPlayerHasResigned;
    std::function<void(ServerMessage*)> m_onPlayerHasDisconnected;
    std::function<void(ServerMessage*)> m_onPlayerHasReconnected;

};