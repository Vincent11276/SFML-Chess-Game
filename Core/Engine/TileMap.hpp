#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <filesystem>
#include "../Utility/Helper.hpp"
#include "../Engine/TileSet.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap();

    TileMap(sf::Vector2i p_mapSize, sf::Vector2f p_cellSize);

    void setTileSet(TileSet& tileSet);

    void mapCellsFrom(const std::vector<std::vector<int>>& p_tiledata);

    void setCell(int id, sf::Vector2i p_coords);

    int getCell(sf::Vector2i p_coords);

    void remove(sf::Vector2i p_coords);
    
    sf::Vector2i getDimension();

    sf::Vector2i worldToMap(sf::Vector2i p_coords);

    sf::Vector2i mapToWorld(sf::Vector2i p_coords);

    bool isInBounds(sf::Vector2i p_coords);
    
    void clear();
private:
    TileSet* m_tileSet;
    
    std::vector<std::vector<int>> mapdata;
    
    sf::VertexArray tileVertices;
   
    void setTileVertices(int id, sf::Vector2i p_coords);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};