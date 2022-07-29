#pragma once 

#include "SFML/Config.hpp"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/Network/Socket.hpp"
#include "SFML/Network/TcpSocket.hpp"

#include <string>
#include <iostream>

struct Player
{
    std::string id;
    std::string name;
};

enum Type
{
    Register,
    CreateNewRoom,
    JoinExistingRoom,
    FetchAvailableRooms,
    PieceMovement,  
    RequestForDraw,
    ResignGame
};

class ChessClient
{
public:
    sf::IpAddress remoteAddress = sf::IpAddress::getLocalAddress();
    unsigned short remotePort = 53000;

    ChessClient()
    {
        // default..
    }

    void connect()
    {
        m_socket.connect(remoteAddress, remotePort);
    }

    void createNewRoom()
    {
        sf::Packet packet;
        packet << m_id << Type::CreateNewRoom << "poop";

    }

private:
    unsigned int m_id;
    
    void sendPacket(sf::Packet& packet)
    {
        if (m_socket.send(packet) != sf::Socket::Done)
        {
            std::cout << "Failed to send packet to the server!" << std::endl;
        }
    }

    void handleIncomingPacketsAsync()
    {
        while (true)
        {
            sf::Packet receivedPacket;

            if (m_socket.receive(receivedPacket) != sf::Socket::Done)
            {
                std::cout << "Failed to receive data from " << m_socket.getRemoteAddress().getPublicAddress(sf::seconds(5)) << std::endl; 
            }
            else this->onPacketReceived(m_socket, receivedPacket);
        }
    }

    void onPacketReceived()
    {

    }
    sf::TcpSocket m_socket;
};