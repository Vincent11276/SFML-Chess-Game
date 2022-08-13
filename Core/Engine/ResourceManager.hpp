#pragma once

#include <map>
#include <utility>
#include <string>

#include "../Utility/Logger.hpp"
#include "../Engine/LoadResult.hpp"
#include "../Engine/ResourceKey.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <format>
#include <iostream>


namespace engine
{
    class ResourceManager
    {
    public:
        ResourceManager() = delete;

        static bool addTexture(uint32_t key, std::string filePath);

        static const sf::Texture& getTexture(uint32_t key);

    private:
        static std::unordered_map<uint32_t, std::unique_ptr<sf::Texture>> m_textures;
    };
}