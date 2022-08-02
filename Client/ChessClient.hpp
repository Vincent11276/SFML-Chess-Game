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


class ChessServer;

class ChessClient
{
public:
    sf::IpAddress remoteAddress = sf::IpAddress::getLocalAddress();
    unsigned short remotePort = 53000;

    ChessClient() = default;
    ChessClient(const ChessClient&) = delete;
    ChessClient& operator=(const ChessClient&) = delete;

    void connect();

    void onMessageReceived(std::function<void(ServerMessage*)> callback);

    bool isConnected();
 
    void authenticate(const std::string &name);
    void createNewRoom(const std::string &roomId, const std::string password);
    void joinExistingRoom(const std::string &roomId, const std::string password);
    void sendPieceMovement(PieceMovement& pieceMovement);
    void resign();

    enum MessageType : sf::Uint8
    {
        Authenticate,
        CreateNewRoom,
        JoinExistingRoom,
        FetchAvailableRooms,
        PieceMovement,  
        RequestForDraw,
        ResignGame
    };
    
private:
    std::thread t1;

    sf::Uint32 m_clientId = 0;
    bool m_isConnected = false;
    
    std::function<void(ServerMessage*)> m_onMessageReceived;
    sf::TcpSocket m_socket;

    void sendMessage(ClientMessage& message);

    void handleIncomingPacketsAsync();
};