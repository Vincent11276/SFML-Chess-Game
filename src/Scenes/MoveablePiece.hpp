#pragma once 

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "../Inputs/Mouse.hpp"
#include "../Engine/PieceType.hpp"
#include "../Engine/ResourceManager.hpp"
#include "../Utility/Helper.hpp"



class MoveablePiece: public sf::Drawable, public sf::Transformable
{
public:
    MoveablePiece();

    void changeType(int pieceType);

    void show();

    void hide();

    void followMouse();

private:
    static std::unordered_map<int, ResourceKey> m_resourceKeys;

    bool m_isVisible = false;

    sf::Sprite moveablePiece_Spr;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};