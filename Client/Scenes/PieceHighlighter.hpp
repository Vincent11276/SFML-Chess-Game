#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Core/Engine/TileMap.hpp"
#include "Core/Engine/ResourceManager.hpp"
#include "Core/Utility/Logger.hpp"
#include "Core/Chess/MoveGenerator.hpp"


class PieceHighlighter : public sf::Drawable, public sf::Transformable
{
public:
    PieceHighlighter()
        : m_tilemap({ 8, 8 }, { 80.f, 80.f })
    {
        m_tileSet = std::make_shared<egn::TileSet>();

        if (!m_tileSet->create({ 80.f, 80.f }, 3))
        {
            Logger::error("Unable to create tileset object for PieceHighlighter class");
        }
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::ActionMark));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::CanMoveMark));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::CanTakeMark));

        m_tilemap.setTileSet(m_tileSet);
    }

    void clear()
    {
        m_tilemap.clear();
    }    

    void markValidMoves(const std::vector<chess::PieceMovement> &validMoves)
    {
        for (auto& validMove: validMoves)
		{
            switch (validMove.action)
            {
            case chess::PieceAction::Relocate:
                m_tilemap.setCell(1, validMove.coords);
                break;

             case chess::PieceAction::TwoSquaresForward:
                m_tilemap.setCell(1, validMove.coords);
                break;

            case chess::PieceAction::Capture:
                m_tilemap.setCell(2, validMove.coords);
                break;

            case chess::PieceAction::CastleLeft:
            case chess::PieceAction::CastleRight:
                m_tilemap.setCell(1, validMove.coords);
                break;

            case chess::PieceAction::EnPessantUp:
            case chess::PieceAction::EnPessantDown:
                m_tilemap.setCell(1, validMove.coords);
                break;

            default:
                break;
            }
		}
    }


    void unmarkValidMoves(const std::vector<chess::PieceMovement> &validMoves)
    {
        for (auto& validMove: validMoves)
        {
            m_tilemap.remove(validMove.coords);
        }
    }

    void markAsHighlighted(const sf::Vector2i& coords)
    {
        m_tilemap.setCell(0, coords);
    }

    void remove(const sf::Vector2i& coords)
    {
        m_tilemap.setCell(-1, coords);
    }


    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= this->getTransform();

        target.draw(m_tilemap, states);
    }

private:
    std::shared_ptr<egn::TileSet> m_tileSet;
    egn::TileMap m_tilemap;
};