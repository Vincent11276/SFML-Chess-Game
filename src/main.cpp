#pragma once

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