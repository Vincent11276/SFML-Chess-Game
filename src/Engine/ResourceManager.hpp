#pragma once

#include <map>
#include <utility>
#include <string>

#include "Resource.hpp"
#include "Singleton.hpp"
#include "../Utility/Logger.hpp"
#include "RTexture.hpp"

#include <memory>
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
    ResourceManager() = delete;

    static void addResource(Resource* resource, const std::string& name, const std::string& path);

    template <typename T>
    static T* getResource(const std::string& name);

    static void eraseResource(const std::string& name);

private:    
    static std::unordered_map<std::string, std::unique_ptr<Resource>> m_resource;
};