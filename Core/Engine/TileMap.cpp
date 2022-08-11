#include "TileMap.hpp"


TileMap::TileMap()
{

}

TileMap::TileMap(sf::Vector2i p_mapSize, sf::Vector2f p_cellSize)
{
    this->mapdata.resize(p_mapSize.y, std::vector<int>(p_mapSize.x, -1));
    this->tileVertices.resize(static_cast<size_t>(p_mapSize.x) * p_mapSize.y * 4);
    this->tileVertices.setPrimitiveType(sf::Quads);
}

void TileMap::setTileSet(std::shared_ptr<TileSet>& tileSet)
{
    this->m_tileSet = tileSet;
}

void TileMap::mapCellsFrom(const std::vector<std::vector<int>>& p_cells)
{
    for (int y = 0; y < this->getDimension().y; y++)
    {
        for (int x = 0; x < this->getDimension().x; x++)
        {
            this->setCell(p_cells[y][x], sf::Vector2i(x, y));
        }
    }
}

sf::Vector2i TileMap::getDimension()
{
    if (this->mapdata.size() > 0)
    {
        return sf::Vector2i(this->mapdata[0].size(), this->mapdata.size());
    }
    else return { 0, 0 };
}

void TileMap::setCell(int id, sf::Vector2i p_coords)
{    
    if (this->isInBounds(p_coords) && id < m_tileSet->getTileCount())
    {
        this->mapdata[p_coords.y][p_coords.x] = id;

        this->setTileVertices(id, p_coords);
    }
}

int TileMap::getCell(sf::Vector2i p_coords)
{
    if (this->isInBounds(p_coords))
    {
        return this->mapdata[p_coords.y][p_coords.x];
    }
    return -1;
}

void TileMap::remove(sf::Vector2i p_coords)
{
    this->setCell(-1, p_coords);
}

sf::Vector2i TileMap::worldToMap(sf::Vector2i p_coords)
{
    sf::Vector2f cellSize = this->m_tileSet->getTileSize();

    return sf::Vector2i(p_coords.x / cellSize.x, p_coords.y / cellSize.y);
}

sf::Vector2i TileMap::mapToWorld(sf::Vector2i p_coords)
{
    sf::Vector2f cellSize = this->m_tileSet->getTileSize();

    return sf::Vector2i(p_coords.x * cellSize.x, p_coords.y * cellSize.y);
}

bool TileMap::isInBounds(sf::Vector2i p_coords)
{
    sf::Vector2i mapSize = getDimension();

    return (p_coords.x >= 0) && (p_coords.x < mapSize.x) &&
        (p_coords.y >= 0) && (p_coords.y < mapSize.y);
}

void TileMap::setTileVertices(int id, sf::Vector2i p_coords)
{

    // get a pointer to the current tile's quad
    sf::Vertex* quad = &tileVertices[(p_coords.x + static_cast<size_t>(p_coords.y) * getDimension().x) * 4];

    sf::Vector2f cellSize = this->m_tileSet->getTileSize();

    // define its 4 corners
    quad[0].position = sf::Vector2f(p_coords.x * cellSize.x, p_coords.y * cellSize.y);
    quad[1].position = sf::Vector2f((p_coords.x + 1) * cellSize.x, p_coords.y * cellSize.y);
    quad[2].position = sf::Vector2f((p_coords.x + 1) * cellSize.x, (p_coords.y + 1) * cellSize.y);
    quad[3].position = sf::Vector2f(p_coords.x * cellSize.x, (p_coords.y + 1) * cellSize.y);

    // check if request for remove
    if (id == -1) {
        int lastTile = m_tileSet->getTileCount();

        quad[0].texCoords = sf::Vector2f(lastTile * cellSize.x, 0);
        quad[1].texCoords = sf::Vector2f(lastTile * cellSize.x + cellSize.x, 0);
        quad[3].texCoords = sf::Vector2f(lastTile * cellSize.x, cellSize.y);
        quad[2].texCoords = sf::Vector2f(lastTile * cellSize.x + cellSize.x, cellSize.y);
    }
    else {
    
        // define its 4 texture coordinates
        quad[0].texCoords = sf::Vector2f(id * cellSize.x, 0);
        quad[1].texCoords = sf::Vector2f(id * cellSize.x + cellSize.x, 0);
        quad[3].texCoords = sf::Vector2f(id * cellSize.x, cellSize.y);
        quad[2].texCoords = sf::Vector2f(id * cellSize.x + cellSize.x, cellSize.y);
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();

    states.texture = &this->m_tileSet->getTileSet();

    target.draw(this->tileVertices, states);
    //target.draw(sf::Sprite(this->m_tileSet->getTileSet()));
}

void TileMap::clear()
{

    mapdata.clear();

    m_tileSet->clear();

    tileVertices.clear();
} 