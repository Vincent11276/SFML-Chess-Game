#include "PieceMarker.hpp"


PieceMarker::PieceMarker()
    : m_validMoves_TileMap({ 8, 8 }, { 80.f, 80.f }),
      m_highlights_TileMap({ 8, 8 }, { 80.f, 80.f }),
      m_hoveringTile_TileMap({ 8, 8 }, { 80.f, 80.f })
{
    // default.. 
}

void PieceMarker::init()
{
    m_tileset = std::make_shared<engine::TileSet>();

    if (!m_tileset->create({ 80.f, 80.f }, 4))  
    {
        Logger::error("Unable to create tileset");
    }

    m_tileset->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::HoveringMark));
    m_tileset->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::ActionMark));
    m_tileset->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::CanMoveMark));
    m_tileset->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::CanTakeMark));

    m_validMoves_TileMap.setTileSet(m_tileset);
    m_highlights_TileMap.setTileSet(m_tileset);
    m_hoveringTile_TileMap.setTileSet(m_tileset);
}

void PieceMarker::setSelectedPiece(const sf::Vector2i& coords)
{
    this->resetSelectedPiece();

    m_highlights_TileMap.setCell(ACTION_MARK, coords);

    m_selectedCoords = coords;
}

void PieceMarker::setValidMoves(const std::vector<chess::PieceMovement>& validMoves)
{
    this->resetValidMoves();

    for (auto& validMove : validMoves)
    {
        switch (validMove.action)
        {
        case chess::PieceAction::Relocate:
        case chess::PieceAction::TwoSquaresForward:
        case chess::PieceAction::CastleLeft:
        case chess::PieceAction::CastleRight:
            m_validMoves_TileMap.setCell(CAN_MOVE_MARK, validMove.coords);
            break;

        case chess::PieceAction::Capture:
        case chess::PieceAction::EnPessantUp:
        case chess::PieceAction::EnPessantDown:
            m_validMoves_TileMap.setCell(CAN_TAKE_MARK, validMove.coords);
            break;

        default:
            break;
        }
    }
}

void PieceMarker::setHoveringTile(const sf::Vector2i& coords)
{
    this->resetHoveringTile();

    m_hoveringTile_TileMap.setCell(HOVERING_MARK, coords);
}

void PieceMarker::setPreviousMove(const sf::Vector2i& selected, const sf::Vector2i& target)
{
    this->resetPreviousMove();

    m_highlights_TileMap.setCell(ACTION_MARK, selected);
    m_highlights_TileMap.setCell(ACTION_MARK, target);

    m_prevMove = std::make_pair(selected, target);
}


void PieceMarker::resetSelectedPiece()
{
    // check if previously selected coords does not match any previously moved coords
    if (m_prevMove.first != m_prevMove.second)
    {
        if (m_selectedCoords != m_prevMove.first || m_selectedCoords != m_prevMove.second)
        {
            m_highlights_TileMap.remove(m_selectedCoords);
        }
    }
}

void PieceMarker::resetValidMoves()
{
    m_validMoves_TileMap.clear();
}

void PieceMarker::resetHoveringTile()
{
    m_hoveringTile_TileMap.clear();
}

void PieceMarker::resetPreviousMove()
{
    m_highlights_TileMap.remove(m_prevMove.first);
    m_highlights_TileMap.remove(m_prevMove.second);
}


void PieceMarker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

    target.draw(m_highlights_TileMap, states);
    target.draw(m_hoveringTile_TileMap, states);
    target.draw(m_validMoves_TileMap, states);
}
