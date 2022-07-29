#pragma once

#include <SFML/Network/Packet.hpp>
#include <unordered_map>
#include <list>

#include "Server.hpp"


struct Player
{
    unsigned int id;
    std::string name;
};

struct Room
{
    std::string id;
    std::string name;

    std::pair<Player, Player> players;
};

class ChessServer : public Server
{
public:
    ChessServer()
    {
        
    }

private:
    std::unordered_map<unsigned int, Room> m_rooms;

    void onNewClientConnection(sf::TcpSocket &client) override
    {
        sf::Packet packet;
        packet << "hello user";

        this->unicast(client, packet);
    }

    void onClientDisconnect(sf::TcpSocket &client) override
    {

    }

    void onPacketReceived(sf::TcpSocket &client, sf::Packet& packet) override
    {
        std::cout << "Received: " << packet << std::endl;

        
        
    }

};