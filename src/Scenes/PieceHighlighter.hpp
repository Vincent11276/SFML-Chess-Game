#pragma once

#include "../Engine/TileMap.hpp"
#include "../Utility/Logger.hpp"
#include "../Engine/ResourceManager.hpp"
#include "../Engine/MoveGenerator.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>



class PieceHighlighter : public sf::Drawable, public sf::Transformable
{
public:
    PieceHighlighter()
        : m_tilemap({ 8, 8 }, { 80.f, 80.f })
    {
        if (!m_tileSet.create({ 80.f, 80.f }, 3))
        {
            Logger::getInstance().log(LogLevel::ERROR,
                "Unable to create tileset object for PieceHighlighter class");
        }
        m_tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::ActionMark));
        m_tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::CanMoveMark));
        m_tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::CanTakeMark));

        m_tilemap.setTileSet(m_tileSet);
    }

    void clear()
    {
        m_tilemap.clear();
    }    

    void markValidMoves(const std::vector<PieceMovement> &validMoves)
    {
        for (auto& validMove: validMoves)
		{
            switch (validMove.action)
            {
            case PieceAction::Move:
                m_tilemap.setCell(1, validMove.coords);
                break;

            case PieceAction::Capture:
                m_tilemap.setCell(2, validMove.coords);
                break;

            case PieceAction::Castle:
                m_tilemap.setCell(1, validMove.coords);
                break;

            case PieceAction::EnPessant:
                m_tilemap.setCell(1, validMove.coords);
                break;

            default:
                break;
            }
		}
    }


    void unmarkValidMoves(const std::vector<PieceMovement> &validMoves)
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
    TileMap m_tilemap;
    TileSet m_tileSet;
};