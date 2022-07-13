#pragma once

#include "Resource.hpp"
#include "../Utility/Logger.hpp"

#include <SFML/Graphics/Texture.hpp>


class RTexture : public Resource
{
public:
	RTexture();

	bool loadResource() override;

private:
	sf::Texture texture;
};