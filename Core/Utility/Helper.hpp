#pragma once

#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

namespace Helper
{
    inline std::string extractFileFormat(const std::string& path)
    {
        for (int32_t i = path.size(); i >= 0; i--)
        {
            if (path[i] == '.')
            {
                return path.substr(i, path.size());
            }
        }
        return std::string();
    }

    template<typename T>
    inline bool isElementExists(const T& element, const std::vector<T> arr)
    {
        return std::find(arr.begin(), arr.end(), element) != arr.end();
    }

    template<typename T>
    inline std::string ToString(const std::vector<T>& arr)
    {
        std::ostringstream ss;

        ss << '[';

        for (size_t i = 0; i < arr.size(); i++)
        {
            ss << '(' << arr[i].x << ", " << arr[i].y << ')';

            if (i < arr.size() - 1)
            {
                ss << ", ";
            }
        }
        ss << ']';

        return ss.str();
    }

    inline void centerSprite(sf::Sprite *sprite)
    {    
        sprite->setOrigin(sprite->getGlobalBounds().getSize() / 2.f);
    }

   // lol


//     template<enum T> :requires  std::is_same_v(T, ChessGame::PieceColor)
//     inline std::string EnumToString(T pieceColor)
//     {
// 		switch (pieceColor)
// 		{
// 		case ChessGame::PieceColor::Black:
// 			return "Black";
// 
// 		case ChessGame::PieceColor::White:
// 			return "White";
// 		};
//     }



 //build
//     inline const std::string ToString2(/*const ChessGame::PieceColor pieceColor*/) // I don't think I have that existing
//     {
//         switch (pieceColor)
//         {
//         case ChessGame::PieceColor::Black:
//             return "Black";
// 
//         case ChessGame::PieceColor::White:
//             return "White";
//         };
//       
//     }
}


