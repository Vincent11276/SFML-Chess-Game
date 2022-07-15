#pragma once 

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "../Inputs/Mouse.hpp"
#include "../Engine/PieceType.hpp"
#include "../Engine/ResourceKey.hpp"



class MoveablePiece: public sf::Drawable, public sf::Transformable
{
public:
    MoveablePiece()
    {
        // default..
    }

    void changeType(int pieceType)
    {
        if (m_resourceKeys.find(pieceType) != m_resourceKeys.end())
        {
            moveablePiece_Spr.setTexture(
                ResourceManager::getTexture(m_resourceKeys[pieceType]));
        }
        else
        {
            Logger::getInstance().log(LogLevel::DEBUG,
                "Unknown piece type enum value received for setting moveable chess piece sprite");
        }
    }

    void show()
    {
        // update transforms
        auto mouse = MouseInput::getRelativePosition();
        moveablePiece_Spr.setPosition(mouse.x, mouse.y);
        Helper::centerSprite(&moveablePiece_Spr);
    
        m_isVisible = true;
    }

    void hide()
    {
        m_isVisible = false;
    }

    void followMouse()
    {
        auto mouse = MouseInput::getRelativePosition();

		moveablePiece_Spr.setPosition(mouse.x, mouse.y);
    }

private:
    static std::unordered_map<int, ResourceKey> m_resourceKeys =
    {
        { PieceType::B_PAWN, ResourceKey::WoodPiecePawnB },
        { PieceType::B_ROOK, ResourceKey::WoodPieceRookB },
        { PieceType::B_KNIGHT, ResourceKey::WoodPieceKnightB },
        { PieceType::B_BISHOP, ResourceKey::WoodPieceBishopB },
        { PieceType::B_QUEEN, ResourceKey::WoodPieceQueenB },
        { PieceType::B_KING, ResourceKey::WoodPieceKingB },
        { PieceType::W_PAWN, ResourceKey:: WoodPiecePawnW},
        { PieceType::W_ROOK, ResourceKey::WoodPieceRookW },
        { PieceType::W_KNIGHT, ResourceKey::WoodPieceKnightW },
        { PieceType::W_BISHOP, ResourceKey::WoodPieceBishopW },
        { PieceType::W_QUEEN, ResourceKey::WoodPieceKingW },
        { PieceType::W_KING, ResourceKey::WoodPieceQueenW },
    };

    bool m_isVisible = false;

    sf::Sprite moveablePiece_Spr;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states)
    {
        if (m_isVisible)
        {
            states.transform *= getTransform();

            target.draw(moveablePiece_Spr);
        }
    }
};