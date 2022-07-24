#pragma once 

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "Core/Inputs/Mouse.hpp"
#include "Core/Engine/ResourceManager.hpp"
#include "Core/Chess/PieceType.hpp"
#include "Core/Utility/Helper.hpp"



class MoveablePiece: public sf::Drawable, public sf::Transformable
{
public:
    MoveablePiece();

    void changeType(int pieceType);

    void followMouse();

private:
    static std::unordered_map<int, ResourceKey> m_resourceKeys;

    sf::Sprite moveablePiece_Spr;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};