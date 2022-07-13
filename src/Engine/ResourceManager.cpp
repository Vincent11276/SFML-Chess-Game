#include "ResourceManager.hpp"


void ResourceManager::addResource(Resource* resource, const std::string& name, const std::string& path)
{
    resource->loadResource();

    m_resource.insert(std::make_pair(name, resource));
}

template <typename T>
T* ResourceManager::getResource(const std::string& name)
{
    return dynamic_cast<T*>(m_resource.find(name)->second);
}

void ResourceManager::eraseResource(const std::string& name)
{
    m_resource.erase(name);
}