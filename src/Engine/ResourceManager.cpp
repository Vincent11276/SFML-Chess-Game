#include "ResourceManager.hpp"


std::unordered_map<uint32_t, std::unique_ptr<sf::Texture>> ResourceManager::m_textures { };


bool ResourceManager::addTexture(uint32_t key, std::string filePath)
{
    auto newTexture = std::make_unique<sf::Texture>();

    if (!newTexture->loadFromFile(filePath))
    {
        Logger::getInstance().log(LogLevel::ERROR,
            "Failed to load \"" + filePath + "\" for texture resource");
            
        return LoadResult::Failure;
    }
    m_textures.emplace(std::make_pair(key, std::move(newTexture)));

    return LoadResult::Success;
}

const sf::Texture& ResourceManager::getTexture(uint32_t key)
{
    if (m_textures.find(key) == m_textures.end())
    {
        Logger::getInstance().log(LogLevel::ERROR,
            std::format("The key '{}' you are trying to access doesn't exists!", key));
    }
    return *m_textures[key];
}

