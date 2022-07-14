// #include <iostream>
// #include <string>
// #include <filesystem>

// using std::cout; using std::cin;
// using std::endl; using std::string;
// using std::filesystem::current_path;

// int main() {
//     cout << "Current working directory: " << current_path() << endl;

//     return EXIT_SUCCESS;
// }

#include <bitset>
#include <vector>
#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Scenes/Game.hpp"



int main()
{
	Game game;
    game.run();

    return EXIT_SUCCESS;
}

// {
//   "configurations": [
//     {
//       "name": "x64-Debug",
//       "generator": "Ninja",
//       "configurationType": "Debug",
//       "buildRoot": "${projectDir}\\out\\build\\${name}",
//       "installRoot": "${projectDir}\\out\\install\\${name}",
//       "cmakeCommandArgs": "",
//       "buildCommandArgs": "",
//       "ctestCommandArgs": ""
//     }
//   ]
// }