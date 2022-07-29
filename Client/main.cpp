#include <iostream>

#include "ChessServer.hpp"
#include "SFML/Network/Packet.hpp"


int main()
{
    std::string input;
    std::cin >> input;

    if (input == "s")
    {
        ChessServer chessServer;
        chessServer.initServer();
    }
    else if (input == "c")
    {
        std::cout << "Running as Client" << std::endl;
        
        std::cout << "Target IP: " << sf::IpAddress::getLocalAddress() << std::endl; 
        std::cout << "Target Port: 53000" << std::endl; 


        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect(sf::IpAddress::getLocalAddress(), 53000);
        if (status != sf::Socket::Done)
        {
            // error...
        }

        while (true)
        {
            std::string data;

            std::cout << "Enter message to send: ";
            std::cin >> data;
        
            sf::Packet packet;
            packet << data;

            // TCP socket:
            if (socket.send(packet) != sf::Socket::Done)
            {
                std::cout << " -- Failed"<< std::endl;
            }
            else std::cout << " -- Success"<< std::endl;
        }   
    }

    std::cin >> input;
    
    return EXIT_SUCCESS;
}


















// #include "TGUI/Layout.hpp"
// #include <TGUI/TGUI.hpp>
// #include <vadefs.h>

// #include <bitset>
// #include <vector>
// #include <iostream>
// #include <memory>

// #include <SFML/Graphics.hpp>

// #include "Scenes/Game.hpp"

// int main()
// {
// 	Game game;
//     game.run();

//     return EXIT_SUCCESS;
// }