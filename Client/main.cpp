#include <iostream>

#include "ChessServer.hpp"
#include "Client/Scenes/Game.hpp"

int main()
{
        // Game::getInstance()->run();

    std::cout << "Enter" << std::endl;

    std::string input;
    std::cin >> input;

    if (input == "s")
    {
        ChessServer chessServer;
        chessServer.initServer();
    }
    else if (input == "c")
    {
        Game::getInstance()->run();
    }

    std::cin >> input;
    
    return EXIT_SUCCESS;
}