#pragma once

#include <string>


class Resource
{
public:
    Resource();
    virtual ~Resource();

    virtual bool loadResource() = 0;

    std::string filePath;
    bool isLoaded = false;
};