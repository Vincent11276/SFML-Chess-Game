#pragma once

#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/Network/Socket.hpp"
#include "SFML/Network/TcpSocket.hpp"
#include "SFML/System/Time.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <stdlib.h>
#include <vector>
#include <thread>


class Server
{
public:
    unsigned short port = 53000;
    bool isAcceptIncomingClients = true;

    Server()
    {
        // default..
    }

    void initServer()
    {
        std::cout << "Server has started at port " << port << std::endl;

        this->mainLoop();
    }

    bool unicast(sf::TcpSocket& client, sf::Packet &packet)
    {
        if (client.send(packet) != sf::Socket::Done)
        {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    unsigned int broadcast(sf::Packet &packet)
    {
        unsigned int successCount;

        for (auto& client: m_clients)
        {
            if (client.send(packet) == sf::Socket::Done)
            {
                successCount++;
            }
        }
        return successCount;
    }

protected:
    std::list<sf::TcpSocket> m_clients;
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;

    void mainLoop()
    {
        std::thread t1(&Server::checkForIncommingConnections, this);

        while (true)
        {
            if (m_selector.wait(sf::seconds(10.f)))
            {
                for (auto& socket: m_clients)
                {
                    if (m_selector.isReady(socket))
                    {
                        sf::Packet receivedPacket;

                        if (socket.receive(receivedPacket) != sf::Socket::Done)
                        {
                            std::cout << "Failed to receive data from " << socket.getRemoteAddress().getPublicAddress(sf::seconds(5)) << std::endl; 
                        }
                        else this->onPacketReceived(socket, receivedPacket);
                    }                
                }
            }
        }
    }

    void checkForIncommingConnections()
    {
        while (isAcceptIncomingClients)
        {
            if (m_listener.listen(port) != sf::Socket::Done) continue;
    
            if (!isAcceptIncomingClients) continue;

            m_clients.emplace_back();

            if (m_listener.accept(m_clients.back()) == sf::Socket::Done)
            {     
                std::cout << "new connection" << std::endl;
                m_selector.add(m_clients.back());
                this->onNewClientConnection(m_clients.back());           
            }
            else m_clients.pop_back();
        }
    }

    virtual void onNewClientConnection(sf::TcpSocket &client) = 0;
    virtual void onClientDisconnect(sf::TcpSocket &client) = 0;
    virtual void onPacketReceived(sf::TcpSocket &client, sf::Packet& packet) = 0;
};