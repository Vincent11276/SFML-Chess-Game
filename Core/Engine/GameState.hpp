#pragma once

#include <SFML/Graphics.hpp>

namespace egn
{
	class GameState
	{
	public:
		GameState() = default;

		virtual void init() = 0;
		virtual void cleanup() = 0;
		virtual void pause() = 0;
		virtual void resume() = 0;

		virtual void handleEvent([[maybe_unused]] sf::Event& event) = 0;
		virtual void update([[maybe_unused]] float deltaTime) = 0;
		virtual void physicsUpdate([[maybe_unused]] float deltaTime) = 0;
		virtual void draw([[maybe_unused]] sf::RenderTarget& target) const = 0;
	};
}