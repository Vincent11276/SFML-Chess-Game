#include <iostream>

#include "ChessServer.hpp"
#include "Client/Scenes/Game.hpp"

int main()
{
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