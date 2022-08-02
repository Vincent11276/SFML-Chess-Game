#pragma once

#include <SFML/Network/Packet.hpp>
#include <map>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <list>

#include "SFML/Config.hpp"
#include "Server.hpp"
#include "ChessClient.hpp"
#include "Random.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "ServerMessage.hpp"
#include "ClientMessage.hpp"

typedef sf::Uint32 ClientToken;

class ClientSession
{
public:
    s
    
    ClientSession()
    {
    
    }

    
};

class ChessServer : public Server
{
public:
    sf::Uint32 maxPlayers = 100;

    ChessServer();
    
private:
    // actual storage of players and rooms bounded to its ID
    std::unordered_map<sf::Uint32, Player> m_chessPlayers;
    std::unordered_map<sf::Uint32, Room> m_chessRooms;

    // stores which room the player is in by mapping its ID
    std::unordered_map<sf::Uint32, sf::Uint32> m_playerRoom;


    
    Player* getPlayerById(sf::Uint32 id);
    Room* getRoomById(sf::Uint32 id);
    Room* getPlayerRoom(sf::Uint32 id);

    void onNewClientConnection(sf::TcpSocket &client) override;
    void onClientDisconnect(sf::TcpSocket &client) override;
    void onPacketReceived(sf::TcpSocket &client, sf::Packet& packet) override;

    void sendMessage(sf::TcpSocket &client, ServerMessage& message);

    ServerMessage onAuthenticate(sf::TcpSocket &client, const ClientMessage& message);
    ServerMessage onCreateNewRoom(sf::TcpSocket &client, const ClientMessage& message);
    ServerMessage onJoinExistingRoom(sf::TcpSocket &client, const ClientMessage& message);
    ServerMessage onFetchAvailableRooms(sf::TcpSocket &client, const ClientMessage& message);
    ServerMessage onPieceMovement(sf::TcpSocket &client, const ClientMessage& message);
    ServerMessage onRequestForDraw(sf::TcpSocket &client, const ClientMessage& message);
    ServerMessage onResignGame(sf::TcpSocket &client, const ClientMessage& message);
};