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
#include "ClientSession.hpp"
#include "Core/Utility/Helper.hpp"
#include "Core/Utility/Logger.hpp"

constexpr auto DEFAULT_ROOM_SIZE = 2;
constexpr auto JOIN_CODE_SIZE = 5;


class ChessServer : public Server
{
public:
    sf::Uint32 maxPlayers = 100;

    ChessServer();

    void addPlayerToRoom(Room* room, sf::Uint32 token);
    
private:
    std::unordered_map<sf::Uint32, ClientSession> m_sessions;

    // hashes
    std::unordered_map<sf::Uint32, Room> m_rooms;

    void onNewClientConnection(sf::TcpSocket& client) override { };
    void onClientDisconnect(sf::TcpSocket& client) override { };
    void onPacketReceived(sf::TcpSocket &client, sf::Packet& packet) override;

    void sendMessage(sf::TcpSocket* client, ServerMessage& message);

    ServerMessage onAuthenticate(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onRegisterPlayer(sf::TcpSocket& client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onCreateNewRoom(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onJoinExistingRoom(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onFetchAvailableRooms(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onPieceMovement(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onRequestForDraw(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onResignGame(sf::TcpSocket &client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onPlayerSendMessage(sf::TcpSocket& client, sf::Uint32 token, const ClientMessage& message);
    ServerMessage onPlayerReadyToPlay(sf::TcpSocket& client, sf::Uint32 token, const ClientMessage& message);
};